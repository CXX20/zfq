#ifndef ZFQ_HEADER_AGGREGATE
#define ZFQ_HEADER_AGGREGATE

#include "tuple.hpp"

namespace zfq::_aggr {
	struct Any { template<typename T> operator T(); };
	template<auto> using AnyT = Any;

	template<typename T, auto... is>
	constexpr auto size(std::index_sequence<is...>) {
		if constexpr (requires { T{AnyT<is>{}...}; }) return const_<sizeof...(is)>;
		else if constexpr (!sizeof...(is)) return const_<std::size_t(-1)>;
		else return _aggr::size<T>(std::make_index_sequence<sizeof...(is) - 1>{});
	}
	
	template<typename... Es, typename C>
	constexpr auto view_impl(Type<C>, Es&... es)
	{ return Tuple<Es&&...>{std::forward<Es>(es)...}; }
	template<typename... Es, typename C>
	constexpr auto view_impl(Type<C&>, Es&... es)
	{ return Tuple<Es&...>{es...}; }

	template<typename T> constexpr auto view(T&&, Const<0>) { return Tuple{}; }
	template<typename T> constexpr auto view(T&& aggregate, Const<1>) {
		auto& [_0] = aggregate;
		return _aggr::view_impl<decltype(_0)>(type<T>, _0);
	}
	template<typename T> constexpr auto view(T&& aggregate, Const<2>) {
		auto& [_0, _1] = aggregate;
		return _aggr::view_impl<decltype(_0), decltype(_1)>(type<T>, _0, _1);
	}
	template<typename T> constexpr auto view(T&& aggregate, Const<3>) {
		auto& [_0, _1, _2] = aggregate;
		return _aggr::view_impl<decltype(_0), decltype(_1), decltype(_2)>(type<T>, _0, _1, _2);
	}
	template<typename T> constexpr auto view(T&& aggregate, Const<4>) {
		auto& [_0, _1, _2, _3] = aggregate;
		return _aggr::view_impl<decltype(_0), decltype(_1), decltype(_2), decltype(_3)>(type<T>, _0, _1, _2, _3);
	}
	template<typename T> constexpr auto view(T&& aggregate, Const<5>) {
		auto& [_0, _1, _2, _3, _4] = aggregate;
		return _aggr::view_impl<decltype(_0), decltype(_1), decltype(_2), decltype(_3), decltype(_4)>(type<T>, _0, _1, _2, _3, _4);
	}
	template<typename T> constexpr auto view(T&& aggregate, Const<6>) {
		auto& [_0, _1, _2, _3, _4, _5] = aggregate;
		return _aggr::view_impl<decltype(_0), decltype(_1), decltype(_2), decltype(_3), decltype(_4), decltype(_5)>(type<T>, _0, _1, _2, _3, _4, _5);
	}
	template<typename T> constexpr auto view(T&& aggregate, Const<7>) {
		auto& [_0, _1, _2, _3, _4, _5, _6] = aggregate;
		return _aggr::view_impl<decltype(_0), decltype(_1), decltype(_2), decltype(_3), decltype(_4), decltype(_5), decltype(_6)>(type<T>, _0, _1, _2, _3, _4, _5, _6);
	}
	template<typename T> constexpr auto view(T&& aggregate, Const<8>) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7] = aggregate;
		return _aggr::view_impl<decltype(_0), decltype(_1), decltype(_2), decltype(_3), decltype(_4), decltype(_5), decltype(_6), decltype(_7)>(type<T>, _0, _1, _2, _3, _4, _5, _6, _7);
	}
	template<typename T> constexpr auto view(T&& aggregate, Const<9>) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8] = aggregate;
		return _aggr::view_impl<decltype(_0), decltype(_1), decltype(_2), decltype(_3), decltype(_4), decltype(_5), decltype(_6), decltype(_7), decltype(_8)>(type<T>, _0, _1, _2, _3, _4, _5, _6, _7, _8);
	}
	template<typename T> constexpr auto view(T&& aggregate, Const<10>) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9] = aggregate;
		return _aggr::view_impl<decltype(_0), decltype(_1), decltype(_2), decltype(_3), decltype(_4), decltype(_5), decltype(_6), decltype(_7), decltype(_8), decltype(_9)>(type<T>, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9);
	}
	template<typename T> constexpr auto view(T&& aggregate, Const<11>) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10] = aggregate;
		return _aggr::view_impl<decltype(_0), decltype(_1), decltype(_2), decltype(_3), decltype(_4), decltype(_5), decltype(_6), decltype(_7), decltype(_8), decltype(_9), decltype(_10)>(type<T>, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10);
	}
	template<typename T> constexpr auto view(T&& aggregate, Const<12>) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11] = aggregate;
		return _aggr::view_impl<decltype(_0), decltype(_1), decltype(_2), decltype(_3), decltype(_4), decltype(_5), decltype(_6), decltype(_7), decltype(_8), decltype(_9), decltype(_10), decltype(_11)>(type<T>, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11);
	}
	template<typename T> constexpr auto view(T&& aggregate, Const<13>) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12] = aggregate;
		return _aggr::view_impl<decltype(_0), decltype(_1), decltype(_2), decltype(_3), decltype(_4), decltype(_5), decltype(_6), decltype(_7), decltype(_8), decltype(_9), decltype(_10), decltype(_11), decltype(_12)>(type<T>, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12);
	}
	template<typename T> constexpr auto view(T&& aggregate, Const<14>) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13] = aggregate;
		return _aggr::view_impl<decltype(_0), decltype(_1), decltype(_2), decltype(_3), decltype(_4), decltype(_5), decltype(_6), decltype(_7), decltype(_8), decltype(_9), decltype(_10), decltype(_11), decltype(_12), decltype(_13)>(type<T>, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13);
	}
	template<typename T> constexpr auto view(T&& aggregate, Const<15>) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14] = aggregate;
		return _aggr::view_impl<decltype(_0), decltype(_1), decltype(_2), decltype(_3), decltype(_4), decltype(_5), decltype(_6), decltype(_7), decltype(_8), decltype(_9), decltype(_10), decltype(_11), decltype(_12), decltype(_13), decltype(_14)>(type<T>, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14);
	}
	template<typename T> constexpr auto view(T&& aggregate, Const<16>) {
		auto& [_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15] = aggregate;
		return _aggr::view_impl<decltype(_0), decltype(_1), decltype(_2), decltype(_3), decltype(_4), decltype(_5), decltype(_6), decltype(_7), decltype(_8), decltype(_9), decltype(_10), decltype(_11), decltype(_12), decltype(_13), decltype(_14), decltype(_15)>(type<T>, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15);
	}
}
namespace zfq {
	template<typename T> concept Aggregatish =
		!Tuplish<T> && std::is_aggregate_v<std::remove_reference_t<T>>;
}
namespace zfq::adl {
	template<typename F, Aggregatish T>
	constexpr decltype(auto) apply(Generic, F&& fn, T&& t)
	{ return zfq::apply(fn, zfq::view(std::forward<T>(t))); }
	template<Aggregatish T> constexpr auto size(Generic, T const&) {
		auto size = _aggr::size<T>(std::make_index_sequence<16 + 1>{});
		static_assert(decltype(size)::value <= 16, "aggregate too large");
		return size;
	}
	template<Aggregatish T> constexpr auto view(Generic, T&& t)
	{ return _aggr::view(std::forward<T>(t), zfq::size(t)); }
}

#endif