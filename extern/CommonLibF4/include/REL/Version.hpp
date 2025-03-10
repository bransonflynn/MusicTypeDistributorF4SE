#pragma once

namespace REL
{
	class Version
	{
	public:
		using value_type = std::uint16_t;
		using reference = value_type&;
		using const_reference = const value_type&;

		constexpr Version() noexcept = default;

		explicit constexpr Version(std::array<value_type, 4> a_version) noexcept :
			_impl(a_version)
		{
		}

		constexpr Version(value_type a_v1, value_type a_v2 = 0, value_type a_v3 = 0, value_type a_v4 = 0) noexcept :
			_impl{ a_v1, a_v2, a_v3, a_v4 }
		{
		}

		explicit constexpr Version(std::string_view a_version)
		{
			std::array<value_type, 4> powers{ 1, 1, 1, 1 };
			std::size_t position = 0;
			for (std::size_t i = 0; i < a_version.size(); ++i) {
				if (a_version[i] == '.') {
					if (++position == powers.size()) {
						throw std::invalid_argument("Too many parts in version number.");
					}
				}
				else {
					powers[position] *= 10;
				}
			}
			position = 0;
			for (std::size_t i = 0; i < a_version.size(); ++i) {
				if (a_version[i] == '.') {
					++position;
				}
				else if (a_version[i] < '0' || a_version[i] > '9') {
					throw std::invalid_argument("Invalid character in version number.");
				}
				else {
					powers[position] /= 10;
					_impl[position] += static_cast<value_type>((a_version[i] - '0') * powers[position]);
				}
			}
		}

		[[nodiscard]] constexpr reference operator[](std::size_t a_index) noexcept { return _impl[a_index]; }
		[[nodiscard]] constexpr const_reference operator[](std::size_t a_index) const noexcept { return _impl[a_index]; }

		[[nodiscard]] constexpr decltype(auto) begin() const noexcept { return _impl.begin(); }
		[[nodiscard]] constexpr decltype(auto) cbegin() const noexcept { return _impl.cbegin(); }
		[[nodiscard]] constexpr decltype(auto) end() const noexcept { return _impl.end(); }
		[[nodiscard]] constexpr decltype(auto) cend() const noexcept { return _impl.cend(); }

		[[nodiscard]] std::strong_ordering constexpr compare(const Version& a_rhs) const noexcept
		{
			for (std::size_t i = 0; i < _impl.size(); ++i) {
				if ((*this)[i] != a_rhs[i]) {
					return (*this)[i] < a_rhs[i] ? std::strong_ordering::less : std::strong_ordering::greater;
				}
			}
			return std::strong_ordering::equal;
		}

		[[nodiscard]] constexpr std::uint32_t pack() const noexcept
		{
			return static_cast<std::uint32_t>(
				(_impl[0] & 0x0FF) << 24u |
				(_impl[1] & 0x0FF) << 16u |
				(_impl[2] & 0xFFF) << 4u |
				(_impl[3] & 0x00F) << 0u);
		}

		[[nodiscard]] constexpr value_type major() const noexcept { return _impl[0]; }
		[[nodiscard]] constexpr value_type minor() const noexcept { return _impl[1]; }
		[[nodiscard]] constexpr value_type patch() const noexcept { return _impl[2]; }
		[[nodiscard]] constexpr value_type build() const noexcept { return _impl[3]; }

		[[nodiscard]] constexpr std::string string(const std::string_view a_separator = "."sv) const
		{
			std::string result;
			for (auto&& ver : _impl) {
				result += std::to_string(ver);
				result.append(a_separator.data(), a_separator.size());
			}
			result.erase(result.size() - a_separator.size(), a_separator.size());
			return result;
		}

		[[nodiscard]] constexpr std::wstring wstring(const std::wstring_view a_separator = L"."sv) const
		{
			std::wstring result;
			for (auto&& ver : _impl) {
				result += std::to_wstring(ver);
				result.append(a_separator.data(), a_separator.size());
			}
			result.erase(result.size() - a_separator.size(), a_separator.size());
			return result;
		}

		[[nodiscard]] static constexpr Version unpack(const std::uint32_t a_packedVersion) noexcept
		{
			return Version{
				static_cast<value_type>((a_packedVersion >> 24) & 0x0FF),
				static_cast<value_type>((a_packedVersion >> 16) & 0x0FF),
				static_cast<value_type>((a_packedVersion >> 4) & 0xFFF),
				static_cast<value_type>(a_packedVersion & 0x0F)
			};
		}

		[[nodiscard]] friend constexpr bool operator==(const Version& a_lhs, const Version& a_rhs) noexcept
		{
			return a_lhs.compare(a_rhs) == 0;
		}

		[[nodiscard]] friend constexpr std::strong_ordering operator<=>(const Version& a_lhs, const Version& a_rhs) noexcept
		{
			return a_lhs.compare(a_rhs);
		}

	private:
		std::array<value_type, 4> _impl{ 0, 0, 0, 0 };
	};

	[[nodiscard]] std::optional<Version> GetFileVersion(std::string_view a_filename);
	[[nodiscard]] std::optional<Version> GetFileVersion(std::wstring_view a_filename);
}

namespace std
{
	template <>
	struct formatter<REL::Version>
		: public formatter<string>
	{
		constexpr auto parse(format_parse_context& a_ctx) const
		{
			return a_ctx.begin();
		}

		template <class FormatContext>
		constexpr auto format(const REL::Version& a_version, FormatContext& a_ctx) const
		{
			return formatter<string>::format(a_version.string(), a_ctx);
		}
	};
}

#ifdef FMT_VERSION
namespace fmt
{
	template <>
	struct formatter<REL::Version>
		: public formatter<std::string>
	{
		constexpr auto parse(format_parse_context& a_ctx) const
		{
			return a_ctx.begin();
		}

		template <class FormatContext>
		constexpr auto format(const REL::Version& a_version, FormatContext& a_ctx) const
		{
			return formatter<std::string>::format(a_version.string(), a_ctx);
		}
	};
}
#endif
