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

	static_assert(!requires_(42_c, [](auto random_adl_pipe)
	-> decltype(random_adl_pipe | [](auto&&) {}) {}));

	using zfq::Overload;

	Overload constexpr set{[](int) { return 1; }, [](int*) { return 2; }};
	static_assert(set(42) == 1);
	static_assert(set(nullptr) == 2);
}
namespace zfq {
	Cpo<[](auto const& t) -> decltype(customize(adl::tag_for(t), t))
	{ return customize(adl::tag_for(t), t); }, 1> constexpr customize;
	namespace adl {
		template<typename T> static constexpr auto customize(adl::Tag, T const&)
		{ return 0; }
	}
}
namespace { namespace ext {
	struct Unaware {};
	constexpr auto customize(zfq::adl::Tag, Unaware) { return 42; }
} }
namespace { namespace lib1 {
	template<typename...> struct Conflict {};
	namespace adl {
		struct Tag {} constexpr tag;
		template<typename T> constexpr auto customize(Tag, T const&) { return 1; }
	}
	template<typename T> constexpr auto adl_tag(T const&) { return adl::tag; }
} }
namespace { namespace lib2 {
	template<typename...> struct Conflict {};
	namespace adl {
		struct Tag {} constexpr tag;
		template<typename T> constexpr auto customize(Tag, T const&) { return 2; }
	}
	template<typename T> constexpr auto adl_tag(T const&) { return adl::tag; }
} }
namespace {
	using Conflict1 = lib1::Conflict<ext::Unaware, lib2::Conflict<>>;
	using Conflict2 = lib2::Conflict<ext::Unaware, lib1::Conflict<>>;
	static_assert(zfq::customize(ext::Unaware{}) == 42);
	static_assert(zfq::customize(Conflict1{}) == 1);
	static_assert(zfq::customize(Conflict2{}) == 2);
}