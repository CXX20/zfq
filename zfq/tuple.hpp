#ifndef ZFQ_HEADER_TUPLE
#define ZFQ_HEADER_TUPLE

#include "fn.hpp"

namespace zfq {
	template<typename T> concept Tuplish =
		requires { sizeof(std::tuple_size<std::remove_cvref_t<T>>); };

	inline Cpo<Overload{
		[]<typename F, typename T>(F&& fn, T&& tuple) -> decltype(auto)
		requires requires { std::forward<T>(tuple).apply(fn); }
		{ return std::forward<T>(tuple).apply(fn); },
		[]<typename F, typename T>(F&& fn, T&& tuple)
		-> decltype(apply(adl::tag_for(tuple), fn, std::forward<T>(tuple)))
		{ return apply(adl::tag_for(tuple), fn, std::forward<T>(tuple)); }
	}, 2> constexpr apply;
	inline Cpo<Overload{
		[](auto const& t) requires requires { t.size(); } { return t.size(); },
		[](auto const& t) -> decltype(size(adl::tag_for(t), t))
		{ return size(adl::tag_for(t), t); },
	}, 1> constexpr size;
	inline Cpo<Overload{
		[]<typename T>(T&& t) requires requires { std::forward<T>(t).view(); }
		{ return std::forward<T>(t).view(); },
		[]<typename T>(T&& t) -> decltype(view(adl::tag_for(t), std::forward<T>(t)))
		{ return view(adl::tag_for(t), std::forward<T>(t)); }
	}, 1> constexpr view;
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

	template<typename...> struct Base;
	template<auto... is, typename... Ts>
	struct Base<std::index_sequence<is...>, Ts...>: Elem<Ts, is>...
	{ using Elem<Ts, is>::typeof..., Elem<Ts, is>::operator[]...; };
}
namespace zfq {
	template<typename... Ts> struct Tuple
	: _impl::tuple::Base<std::index_sequence_for<Ts...>, Ts...> {
		constexpr auto size() const { return const_<sizeof...(Ts)>; }
		template<typename U> constexpr auto operator==(U const& u) const {
			using zfq::size;
			if constexpr (decltype(size(*this) != size(u))::value) return false_;
			else return apply([&](auto&&... es) {
				return apply([&...es = es](auto&&... ds) {
					return !!(true_ & ... & (es == ds));
				}, u); // TODO remove the explicit capture (compiler bug workaround)
			}, *this);
		}
		template<typename U> constexpr auto operator!=(U const& u) const
		{ return !(*this == u); }
	};
	template<typename... Ts> Tuple(Ts...) -> Tuple<Ts...>;
	template<typename... Ts> Tuple(Tuple<Ts...>) -> Tuple<Tuple<Ts...>>;
	
	template<typename T> auto _zfq_tuplish(T&& t) -> decltype(t.size().value);
	template<auto i, typename T> constexpr decltype(auto) get(T&& tuple)
	{ return std::forward<T>(tuple)[const_<i>]; }
}
namespace zfq::adl {
	template<typename F, typename T>
	constexpr decltype(auto) apply(Tag, F&& fn, T&& tuple) {
		return [&]<auto... is>(std::index_sequence<is...>) -> decltype(auto) {
			return fn(std::forward<T>(tuple)[const_<is>]...);
		}(std::make_index_sequence<decltype(zfq::size(tuple))::value>{});
	}
	template<Tuplish T> constexpr auto view(Tag, T&& tuple) {
		return zfq::apply([]<typename... Es>(Es&&... es) {
			return Tuple<Es&&...>{std::forward<Es>(es)...};
		}, std::forward<T>(tuple));
	}
}
namespace std {
	template<typename T> requires requires(T t) { _zfq_tuplish(t); }
	struct tuple_size<T>
	{ static auto constexpr value = decltype(std::declval<T>().size())::value; };
	template<auto i, typename T> requires requires(T t) { _zfq_tuplish(t); }
	struct tuple_element<i, T>
	{ using type = zfq::Decltype<T::typeof(zfq::const_<i>)>; };
}

#endif