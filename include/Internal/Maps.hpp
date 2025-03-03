#pragma once

#include "parallel_hashmap/phmap.h"

namespace Internal
{
	class Maps
	{
	public:
		// using
		using TPrepMap = phmap::parallel_flat_hash_map<std::string, std::unordered_set<std::string>>;

		using TMapPair = std::pair<std::vector<RE::BGSMusicTrackFormWrapper*>, bool>;

		using TDistrMap = phmap::parallel_flat_hash_map<RE::BGSMusicType*, TMapPair>;

		using TLocationRegionPrepMap = phmap::parallel_flat_hash_map<std::string, std::string>;

		using TLocationMap = phmap::parallel_flat_hash_map<RE::BGSLocation*, RE::BGSMusicType*>;

		using TRegionMap = phmap::parallel_flat_hash_map<RE::TESRegion*, RE::BGSMusicType*>;

		using TCreatedFormsMap = phmap::parallel_flat_hash_map<std::string, RE::BGSMusicType*>;

		// vars
		inline static TPrepMap prep_map;

		inline static TDistrMap distr_map;

		inline static TLocationRegionPrepMap location_prep_map;

		inline static TLocationMap location_distr_map;

		// inline static TLocationRegionPrepMap region_prep_map;

		// inline static TRegionMap region_distr_map;

		inline static TCreatedFormsMap created_forms_map;
	};
}
