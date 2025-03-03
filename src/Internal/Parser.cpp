#include "Internal/Parser.hpp"
#include "Internal/Maps.hpp"
#include "Internal/Utility.hpp"

namespace Internal
{
	void Parser::ParseINIs(CSimpleIniA& ini) noexcept
	{
		const std::filesystem::path dataDir = R"(.\Data)";
		const auto suffix = L"_MUS.ini";

		if (!exists(dataDir)) {
			logger::error("ERROR: Failed to find Data directory");
			return;
		}

		std::vector<std::filesystem::path> musinis;
		std::string_view iniExtension = ".ini";

		// populate musinis
		for (std::error_code errCode{}; const auto& file : std::filesystem::directory_iterator{ dataDir, errCode }) {
			if (errCode.value()) {
				logger::warn(FMT_STRING("Error Code: {}"), errCode.value());
				continue;
			}

			const auto& path = file.path();
			if (path.extension() != iniExtension) {
				continue;
			}

			const auto fileName = path.filename();
			if (!fileName.wstring().ends_with(suffix)) {
				continue;
			}

			if (std::find(musinis.begin(), musinis.end(), path) != musinis.end()) {
				logger::warn(FMT_STRING("Found duplicate _MUS.ini file"), fileName.string());
				continue;
			}

			musinis.push_back(path);
		}

		// populate maps from inis
		const std::regex commaSpace{ ", " };

		for (const auto& file : musinis) {
			const auto filename = file.filename().string();
			logger::info(FMT_STRING("Loading config file: "), filename);

			ini.LoadFile(file.wstring().data());

			CSimpleIniA::TNamesDepend keys{};
			ini.GetAllKeys("General", keys);

			for (const auto& key : keys) {
				CSimpleIniA::TNamesDepend values{};
				ini.GetAllValues("General", key.pItem, values);
				for (const auto& value : values) {
					const auto trimmed = std::regex_replace(std::string{ value.pItem }, commaSpace, ", ");
					Maps::general_prep_map[key.pItem] = value.pItem;
					logger::info(FMT_STRING("Added [{}: {}] to general_prep_map"), key.pItem, trimmed);
				}
			}

			// location setup
			if (ini.SectionExists("Location")) {
				CSimpleIni::TNamesDepend locationKeys{};
				ini.GetAllKeys("Location", locationKeys);

				for (const auto& key : locationKeys) {
					CSimpleIniA::TNamesDepend values{};
					ini.GetAllValues("Location", key.pItem, values);
					for (const auto& value : values) {
						Maps::location_prep_map[key.pItem] = value.pItem;
						logger::info(FMT_STRING("Added [{}: {}] to location_prep_map"), key.pItem, value.pItem);
					}
				}
			}

			// region setup
			if (ini.SectionExists("Region")) {
				CSimpleIniA::TNamesDepend regionKeys{};
				ini.GetAllKeys("Region", regionKeys);

				for (const auto& key : regionKeys) {
					CSimpleIniA::TNamesDepend values{};
					ini.GetAllValues("Region", key.pItem, values);
					for (const auto& value : values) {
						Maps::region_prep_map[key.pItem] = value.pItem;
						logger::info(FMT_STRING("Added [{}: {}] to region_prep_map"), key.pItem, value.pItem);
					}
				}
			}

			ini.Reset();
		}
	}
}
