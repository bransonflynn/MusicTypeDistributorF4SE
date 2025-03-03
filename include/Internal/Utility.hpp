#pragma once

namespace Internal
{
	class Utility
	{
	public:
		static auto Split(std::string s) noexcept
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
	};
}
