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
	static_assert(!requires_(sfinae, [](auto&& fn) -> decltype(fn(nullptr)) {}));

	Pipe constexpr variadic = [](auto&&... ts) { return (0_c + ... + ts); };
	static_assert(variadic().value == 0);
	static_assert(variadic(42_c).value == 42);
	static_assert(variadic(4_c, 2_c).value == 6);
	static_assert((42_c | variadic).value == 42);

	static_assert(!requires_(42_c, [](auto random_adl_pipe)
	-> decltype(random_adl_pipe | [](auto&&) {}) {}));

	using zfq::Overload;

	Overload constexpr set{[](int) { return 1; }, [](int*) { return 2; }};
	static_assert(set(42) == 1);
	static_assert(set(nullptr) == 2);
}

namespace zfq {
	Cpo<[](auto&& t) -> decltype(customize(adl::dispatch(t), t))
	{ return customize(adl::dispatch(t), t); }, 1> constexpr customize;
	namespace adl {
		template<typename T> static constexpr auto customize(Generic, T const&)
		{ return 0_c; }
	}
}
namespace { namespace ext {
	struct Unaware {};
	constexpr auto customize(zfq::adl::Specific<zfq::adl::Generic>, Unaware)
	{ return 42_c; }
} }
namespace { namespace mix {
	struct Unaware {};
	struct Untouched {};
	namespace adl {
		struct Tag {};
		constexpr auto customize(Tag, Unaware) { return 12_c; }
	}
	template<typename T> constexpr adl::Tag adl_tag(T const&) { return {}; }
} }
namespace { namespace lib1 {
	template<typename...> struct Conflict {};
	namespace adl {
		struct Tag {};
		template<typename T> constexpr auto customize(Tag, T const&) { return 1_c; }
	}
	template<typename T> constexpr adl::Tag adl_tag(T const&) { return {}; }
} }
namespace { namespace lib2 {
	template<typename...> struct Conflict {};
	namespace adl {
		struct Tag {};
		template<typename T> constexpr auto customize(Tag, T const&) { return 2_c; }
	}
	template<typename T> constexpr adl::Tag adl_tag(T const&) { return {}; }
} }
namespace {
	using zfq::adl::tag_for;

	template<auto t, typename T> auto constexpr tagged =
		std::is_same_v<decltype(tag_for(t)), T>;
	auto constexpr tagless = [](auto&& t) { return !requires { tag_for(t); }; };

	static_assert(tagged<42_c, zfq::adl::Generic>);
	static_assert(tagged<mix::Unaware{}, mix::adl::Tag>);
	static_assert(tagged<mix::Untouched{}, mix::adl::Tag>);
	static_assert(tagged<lib1::Conflict<mix::Unaware>{}, lib1::adl::Tag>);
	static_assert(tagged<lib2::Conflict<mix::Unaware>{}, lib2::adl::Tag>);
	static_assert(tagless(42));
	static_assert(tagless(ext::Unaware{}));

	using Conflict1 = lib1::Conflict<ext::Unaware, lib2::Conflict<>>;
	using Conflict2 = lib2::Conflict<ext::Unaware, lib1::Conflict<>>;
	static_assert(zfq::customize(42).value == 0);
	static_assert(zfq::customize(ext::Unaware{}).value == 42);
	static_assert(zfq::customize(mix::Unaware{}).value == 12);
	static_assert(zfq::customize(mix::Untouched{}).value == 0);
	static_assert(zfq::customize(Conflict1{}).value == 1);
	static_assert(zfq::customize(Conflict2{}).value == 2);
}

namespace zfq {
	Cpo<[](auto&& t) -> decltype(override_(adl::dispatch(t), t))
	{ return override_(adl::dispatch(t), t); }, 1> constexpr override_;
	Cpo<[](auto&& t) -> decltype(keep(adl::dispatch(t), t))
	{ return keep(adl::dispatch(t), t); }, 1> constexpr keep;
	namespace adl {
		template<typename T> static constexpr auto override_(Generic, T&&)
		requires std::is_fundamental_v<std::remove_reference_t<T>>
		{ return false_; }
		template<typename T> static constexpr auto keep(Generic, T&&)
		{ return true_; }
	}
}
namespace { namespace mixx {
	struct Base {};
	struct Derived: Base {};
	namespace adl {
		struct Tag {};
		template<typename... As> constexpr auto override_(Tag, Base const&, As&&...)
		{ return zfq::true_; }
	}
	template<typename T> constexpr adl::Tag adl_tag(T const&) { return {}; }
} }
namespace {
	static_assert(!zfq::override_(42));
	static_assert(zfq::override_(mixx::Derived{}).value);
	static_assert(zfq::keep(mixx::Derived{}).value);
}