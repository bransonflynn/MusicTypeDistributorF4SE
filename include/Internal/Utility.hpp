#pragma once

namespace Internal
{
	struct EditorIDAndPluginName
	{
		std::string editor_id{};
		std::string plugin_name{};
	};

	const std::string_view mus_suffix = "_MUS.ini";

	class Utility
	{
	public:
		static std::vector<std::string> Split(std::string s) noexcept;

		static auto ToFormID(const std::string& s) noexcept { return static_cast<RE::TESFormID>(std::stoul(s, nullptr, 16)); }

		static std::vector<RE::BGSMusicTrackFormWrapper*> BuildFormVec(const std::unordered_set<std::string>& tokens) noexcept;

		// does the given _MUS.ini file have the modname.esp_MUS.ini structure to check if modname.esp is loaded?
		static std::string IsIniFileModName(std::string filename);

		// is the given mod installed and loaded?
		static bool IsPluginInstalled(std::string_view name);
	};
}
