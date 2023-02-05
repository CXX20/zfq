#ifndef ZFQ_HEADER_VALUE
#define ZFQ_HEADER_VALUE

#include <cassert>
#include <utility>

namespace zfq {
	template<auto t> struct Value {
		static auto constexpr value = t;
		constexpr Value() = default;
		template<typename U, std::enable_if_t<
				t == U::value, int> = 0> constexpr Value(U) {}
		template<auto u = t> constexpr Value<!u> operator!() const { return {}; }
		template<auto u = t> constexpr Value<-u> operator-() const { return {}; }
		template<auto u = t> constexpr Value<~u> operator~() const { return {}; }
		template<typename U> constexpr Value<t == U::value> operator==(U) const
		{ return {}; }
		template<typename U> constexpr Value<t != U::value> operator!=(U) const
		{ return {}; }
		template<typename U> constexpr Value<t < U::value> operator<(U) const
		{ return {}; }
		template<typename U> constexpr Value<(t > U::value)> operator>(U) const
		{ return {}; }
		template<typename U> constexpr Value<t <= U::value> operator<=(U) const
		{ return {}; }
		template<typename U> constexpr Value<t >= U::value> operator>=(U) const
		{ return {}; }
		template<typename U> constexpr Value<t <=> U::value> operator<=>(U) const
		{ return {}; }
		template<typename U> constexpr Value<t + U::value> operator+(U) const
		{ return {}; }
		template<typename U> constexpr Value<t - U::value> operator-(U) const
		{ return {}; }
		template<typename U> constexpr Value<t * U::value> operator*(U) const
		{ return {}; }
		template<typename U> constexpr Value<t / U::value> operator/(U) const
		{ return {}; }
		template<typename U> constexpr Value<t % U::value> operator%(U) const
		{ return {}; }
		template<typename U> constexpr Value<t & U::value> operator&(U) const
		{ return {}; }
		template<typename U> constexpr Value<t | U::value> operator|(U) const
		{ return {}; }
		template<typename U> constexpr Value<t ^ U::value> operator^(U) const
		{ return {}; }
		template<typename U> constexpr Value<t << U::value> operator<<(U) const
		{ return {}; }
		template<typename U> constexpr Value<(t >> U::value)> operator>>(U) const
		{ return {}; }
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
}

#endif