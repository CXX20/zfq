#include "../zfq/const.hpp"
#include <type_traits>

namespace {
	using zfq::Const;

	static_assert(Const<42>::value == 42);
	static_assert(decltype(Const{std::integral_constant<int, 7>{}})::value == 7);

	using zfq::operator""_c;

	static_assert(decltype(0_c)::value == 0);
	static_assert(decltype(1_c)::value == 1);
	static_assert(decltype(2_c)::value == 2);
	static_assert(decltype(10_c)::value == 10);
	static_assert(decltype(12_c)::value == 12);
	static_assert(decltype(123_c)::value == 123);
	static_assert(decltype(123'456_c)::value == 123'456);
	static_assert(decltype(0b100101_c)::value == 0b100101);
	static_assert(decltype(0B100101_c)::value == 0B100101);
	static_assert(decltype(01234567_c)::value == 01234567);
	static_assert(decltype(0x123'ABC_c)::value == 0x123'ABC);
	static_assert(decltype(0xABC'DEF_c)::value == 0xABC'DEF);

	static_assert((1_c + 2_c) * 3_c - 4_c == -(-5_c));
	static_assert((2_c * std::integral_constant<int, 3>{}).value == 6);

	using zfq::const_;
	
	static_assert([](int i) { return i; }(42_c) == 42);
	static_assert([](int i) { return i; }(const_<42LL>) == 42);
	static_assert(int{42_c} == 42);
	static_assert(int{const_<42LL>} == 42);

	static_assert([](unsigned u) { return u; }(42_c) == 42);
	static_assert([](unsigned u) { return u; }(const_<42LL>) == 42);
	static_assert(unsigned{42_c} == 42);
	static_assert(unsigned{const_<42LL>} == 42);
	static_assert(unsigned(-1_c) == -1);

	static_assert([](char c) { return c; }(42_c) == 42);
	static_assert([](char c) { return c; }(const_<42LL>) == 42);
	static_assert(char{42_c} == 42);
	static_assert(char{const_<42LL>} == 42);

	static_assert([](bool b) { return b; }(1_c));
	static_assert([](bool b) { return b; }(42_c));
	static_assert(bool{1_c});
	static_assert(bool(42_c));
	static_assert(42_c ? true : false);

	struct Src {} constexpr src;
	struct Implicit { constexpr Implicit(Src) {} };
	Implicit constexpr implicit = const_<src>;
	struct Explicit { constexpr explicit Explicit(Src) {} };
	Explicit constexpr explicit_{const_<src>};
	static_assert(!std::is_convertible_v<Const<src>, Explicit>);
}