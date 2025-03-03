#pragma once
#include <simpleini.h>

namespace Internal
{
	class Parser
	{
	public:
		// gather all inis from dir
		static void ParseINIs(CSimpleIniA& ini) noexcept;

		//
		// static void PrepareDistributionMap() noexcept;
	};
}
