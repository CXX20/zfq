#ifndef ZFQ_HEADER_VALUE
#define ZFQ_HEADER_VALUE

#include <cassert>
#include <type_traits>

namespace zfq {
	template<typename T> concept Typelvl =
		std::is_empty_v<T> && std::is_trivial_v<T>;

	template<auto t> struct Value {
		static auto constexpr value = t;
		constexpr Value() = default;
		template<typename U, std::enable_if_t<
				t == U::value, int> = 0> constexpr Value(U) {}
		template<typename U, decltype(U{value})* = nullptr, std::enable_if_t<
			std::is_convertible_v<U, decltype(t)>, int
		> = 0> constexpr operator U() const { return U{value}; }
		constexpr operator decltype(t)() const { return t; }
	};
	template<typename T> Value(T) -> Value<T::value>;
	template<auto t> inline Value<t> constexpr value;
	template<char... cs> constexpr auto operator""_c() {
		return value<[i = 0, chars = {cs...}]() mutable {
			for (auto c: chars)
				i = c == '\'' ? i : c < '0' || c > '9' ? throw : i * 10 + (c - '0');
			return *chars.begin() == '0' && chars.size() != 1 ? throw : i;
		}()>;
	}
	template<auto t> constexpr Value<!t> operator!(Value<t>) { return {}; }
	template<auto t> constexpr Value<-t> operator-(Value<t>) { return {}; }
	template<auto t> constexpr Value<~t> operator~(Value<t>) { return {}; }
	template<auto t, typename U> constexpr auto operator==(Value<t>, U)
	-> Value<t == U::value> { return {}; }
	template<auto t, typename U> constexpr auto operator!=(Value<t>, U)
	-> Value<t != U::value> { return {}; }
	template<auto t, typename U> constexpr auto operator<(Value<t>, U)
	-> Value<t < U::value> { return {}; }
	template<auto t, typename U> constexpr auto operator>(Value<t>, U)
	-> Value<(t > U::value)> { return {}; }
	template<auto t, typename U> constexpr auto operator<=(Value<t>, U)
	-> Value<t <= U::value> { return {}; }
	template<auto t, typename U> constexpr auto operator>=(Value<t>, U)
	-> Value<t >= U::value> { return {}; }
	template<auto t, typename U> constexpr auto operator<=>(Value<t>, U)
	-> Value<t <=> U::value> { return {}; }
	template<auto t, typename U> constexpr auto operator+(Value<t>, U)
	-> Value<t + U::value> { return {}; }
	template<auto t, typename U> constexpr auto operator-(Value<t>, U)
	-> Value<t - U::value> { return {}; }
	template<auto t, typename U> constexpr auto operator*(Value<t>, U)
	-> Value<t * U::value> { return {}; }
	template<auto t, typename U> constexpr auto operator/(Value<t>, U)
	-> Value<t / U::value> { return {}; }
	template<auto t, typename U> constexpr auto operator%(Value<t>, U)
	-> Value<t % U::value> { return {}; }
	template<auto t, typename U> constexpr auto operator&(Value<t>, U)
	-> Value<t & U::value> { return {}; }
	template<auto t, typename U> constexpr auto operator|(Value<t>, U)
	-> Value<t | U::value> { return {}; }
	template<auto t, typename U> constexpr auto operator^(Value<t>, U)
	-> Value<t ^ U::value> { return {}; }
	template<auto t, typename U> constexpr auto operator<<(Value<t>, U)
	-> Value<t << U::value> { return {}; }
	template<auto t, typename U> constexpr auto operator>>(Value<t>, U)
	-> Value<(t >> U::value)> { return {}; }
	template<typename T> constexpr void assert_(T const& t)
	{ std::is_constant_evaluated() && !t ? throw : assert(t); }
	template<auto t> constexpr void assert_(Value<t>) { static_assert(t); }
}

#endif