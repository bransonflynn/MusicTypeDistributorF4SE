#pragma once

#include "RE/Scaleform/Kernel/SF_Stats.hpp"

namespace RE::Scaleform
{
	struct AllocInfo;

	struct AllocInfo
	{
	public:
		~AllocInfo() noexcept {} // NOLINT(modernize-use-equals-default)

		// members
		std::uint32_t statID; // 00
	};
	static_assert(sizeof(AllocInfo) == 0x4);
}
