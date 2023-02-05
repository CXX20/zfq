#ifndef ZFQ_HEADER_TUPLE
#define ZFQ_HEADER_TUPLE

#include "fn.hpp"

namespace zfq::cpo {
	inline Pipe<[](auto const& t) { return size(t); }> constexpr size;
	inline Pipe<[]<typename F, typename T>(F&& fn, T&& tuple) -> decltype(auto)
	{ return apply(fn, std::forward<T>(tuple)); }> constexpr apply;
}
namespace zfq::_tuple {
	template<typename T, auto i> struct Elem {
		[[no_unique_address]] T _t;
		static constexpr auto typeof(Value<i>) { return type<T>; }
		constexpr auto& operator[](Value<i>) const& { return _t; }
		constexpr auto& operator[](Value<i>) & { return _t; }
		constexpr auto&& operator[](Value<i>) && { return std::forward<T>(_t); }
		constexpr auto& operator[](Type<T>) const& { return _t; }
		constexpr auto& operator[](Type<T>) & { return _t; }
		constexpr auto&& operator[](Type<T>) && { return std::forward<T>(_t); }
	};

	template<typename...> struct Tuple;
	template<auto... is, typename... Ts>
	struct Tuple<std::index_sequence<is...>, Ts...>: Elem<Ts, is>...
	{ using Elem<Ts, is>::typeof..., Elem<Ts, is>::operator[]...; };
}
namespace zfq {
	template<auto... ts> struct Tuple // HACK for IDE (should be <Type... ts>)
	: _tuple::Tuple<std::make_index_sequence<sizeof...(ts)>, Decltype<ts>...> {
		friend constexpr Value<sizeof...(ts)> size(Tuple const&) { return {}; }
		template<typename U> constexpr auto operator==(U const& u) const {
			decltype(cpo::size(*this)) constexpr n;
			decltype(cpo::size(u)) constexpr m;
			if constexpr (auto apply = cpo::apply; n != m) return value<false>;
			else return apply([&](auto&&... es) {
				return apply([&](auto&&... ds) { return !!(... & (es == ds)); }, u);
			}, *this);
		}
		template<typename U> constexpr auto operator!=(U const& u) const
		{ return !(*this == u); }
	};
	template<typename... Ts> Tuple(Ts...) -> Tuple<type<Ts>...>;
	template<auto... ts> Tuple(Tuple<ts...>) -> Tuple<type<Tuple<ts...>>>;
	
	template<typename T> auto _zfq_tuplish(T const& t)
	-> std::enable_if_t<std::is_empty_v<decltype(size(t))>>;
	template<auto i, typename T> constexpr decltype(auto) get(T&& tuple)
	{ return std::forward<T>(tuple)[value<i>]; }
	template<typename F, typename T>
	constexpr decltype(auto) apply(F&& fn, T&& tuple) {
		return [&]<auto... is>(std::index_sequence<is...>) -> decltype(auto) {
			return fn(std::forward<T>(tuple)[value<is>]...);
		}(std::make_index_sequence<decltype(cpo::size(tuple)){}>{});
	}
}
template<typename T> requires requires(T t) { _zfq_tuplish(t); }
struct std::tuple_size<T>
{ static decltype(size(std::declval<T>())) constexpr value{}; };
template<auto i, typename T> requires requires(T t) { _zfq_tuplish(t); }
struct std::tuple_element<i, T>
{ using type = zfq::Decltype<T::typeof(zfq::value<i>)>; };

#endif