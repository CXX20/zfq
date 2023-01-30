#include "../zfq/tuple.hpp"

namespace {
	using zfq::Tuple, zfq::type, zfq::operator""_c;

	using Tuple3 = decltype(Tuple{1, 2., 3u});
	Tuple constexpr tuple3{1, 2., 3u};

	static_assert(std::is_same_v<decltype(Tuple{}), Tuple<>>);
	static_assert(Tuple{Tuple{}}.typeof(0_c) == type<Tuple<>>);
	static_assert(Tuple{tuple3}.typeof(0_c) == type<Tuple3>);

	static_assert(tuple3[std::integral_constant<int, 0>{}] == 1);
	static_assert(tuple3[0_c] == 1);
	static_assert(tuple3[1_c] == 2.);
	static_assert(tuple3[2_c] == 3u);
	static_assert(size(tuple3).value == 3);
	static_assert(Tuple3::typeof(std::integral_constant<int, 0>{}) == type<int>);
	static_assert(Tuple3::typeof(0_c) == type<int>);
	static_assert(Tuple3::typeof(1_c) == type<double>);
	static_assert(Tuple3::typeof(2_c) == type<unsigned>);
	static_assert(Tuple3::indexof(std::type_identity<int>{}) == 0_c);
	static_assert(Tuple3::indexof(type<int>) == 0_c);
	static_assert(Tuple3::indexof(type<double>) == 1_c);
	static_assert(Tuple3::indexof(type<unsigned>) == 2_c);

	static_assert(std::is_same_v<
			zfq::Decltype<Tuple<type<int const&>>::typeof(0_c)>, int const&>);

	static_assert(Tuple{1, 2} == Tuple{1, 2});
	static_assert(Tuple{1, 2} != Tuple{3, 4});
	static_assert((Tuple{1, 2} != Tuple{3}).value);
	static_assert((Tuple{1_c, 2_c} == Tuple{1_c, 2_c}).value);
	static_assert((Tuple{1_c, 2_c} != Tuple{1_c, 3_c}).value);

	static_assert([t = Tuple{1, 2., 3u}] {
		auto& [_1, _2, _3] = t;
		return _1 == 1 && _2 == 2. && _3 == 3u;
	}());

	namespace my {
		struct Wrapper: Tuple3 {};
		static_assert([t = Wrapper{1, 2., 3u}] {
			apply([](int, double, unsigned) {}, t);
			auto& [_1, _2, _3] = t;
			return size(t) == 3_c && t == t && _1 == 1 && _2 == 2. && _3 == 3u;
		}());
	}
}