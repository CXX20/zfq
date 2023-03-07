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
	static_assert(zfq::size(tuple3).value == 3);
	static_assert(Tuple3::typeof(std::integral_constant<int, 0>{}) == type<int>);
	static_assert(Tuple3::typeof(0_c) == type<int>);
	static_assert(Tuple3::typeof(1_c) == type<double>);
	static_assert(Tuple3::typeof(2_c) == type<unsigned>);

	static_assert(std::is_same_v<
			zfq::Decltype<Tuple<int const&>::typeof(0_c)>, int const&>);

	static_assert(Tuple{} == Tuple{});
	static_assert(Tuple{1, 2} == Tuple{1, 2});
	static_assert(Tuple{1, 2} != Tuple{3, 4});
	static_assert((Tuple{1, 2} != Tuple{3}).value);
	static_assert((Tuple{1_c, 2_c} == Tuple{1_c, 2_c}).value);
	static_assert((Tuple{1_c, 2_c} != Tuple{1_c, 3_c}).value);

	template<typename T, typename L, typename R> concept Correct = requires(T t) {
		{ t[0_c] } -> std::same_as<L>;
		{ std::move(t)[0_c] } -> std::same_as<R>;
	};
	static_assert(Correct<Tuple<int>, int&, int&&>);
	static_assert(Correct<Tuple<int> const, int const&, int const&&>);
	static_assert(Correct<Tuple<int const>, int const&, int const&&>);
	static_assert(Correct<Tuple<int const> const, int const&, int const&&>);
	static_assert(Correct<Tuple<int&>, int&, int&>);
	static_assert(Correct<Tuple<int&> const, int&, int&>);
	static_assert(Correct<Tuple<int const&>, int const&, int const&>);
	static_assert(Correct<Tuple<int const&> const, int const&, int const&>);
	static_assert(Correct<Tuple<int&&>, int&, int&&>);
	static_assert(Correct<Tuple<int&&> const, int&, int&&>);
	static_assert(Correct<Tuple<int const&&>, int const&, int const&&>);
	static_assert(Correct<Tuple<int const&&> const, int const&, int const&&>);

	using zfq::TupleLike;

	struct Tuple0 { constexpr auto size() { return 0_c; } };
	static_assert(TupleLike<Tuple0>);
	static_assert(TupleLike<Tuple3>);
	static_assert(!TupleLike<int>);
	static_assert(!TupleLike<zfq::Const<42>>);
	
	static_assert([t = Tuple{1, 2., 3u}] {
		auto& [_1, _2, _3] = t;
		return _1 == 1 && _2 == 2. && _3 == 3u;
	}());

	namespace my {
		struct Wrapper: Tuple3 {};
		static_assert([t = Wrapper{1, 2., 3u}] {
			zfq::expand(t) | [](int, double, unsigned) {};
			t | zfq::expand | [](int, double, unsigned) {};
			auto [_1, _2, _3] = t;
			return
				zfq::size(t).value == 3 && t == t && _1 == 1 && _2 == 2. && _3 == 3u;
		}());
	}
}
namespace { namespace ext {
	struct Tag {} constexpr tag;
	template<typename T> constexpr auto adl_tag(T const&) { return tag; }
	template<typename...> struct Conflict
	{ constexpr auto operator==(Conflict const&) const { return true; } };
	template<auto i, typename T> constexpr auto get(T&&) { return i; }
	static_assert(get<0>(Conflict<zfq::Tuple<>>{}) == 0);
	static_assert(get<0>(zfq::Tuple<Conflict<>>{}) == Conflict{});
} }