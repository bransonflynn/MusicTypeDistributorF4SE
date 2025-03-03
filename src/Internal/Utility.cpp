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
}
