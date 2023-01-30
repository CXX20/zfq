#include "../zfq/value.hpp"

namespace {
	using zfq::Typelvl, zfq::Value, zfq::operator""_c;

	static_assert(Typelvl<Value<42>>);

	static_assert(Value<42>::value == 42);
	static_assert(0_c == 0);
	static_assert(1_c == 1);
	static_assert(2_c == 2);
	static_assert(42_c == 42);
	static_assert(123_c == 123);
	static_assert(123'456_c == 123'456);
	static_assert(123'456 == 123'456_c);
	static_assert(std::is_same_v<decltype(42_c), Value<42>>);
	static_assert((1_c + 2_c) * 3_c - 4_c == -(-5_c));
	static_assert(bool{1_c});
	static_assert(unsigned{255_c});
	static_assert(unsigned(-1_c));

	static_assert((2_c * std::integral_constant<int, 3>{}).value == 6);

	using zfq::value;

	static_assert(std::is_same_v<decltype(Value{value<42>}), Value<42>>);
	static_assert(std::is_same_v<
			decltype(Value{std::integral_constant<int, 42>{}}), Value<42>>);

	using zfq::assert_;

	static_assert((assert_(42), true));
	static_assert((assert_(42_c), true));
	static_assert((assert_(std::integral_constant<int, 42>{}), true));
}