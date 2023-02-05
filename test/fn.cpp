#include "../zfq/fn.hpp"

namespace {
	using zfq::Pipe;

	Pipe<[](auto t) { return t * t; }> constexpr sqr;
	static_assert(sqr(4) == 16);
	static_assert((4 | sqr()) == 16);
	static_assert((4 | sqr) == 16);
	static_assert(!std::is_invocable_v<decltype(sqr()), int>);
	static_assert(!std::is_move_assignable_v<decltype(sqr())>);

	Pipe<[](auto t, auto u) { return t - u; }> constexpr sub;
	static_assert(sub(16, 4) == 12);
	static_assert((16 | sub(4)) == 12);
	static_assert(!std::is_invocable_v<decltype(sub(4)), int>);
	static_assert(!std::is_move_assignable_v<decltype(sub(4))>);

	Pipe<[](auto) {}> constexpr ignore;
	static_assert(std::is_same_v<decltype(ignore(42)), void>);
	static_assert(std::is_same_v<decltype(42 | ignore()), void>);
	static_assert(std::is_same_v<decltype(42 | ignore), void>);

	Pipe<[](auto const& t) -> auto& { return t; }> constexpr cref;
	static_assert(std::is_same_v<decltype(cref(42)), int const&>);
	static_assert(std::is_same_v<decltype(42 | cref()), int const&>);
	static_assert(std::is_same_v<decltype(42 | cref), int const&>);

	namespace lib1 {
		inline namespace cpo { Pipe<[](auto t) { return foo(t); }> constexpr foo; }
		struct Foo {};
		template<typename T> constexpr auto foo(T) { return 1; }
	}
	namespace lib2 {
		inline namespace cpo { Pipe<[](auto t) { return foo(t); }> constexpr foo; }
		struct Foo {};
		template<typename T> constexpr auto foo(T) { return 2; }
		static_assert(cpo::foo(lib1::Foo{}) == 1);
		static_assert(cpo::foo(Foo{}) == 2);
	}
	static_assert(foo(lib1::Foo{}) == 1);
	static_assert(foo(lib2::Foo{}) == 2);
	static_assert(lib1::cpo::foo(lib1::Foo{}) == 1);
	static_assert(lib1::cpo::foo(lib2::Foo{}) == 2);
	static_assert(lib2::cpo::foo(lib1::Foo{}) == 1);
	static_assert(lib2::cpo::foo(lib2::Foo{}) == 2);

	static_assert([]<typename T = zfq::Value<42>>(T random_adl_pipe = {})
	{ return !requires { random_adl_pipe | [](auto&&) {}; }; }());
}

namespace zfq { template<typename T> static constexpr void adl_barriered(T) {} }

namespace {
	template<typename T> constexpr auto adl_barriered(T) { return true; }

	static_assert(adl_barriered(sqr));

	using zfq::Overload;

	Overload constexpr set{[](int) { return 1; }, [](int*) { return 2; }};
	static_assert(set(42) == 1);
	static_assert(set(nullptr) == 2);
	static_assert(adl_barriered(set));
}