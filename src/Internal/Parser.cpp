#include "Internal/Parser.hpp"
#include "Internal/Maps.hpp"
#include "Internal/Utility.hpp"
#include "parallel_hashmap/btree.h"

namespace Internal
{
	void Parser::ParseINIs(CSimpleIniA& ini) noexcept
	{
		const std::filesystem::path data_dir{ R"(.\Data)" };
		const auto pattern{ L"_MUS.ini" };

		if (!exists(data_dir)) {
			logger::error("ERROR: Failed to find Data directory"sv);
			return;
		}

		phmap::btree_set<std::filesystem::path> mus_inis;

		for (std::error_code ec{}; const auto& file : std::filesystem::directory_iterator{ data_dir, ec }) {
			if (ec.value()) {
				logger::warn(FMT_STRING("ERROR CODE: {}"), ec.value());
				continue;
			}

			const auto& path{ file.path() };
			if (path.extension() != ".ini") {
				continue;
			}

			const auto filename{ path.filename() };
			if (!filename.wstring().ends_with(pattern)) {
				continue;
			}

			if (mus_inis.contains(path)) {
				logger::warn(FMT_STRING("WARNING: Found duplicate _MUS.ini file: {}"), filename.string());
				continue;
			}

			mus_inis.insert(path);
		}

		const std::regex comma_space{ ", " };

		for (const auto& f : mus_inis) {
			const auto filename{ f.filename().string() };

			logger::info(FMT_STRING("Loading config file: {}"), filename);

			ini.LoadFile(f.wstring().data());

			CSimpleIniA::TNamesDepend keys{};
			ini.GetAllKeys("General", keys);

			for (const auto& k : keys) {
				CSimpleIniA::TNamesDepend values{};
				ini.GetAllValues("General", k.pItem, values);
				for (const auto& v : values) {
					const auto trimmed{ std::regex_replace(std::string{ v.pItem }, comma_space, ",") };
					auto trimmed_split = Utility::Split(trimmed);

					// check for invalid EditorIDs
					for (int i = 0; i < trimmed_split.size(); i++) {
						if (RE::TESForm::GetFormByEditorID(RE::BSFixedString(trimmed_split[i])) == nullptr) {
							logger::warn(FMT_STRING("Invalid EditorID: {} was found in ini file {}. This form was not inserted. Please fix this in the _MUS.ini file."),
								trimmed_split[i], filename);
							trimmed_split.erase(trimmed_split.begin() + i);
						}
					}

					if (trimmed_split.size() > 0) {
						Maps::prep_map[k.pItem].insert_range(trimmed_split);
						logger::info(FMT_STRING("Added [{}: {}] to prep map"), k.pItem, trimmed);
					}
					else {
						logger::warn(FMT_STRING("Did not add [{}: {}] to prep map. There were no valid EditorIDs."), k.pItem, trimmed);
					}
				}
			}

			if (ini.SectionExists("Location")) {
				CSimpleIniA::TNamesDepend loc_keys{};
				ini.GetAllKeys("Location", loc_keys);

				for (const auto& k : loc_keys) {
					CSimpleIniA::TNamesDepend values{};
					ini.GetAllValues("Location", k.pItem, values);
					for (const auto& v : values) {
						Maps::location_prep_map[k.pItem] = v.pItem;
						logger::info(FMT_STRING("Added [{}: {}] to location prep map"), k.pItem, v.pItem);
					}
				}
			}

			// regionDataList has not been decoded for Fallout 4 yet
			// if (ini.SectionExists("Region")) {
			// 	CSimpleIniA::TNamesDepend reg_keys{};
			// 	ini.GetAllKeys("Region", reg_keys);

			// for (const auto& k : reg_keys) {
			// 	CSimpleIniA::TNamesDepend values{};
			// 	ini.GetAllValues("Region", k.pItem, values);
			// 	for (const auto& v : values) {
			// 		Maps::region_prep_map[k.pItem] = v.pItem;
			// 		logger::debug("Added [{}: {}] to region prep map", k.pItem, v.pItem);
			// 	}
			// }

			ini.Reset();
		}
	}

	void Parser::PrepareDistributionMap() noexcept
	{
		for (const auto& [k, v] : Maps::prep_map) {
			auto k_copy{ k };

			// clear check
			bool clear_list{};
			if (k_copy.back() == '!') {
				k_copy = k_copy.substr(0, k_copy.size() - 1);
				clear_list = true;
			}
			// general prep
			if (const auto music_type{ RE::TESForm::GetFormByEditorID(RE::BSFixedString(k_copy))->As<RE::BGSMusicType>() }) {
				logger::info(FMT_STRING("Found music type {} ({:08X})"),
					music_type->GetFormEditorID(), music_type->GetFormID());

				const auto form_vec = Utility::BuildFormVec(v);
				Maps::distr_map[music_type] = { form_vec, clear_list };
			}
			else {
				logger::warn(FMT_STRING("An EditorID was parsed that did not correspond with a form. EditorID: {}"),
					k_copy);
			}

			// location distribution prep
			for (const auto& [loc, mus] : Maps::location_prep_map) {
				if (const auto location{ RE::TESForm::GetFormByEditorID(RE::BSFixedString(loc))->As<RE::BGSLocation>() }) {
					logger::info(FMT_STRING("Found location {} ({:08X}) in {}"),
						Utility::GetFormEditorID(location), location->GetFormID(), location->GetFile()->GetFilename());

					if (Maps::created_forms_map.contains(mus)) {
						const auto music_type{ Maps::created_forms_map[mus] };
						logger::info(FMT_STRING("\tFound created music type {} ({:08X})"),
							music_type->GetFormEditorID(), music_type->GetFormID());

						Maps::location_distr_map[location] = music_type;
					}
					else if (const auto music_type{ RE::TESForm::GetFormByEditorID(RE::BSFixedString(mus))->As<RE::BGSMusicType>() }) {
						logger::info(FMT_STRING("\tFound music type {} ({:08X})"),
							music_type->GetFormEditorID(), music_type->GetFormID());

						Maps::location_distr_map[location] = music_type;
					}
				}
			}

			// regionDataList has not been decoded for Fallout 4 yet
			// region distribution prep
			// for (const auto& [reg, mus] : Maps::region_prep_map) {
			// 	if (const auto region{ RE::TESForm::GetFormByEditorID(RE::BSFixedString(reg))->As<RE::TESRegion>() }) {
			// 		logger::info(FMT_STRING("Found region {} ({:08X}) in {}"),
			// 			Utility::GetFormEditorID(region), region->GetFormID(), region->GetFile()->GetFilename());

			// 		if (Maps::created_forms_map.contains(mus)) {
			// 			const auto music_type{ Maps::created_forms_map[mus] };
			// 			logger::info(FMT_STRING("\tFound created music type {} ({:08X})"),
			// 				music_type->GetFormEditorID(), music_type->GetFormID());

			// 			Maps::region_distr_map[region] = music_type;
			// 		}
			// 		else if (const auto music_type{ RE::TESForm::GetFormByEditorID(RE::BSFixedString(mus))->As<RE::BGSMusicType>() }) {
			// 			logger::info(FMT_STRING("\tFound music type {} ({:08X})"),
			// 				music_type->GetFormEditorID(), music_type->GetFormID());

			// 			Maps::region_distr_map[region] = music_type;
			// 		}
			// 	}
			// }
		}
	}
}

// const auto factory = RE::ConcreteFormFactory<RE::BGSMusicType>::GetFormFactory();
// const auto new_music_type = factory->Create();
// new_music_type->SetFormEditorID(k_copy.c_str());
// logger::info(FMT_STRING("Creating new music type {} ({:08X})"),
// 	k_copy, new_music_type->GetFormID());

// Maps::created_forms_map[k_copy] = new_music_type;
// const auto form_vec{ Utility::BuildFormVec(v) };
// Maps::distr_map[new_music_type] = { form_vec, clear_list };
