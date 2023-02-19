#ifndef ZFQ_HEADER_TUPLE
#define ZFQ_HEADER_TUPLE

#include "fn.hpp"

namespace zfq::cpo {
	inline Cpo<Overload{
		[](auto const& t) requires requires { t.size(); } { return t.size(); },
		[](auto const& t) -> decltype(size(t)) { return size(t); }
	}, 1> constexpr size;
	inline Cpo<[]<typename F, typename T>(F&& fn, T&& tuple)
	-> decltype(apply(fn, std::forward<T>(tuple)))
	{ return apply(fn, std::forward<T>(tuple)); }, 2> constexpr apply;
}
namespace zfq::_impl::tuple {
	template<typename T, auto i> struct Elem {
		T _t;
		
		static constexpr auto typeof(Const<i>) { return type<T>; }
		
		constexpr auto&& operator[](Const<i>) const& { return _t; }
		constexpr auto&& operator[](Const<i>) & { return _t; }
		constexpr auto&& operator[](Const<i>) && { return std::forward<T>(_t); }
		constexpr auto&& operator[](Type<T>) const& { return _t; }
		constexpr auto&& operator[](Type<T>) & { return _t; }
		constexpr auto&& operator[](Type<T>) && { return std::forward<T>(_t); }
	};

	template<typename...> struct Tuple;
	template<auto... is, typename... Ts>
	struct Tuple<std::index_sequence<is...>, Ts...>: Elem<Ts, is>...
	{ using Elem<Ts, is>::typeof..., Elem<Ts, is>::operator[]...; };
}
namespace zfq {
	template<auto... ts> struct Tuple
	: _impl::tuple::Tuple<
			std::make_index_sequence<sizeof...(ts)>, Decltype<ts>...> {
		constexpr auto size() const { return sizeof_pack(ts...); }
		template<typename U> constexpr auto operator==(U const& u) const {
			if constexpr (decltype(cpo::size(*this) == cpo::size(u))::value)
				return cpo::apply([&](auto&&... es) {
					return cpo::apply([&...es = es](auto&&... ds) {
						return !!(true_ & ... & (es == ds));
					}, u); // TODO remove the explicit capture (compiler bug workaround)
				}, *this);
			else return false_;
		}
		template<typename U> constexpr auto operator!=(U const& u) const
		{ return !(*this == u); }
	};
	template<typename... Ts> Tuple(Ts...) -> Tuple<type<Ts>...>;
	template<auto... ts> Tuple(Tuple<ts...>) -> Tuple<type<Tuple<ts...>>>;
	
	inline Pipe constexpr make_idx_seq = []<typename N>(N) {
		return []<auto... is>(std::index_sequence<is...>)
		{ return Tuple{const_<is>...}; }(std::make_index_sequence<N::value>{});
	};
	template<typename F, typename T>
	constexpr decltype(auto) apply(F&& fn, T&& tuple) {
		return [&]<auto... is>(std::index_sequence<is...>) -> decltype(auto) {
			return fn(std::forward<T>(tuple)[const_<is>]...);
		}(std::make_index_sequence<decltype(cpo::size(tuple))::value>{});
	}
	
	template<typename T> auto _zfq_tuplish(T const& t)
	-> std::enable_if_t<Meta<decltype(cpo::size(t))>>;
	template<auto i, typename T> constexpr decltype(auto) get(T&& tuple)
	{ return std::forward<T>(tuple)[const_<i>]; }
}
template<typename T> requires requires(T t) { _zfq_tuplish(t); }
struct std::tuple_size<T> {
	static auto constexpr value =
		decltype(zfq::cpo::size(std::declval<T>()))::value;
};
template<auto i, typename T> requires requires(T t) { _zfq_tuplish(t); }
struct std::tuple_element<i, T>
{ using type = zfq::Decltype<T::typeof(zfq::const_<i>)>; };

#endif