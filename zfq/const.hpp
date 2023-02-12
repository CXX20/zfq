#ifndef ZFQ_HEADER_CONST
#define ZFQ_HEADER_CONST

#include <cstdint>
#include <type_traits>

namespace zfq {
	using u8 = std::uint8_t;
	using u64 = std::uint64_t;

	template<auto t> struct Const {
		static auto constexpr value = t;
		constexpr Const() = default;
		template<typename U> constexpr Const(U) requires (t == U::value) {}
		template<typename U> constexpr operator U() const
		requires (requires { U{value}; } && std::is_convertible_v<U, decltype(t)>)
		{ return U{value}; }
		constexpr operator decltype(t)() const { return t; }
		template<auto u = t> constexpr Const<!u> operator!() const { return {}; }
		template<auto u = t> constexpr Const<-u> operator-() const { return {}; }
		template<auto u = t> constexpr Const<~u> operator~() const { return {}; }
		template<typename U> constexpr Const<t == U::value> operator==(U) const
		{ return {}; }
		template<typename U> constexpr Const<t != U::value> operator!=(U) const
		{ return {}; }
		template<typename U> constexpr Const<t < U::value> operator<(U) const
		{ return {}; }
		template<typename U> constexpr Const<(t > U::value)> operator>(U) const
		{ return {}; }
		template<typename U> constexpr Const<t <= U::value> operator<=(U) const
		{ return {}; }
		template<typename U> constexpr Const<t >= U::value> operator>=(U) const
		{ return {}; }
		template<typename U> constexpr Const<t <=> U::value> operator<=>(U) const
		{ return {}; }
		template<typename U> constexpr Const<t + U::value> operator+(U) const
		{ return {}; }
		template<typename U> constexpr Const<t - U::value> operator-(U) const
		{ return {}; }
		template<typename U> constexpr Const<t * U::value> operator*(U) const
		{ return {}; }
		template<typename U> constexpr Const<t / U::value> operator/(U) const
		{ return {}; }
		template<typename U> constexpr Const<t % U::value> operator%(U) const
		{ return {}; }
		template<typename U> constexpr Const<t & U::value> operator&(U) const
		{ return {}; }
		template<typename U> constexpr Const<t | U::value> operator|(U) const
		{ return {}; }
		template<typename U> constexpr Const<t ^ U::value> operator^(U) const
		{ return {}; }
		template<typename U> constexpr Const<t << U::value> operator<<(U) const
		{ return {}; }
		template<typename U> constexpr Const<(t >> U::value)> operator>>(U) const
		{ return {}; }
	};
	template<typename U> Const(U) -> Const<U::value>;
	template<auto t> inline Const<t> constexpr const_;
	template<char... cs> constexpr auto operator""_c() {
		return const_<[i = 0, chars = {cs...}]() mutable {
			for (auto c: chars)
				i = c == '\'' ? i : c < '0' || c > '9' ? throw : i * 10 + (c - '0');
			return *chars.begin() == '0' && chars.size() != 1 ? throw : i;
		}()>;
	}
	template<u8 i> inline Const<i> constexpr u8_;
	template<u64 i> inline Const<i> constexpr u64_;
	inline Const<true> constexpr true_;
	inline Const<false> constexpr false_;
	inline auto constexpr sizeof_pack = []<typename... Ts>(Ts&&...)
	{ return u64_<sizeof...(Ts)>; };
}

#endif