#include "Internal/LoadEditorIDs.hpp"

namespace Internal::LoadEditorIDs
{
	static constexpr auto RESERVED_SIZE = 0x20000ui32;

	static auto EditorIDs = std::unordered_map<RE::TESFormID, RE::BSFixedString>();

	void Install() noexcept
	{
		if (REX::W32::GetModuleHandleW(L"PapyrusCommonLibrary.dll") || REX::W32::GetModuleHandleW(L"BakaFramework.dll")) {
			logger::info(FMT_STRING("An EditorID-loading mod was already installed, aborting EditorID loading."));
			return;
		}

		EditorIDs.reserve(RESERVED_SIZE);

		// we only need to load editorids for a small amount of records
		Patch<RE::BGSLocation>::Install();
		Patch<RE::BGSMusicType>::Install();
		Patch<RE::BGSMusicTrackFormWrapper>::Install();
	}

	const char* GetFormEditorIDImpl(RE::TESForm* a_this)
	{
		const auto&& [_, mutex] = RE::TESForm::GetAllFormsByEditorID();
		const auto lock = RE::BSAutoReadLock{ mutex.get() };

		const auto formIt = EditorIDs.find(a_this->GetFormID());
		return formIt != EditorIDs.end() ? formIt->second.data() : "";
	}

	bool SetFormEditorIDImpl(RE::TESForm* a_this, const char* a_editorID)
	{
		if (!a_editorID || !a_editorID[0]) {
			return false;
		}

		const auto formID = a_this->GetFormID();
		const auto editorID = RE::BSFixedString(a_editorID);

		if (editorID.empty()) {
			return false;
		}

		const auto&& [map, mutex] = RE::TESForm::GetAllFormsByEditorID();
		const auto lock = RE::BSAutoWriteLock{ mutex.get() };

		const auto&& [it, hasInserted] = map->emplace(editorID, a_this);
		EditorIDs[formID] = editorID;

		if (hasInserted) {
			return true;
		}

		const auto ownerFormID = it->second->GetFormID();
		if (formID == ownerFormID) {
			return false;
		}

		logger::warn("Duplicate EditorID - Sender: {:08X}, Owner: {:08X}, ID: \"{}\""sv, formID, ownerFormID, editorID);
		return false;
	}
}
