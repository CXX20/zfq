#ifndef ZFQ_HEADER_CONST
#define ZFQ_HEADER_CONST

namespace zfq::_const {
	template<typename T> struct Convertible { T t; };

	template<char b, char... cs> inline auto constexpr pp = [] {
		int ret{}, to_int[256];
		for (char i = '0'; i <= '9'; ++i) to_int[i] = i - '0';
		for (char i = 'a'; i <= 'f'; ++i) to_int[i] = i - 'a' + 10;
		for (char i = 'A'; i <= 'F'; ++i) to_int[i] = i - 'A' + 10;
		return (ret, ..., (ret = cs == '\'' ? ret : ret * b + to_int[cs]));
	};
	template<char... cs> inline auto constexpr p = pp<10, cs...>;
	template<char... cs> inline auto constexpr p<'0', cs...> = pp<8, cs...>;
	template<char... cs> inline auto constexpr p<'0', 'b', cs...> = pp<2, cs...>;
	template<char... cs> inline auto constexpr p<'0', 'B', cs...> = pp<2, cs...>;
	template<char... cs> inline auto constexpr p<'0', 'x', cs...> = pp<16, cs...>;
	template<char... cs> inline auto constexpr p<'0', 'X', cs...> = pp<16, cs...>;
}
namespace zfq {
	template<auto t> struct Const {
		static auto constexpr value = t;
		constexpr Const() = default;
		template<typename C> constexpr Const(C) requires (C::value == value) {}
		template<typename U> constexpr operator U() const
		requires requires { _const::Convertible<U>{value}; }
		{ return value; }
		constexpr operator decltype(value)() const { return value; }
		template<auto u = t> constexpr Const<!u> operator!() const { return {}; }
		template<auto u = t> constexpr Const<-u> operator-() const { return {}; }
		template<auto u = t> constexpr Const<~u> operator~() const { return {}; }
		template<typename U> constexpr auto operator==(U) const
		-> Const<value == U::value> { return {}; }
		template<typename U> constexpr auto operator!=(U) const
		-> Const<value != U::value> { return {}; }
		template<typename U> constexpr auto operator<(U) const
		-> Const<value < U::value> { return {}; }
		template<typename U> constexpr auto operator>(U) const
		-> Const<(value > U::value)> { return {}; }
		template<typename U> constexpr auto operator<=(U) const
		-> Const<value <= U::value> { return {}; }
		template<typename U> constexpr auto operator>=(U) const
		-> Const<value >= U::value> { return {}; }
		template<typename U> constexpr auto operator<=>(U) const
		-> Const<value <=> U::value> { return {}; }
		template<typename U> constexpr auto operator+(U) const
		-> Const<value + U::value> { return {}; }
		template<typename U> constexpr auto operator-(U) const
		-> Const<value - U::value> { return {}; }
		template<typename U> constexpr auto operator*(U) const
		-> Const<value * U::value> { return {}; }
		template<typename U> constexpr auto operator/(U) const
		-> Const<value / U::value> { return {}; }
		template<typename U> constexpr auto operator%(U) const
		-> Const<value % U::value> { return {}; }
		template<typename U> constexpr auto operator&(U) const
		-> Const<value & U::value> { return {}; }
		template<typename U> constexpr auto operator|(U) const
		-> Const<value | U::value> { return {}; }
		template<typename U> constexpr auto operator^(U) const
		-> Const<value ^ U::value> { return {}; }
		template<typename U> constexpr auto operator<<(U) const
		-> Const<value << U::value> { return {}; }
		template<typename U> constexpr auto operator>>(U) const
		-> Const<(value >> U::value)> { return {}; }
	};
	template<typename C> Const(C) -> Const<C::value>;
	template<auto t> inline Const<t> constexpr const_;
	inline Const<true> constexpr true_;
	inline Const<false> constexpr false_;
	template<char... cs> constexpr auto operator""_c()
	{ return const_<_const::p<cs...>()>; }
}

#endif