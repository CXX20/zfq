#include "../zfq/fn.hpp"

namespace {
	using zfq::requires_;

	static_assert(decltype(requires_(123, [](auto i) -> decltype(~i) {}))::value);
	static_assert(!decltype(requires_(1., [](auto f) -> decltype(~f) {}))::value);
	static_assert(requires_(123, [](auto i) -> decltype(~i) {}).value);
	static_assert(!requires_(1., [](auto f) -> decltype(~f) {}).value);

	using zfq::Cpo, zfq::Pipe, zfq::operator""_c;

	Pipe constexpr sqr = [](auto t) { return t * t; };
	static_assert(sqr(4) == 16);
	static_assert((4 | sqr()) == 16);
	static_assert((4 | sqr) == 16);
	static_assert(!requires_(sqr(), [](auto) {}));
	static_assert(!requires_(sqr(), [](auto&& fn) -> decltype(fn(4)) {}));

	Pipe constexpr sub = [](auto t, auto u) { return t - u; };
	static_assert(sub(16, 4) == 12);
	static_assert((16 | sub(4)) == 12);
	static_assert(!requires_(sub(4), [](auto) {}));
	static_assert(!requires_(sub(4), [](auto&& fn) -> decltype(fn(16)) {}));

	Pipe constexpr ignore = [](auto&&) {};
	static_assert(std::is_same_v<decltype(ignore(42)), void>);
	static_assert(std::is_same_v<decltype(42 | ignore()), void>);
	static_assert(std::is_same_v<decltype(42 | ignore), void>);

	Pipe constexpr cref = [](auto const& t) -> auto& { return t; };
	static_assert(std::is_same_v<decltype(cref(42)), int const&>);
	static_assert(std::is_same_v<decltype(42 | cref()), int const&>);
	static_assert(std::is_same_v<decltype(42 | cref), int const&>);

	Pipe constexpr sfinae{[](auto t) -> decltype(-t) { return -t; }, 1_c};
	static_assert(sfinae(42) == -42);
	static_assert(!requires_(sfinae, [](auto&& fn) -> decltype(fn((void*)0)) {}));

	namespace lib1 {
		namespace cpo { Cpo<[](auto t) { return foo(t); }, 1> constexpr foo; }
		struct Foo {};
		template<typename T> constexpr auto foo(T) { return 1; }
	}
	namespace lib2 {
		namespace cpo { Cpo<[](auto t) { return foo(t); }, 1> constexpr foo; }
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

	static_assert(!requires_(42_c, [](auto random_adl_pipe)
	-> decltype(random_adl_pipe | [](auto&&) {}) {}));
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