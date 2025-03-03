#pragma once

namespace Internal
{
	struct EditorIDAndPluginName
	{
		std::string editor_id{};
		std::string plugin_name{};
	};

	class Utility
	{
	public:
		static std::vector<std::string> Split(std::string s) noexcept;

		static std::string GetFormEditorID(const RE::TESForm* a_form) noexcept;

		static auto ToFormID(const std::string& s) noexcept { return static_cast<RE::TESFormID>(std::stoul(s, nullptr, 16)); }

		static std::vector<RE::BGSMusicTrackFormWrapper*> BuildFormVec(const std::unordered_set<std::string>& tokens) noexcept;
	};
}
