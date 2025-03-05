#include "Internal/Utility.hpp"

namespace Internal
{
	std::vector<std::string> Utility::Split(std::string s) noexcept
	{
		std::vector<std::string> result{};

		std::size_t pos{};
		while ((pos = s.find(',')) != std::string::npos) {
			result.emplace_back(s.substr(0, pos));
			s.erase(0, pos + 1);
		}

		if (!s.empty()) {
			result.emplace_back(s);
		}

		return result;
	}

	std::vector<RE::BGSMusicTrackFormWrapper*> Utility::BuildFormVec(const std::unordered_set<std::string>& tokens) noexcept
	{
		std::vector<RE::BGSMusicTrackFormWrapper*> result;

		for (const auto& token : tokens) {
			const auto music_track = RE::TESForm::GetFormByEditorID(RE::BSFixedString(token))->As<RE::BGSMusicTrackFormWrapper>();
			if (music_track) {
				logger::info("\tFound music track {} ({:08X})", music_track->GetFormEditorID(), music_track->GetFormID());
				result.emplace_back(music_track);
			}
		}

		return result;
	}

	std::string Utility::IsIniFileModName(std::string filename)
	{
		// TODO improve this garbage

		// check for modname
		std::string::size_type index = filename.find(mus_suffix);
		if (index != std::string::npos) {
			filename.erase(index, mus_suffix.length());
		}
		logger::info(FMT_STRING("filename erase: {}"), filename);

		std::string extension = filename.substr(filename.length() - 4, 4);
		logger::info(FMT_STRING("extension: {}"), extension);

		if (extension == ".esp" || extension == ".esm" || extension == ".esl") {
			logger::info(FMT_STRING("filename was esp file"));
			return filename; // true
		}
		else {
			return ""; // false
		}
	}

	bool Utility::IsPluginInstalled(std::string_view name)
	{
		auto dataHandler = RE::TESDataHandler::GetSingleton();

		auto* modInfo = dataHandler->LookupLoadedModByName(name);
		if (modInfo) {
			return true;
		}

		modInfo = dataHandler->LookupLoadedLightModByName(name);
		if (modInfo) {
			return true;
		}

		return false;
	}
}
