#include "../zfq/const.hpp"

namespace {
	using zfq::Const, zfq::operator""_c;

	static_assert(Const<42>::value == 42);
	static_assert(0_c == 0);
	static_assert(1_c == 1);
	static_assert(2_c == 2);
	static_assert(42_c == 42);
	static_assert(123_c == 123);
	static_assert(123'456_c == 123'456);
	static_assert(123'456 == 123'456_c);
	static_assert(std::is_same_v<decltype(42_c), Const<42>>);
	static_assert((1_c + 2_c) * 3_c - 4_c == -(-5_c));
	static_assert(bool{1_c});
	static_assert(unsigned{255_c});
	static_assert(unsigned(-1_c));

	static_assert((2_c * std::integral_constant<int, 3>{}).value == 6);

	using zfq::const_;

	static_assert(std::is_same_v<decltype(Const{const_<42>}), Const<42>>);
	static_assert(std::is_same_v<
			decltype(Const{std::integral_constant<int, 42>{}}), Const<42>>);
}