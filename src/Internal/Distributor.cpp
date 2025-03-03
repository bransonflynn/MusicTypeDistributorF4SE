#include "Internal/Distributor.hpp"
#include "Internal/Maps.hpp"
#include "Internal/Utility.hpp"

namespace Internal
{
	void Distributor::Distribute() noexcept
	{
		for (const auto& [music_type, pair] : Maps::distr_map) {
			const auto& [tracks, clear_list]{ pair };
			const auto music_type_name{ music_type->GetFormEditorID() };
			const auto music_type_form_id{ music_type->GetFormID() };
			if (clear_list) {
				logger::info(FMT_STRING("Clearing {} ({:08X})"),
					music_type_name, music_type_form_id);

				music_type->tracks.clear();
			}
			for (const auto& track : tracks) {
				logger::info(FMT_STRING("Distributing {} ({:08X}) to {} ({:08X})"),
					track->GetFormEditorID(), track->GetFormID(), music_type_name, music_type_form_id);

				music_type->tracks.emplace_back(track);
			}
		}

		for (const auto& [location, music_type] : Maps::location_distr_map) {
			logger::info(FMT_STRING("Updating location {} ({:08X}) music type to {} ({:08X})"),
				location->GetFormEditorID(), location->GetFormID(), music_type->GetFormEditorID(), music_type->GetFormID());

			location->musicType = music_type;
		}

		// regionDataList has not been decoded for Fallout 4 yet
		// for (const auto& [region, music_type] : Maps::region_distr_map) {
		// 	const auto reg_edid{ Utility::GetFormEditorID(region) };

		// 	for (const auto& reg_data : region->dataList->regionDataList) {
		// 		const auto manager{ RE::TESDataHandler::GetSingleton()->GetRegionDataManager() };
		// 		if (const auto reg_data_sound{ manager->AsRegionDataSound(reg_data) }) {
		// 			// logger::info("Updating region {} ({:08X}) music type to {} ({:08X})", reg_edid, region->GetFormID(), Utility::GetFormEditorID(music_type), music_type->GetFormID());
		// 			reg_data_sound->music = music_type;
		// 		}
		// 	}
		// }
	}
}
