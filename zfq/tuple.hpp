#ifndef ZFQ_HEADER_TUPLE
#define ZFQ_HEADER_TUPLE

#include <utility>
#include "type.hpp"

namespace zfq {
	template<typename T> concept Tuplish =
		requires(T t) { { size(t) } -> Typelvl; };

	template<typename F, typename T>
	constexpr decltype(auto) apply(F&& fn, T&& tuple) {
		return [&]<auto... is>(std::index_sequence<is...>) -> decltype(auto) {
			return fn(std::forward<T>(tuple)[value<is>]...);
		}(std::make_index_sequence<decltype(size(tuple)){}>{});
	}
	template<Tuplish T, Tuplish U>
	constexpr auto operator==(T const& t, U const& u) {
		decltype(size(t)) constexpr n;
		decltype(size(u)) constexpr m;
		if constexpr (n != m) return value<false>;
		else return apply([&](auto&&... es) {
			return apply([&](auto&&... ds) { return !!(... & (es == ds)); }, u);
		}, t);
	}
	template<Tuplish T, Tuplish U>
	constexpr auto operator!=(T const& t, U const& u)
	{ return !(t == u); } // TODO generic
}
namespace zfq::_tuple {
	template<typename T, auto i> struct Elem {
		[[no_unique_address]] T _t;
		static constexpr auto typeof(Value<i>) { return type<T>; }
		static constexpr auto indexof(Type<T>) { return value<i>; }
		constexpr auto& operator[](Value<i>) const& { return _t; }
		constexpr auto& operator[](Value<i>) & { return _t; }
		constexpr auto&& operator[](Value<i>) && { return std::forward<T>(_t); }
		constexpr auto& operator[](Type<T>) const& { return _t; }
		constexpr auto& operator[](Type<T>) & { return _t; }
		constexpr auto&& operator[](Type<T>) && { return std::forward<T>(_t); }
	};

	template<typename...> struct Tuple;
	template<auto... is, typename... Ts>
	struct Tuple<std::index_sequence<is...>, Ts...>: Elem<Ts, is>... {
		using Elem<Ts, is>::typeof...;
		using Elem<Ts, is>::indexof...;
		using Elem<Ts, is>::operator[]...;
	};
}
namespace zfq {
	template<auto... ts> struct Tuple // HACK for IDE (should be <Type... ts>)
	: _tuple::Tuple<std::make_index_sequence<sizeof...(ts)>, Decltype<ts>...>
	{ friend constexpr Value<sizeof...(ts)> size(Tuple const&) { return {}; } };
	template<typename... Ts> Tuple(Ts...) -> Tuple<type<Ts>...>;
	template<auto... ts> Tuple(Tuple<ts...>) -> Tuple<type<Tuple<ts...>>>;

	template<Tuplish T> constexpr void _zfq_tuplish(T const&);
	template<auto i, typename T> constexpr decltype(auto) get(T&& tuple)
	{ return std::forward<T>(tuple)[value<i>]; }
}
template<typename T> requires requires(T t) { _zfq_tuplish(t); }
struct std::tuple_size<T>
: decltype(size(std::declval<T>())) {};
template<auto i, typename T> requires requires(T t) { _zfq_tuplish(t); }
struct std::tuple_element<i, T>
: decltype(T::typeof(zfq::value<i>)) {};

#endif