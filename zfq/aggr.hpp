#ifndef ZFQ_HEADER_AGGR
#define ZFQ_HEADER_AGGR

#include "_gen/aggr.hpp"

namespace zfq::_aggr {
	struct Any { template<typename T> operator T(); };
	template<auto> using AnyT = Any;
	template<typename T, auto... is>
	constexpr auto size(std::index_sequence<is...>) {
		if constexpr (requires { T{AnyT<is>{}...}; }) return const_<sizeof...(is)>;
		else if constexpr (!sizeof...(is)) return const_<std::size_t(-1)>;
		else return _aggr::size<T>(std::make_index_sequence<sizeof...(is) - 1>{});
	}
	template<typename T> constexpr auto size(T const&) {
		auto constexpr n = _aggr::size<T>(std::make_index_sequence<16 + 1>{});
		static_assert(n.value <= 16, "aggregate too large");
		return n;
	}
	template<typename T> concept Alike =
		std::is_aggregate_v<std::remove_reference_t<T>> &&
		!requires { std::tuple_size<std::remove_reference_t<T>>::value; } &&
		requires(T t) { requires !!decltype(_aggr::size(t))::value; };
}
namespace zfq {
	template<typename T> concept AggrLike = TupleLike<T> && _aggr::Alike<T>;
}
namespace zfq::adl {
	template<typename F, AggrLike T>
	constexpr decltype(auto) apply(Generic, F&& fn, T&& t)
	{ return std::forward<T>(t) | zfq::view | expand | fn; }
	template<_aggr::Alike T> constexpr auto size(Generic, T const& t)
	{ return _aggr::size(t); }
	template<AggrLike T> constexpr auto view(Generic, T&& t)
	{ return _aggr::view(std::forward<T>(t), zfq::size(t)); }
}

#endif