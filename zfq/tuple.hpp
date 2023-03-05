#ifndef ZFQ_HEADER_TUPLE
#define ZFQ_HEADER_TUPLE

#include "fn.hpp"

namespace zfq::_tuple {
	template<typename T> concept ZfqTuplish = requires(T t) {
		{ decltype(t.size())::value };
		{ adl::tag_for(t) } -> std::same_as<adl::Generic>;
	};

	template<typename T> class [[nodiscard]] Expand {
		T&& raw;
	public:
		constexpr Expand(T&& ref): raw{std::forward<T>(ref)} {}
		template<typename F> constexpr auto operator|(F&& fn) &&
		-> decltype(apply(adl::dispatch(raw), fn, std::forward<T>(raw)))
		{ return apply(adl::dispatch(raw), fn, std::forward<T>(raw)); }
	};

	template<typename T, auto i> struct Elem {
		T _t;
		static constexpr auto typeof(Const<i>) { return type<T>; }
		constexpr auto&& operator[](Const<i>) const& { return _t; }
		constexpr auto&& operator[](Const<i>) & { return _t; }
		constexpr auto&& operator[](Const<i>) && { return std::forward<T>(_t); }
		constexpr auto&& operator[](Const<i>) const&&
		{ return std::forward<T const>(_t); }
		constexpr auto&& operator[](Type<T>) const& { return _t; }
		constexpr auto&& operator[](Type<T>) & { return _t; }
		constexpr auto&& operator[](Type<T>) && { return std::forward<T>(_t); }
		constexpr auto&& operator[](Type<T>) const&&
		{ return std::forward<T const>(_t); }
	};

	template<typename...> struct Base;
	template<auto... is, typename... Ts>
	struct Base<std::index_sequence<is...>, Ts...>: Elem<Ts, is>...
	{ using Elem<Ts, is>::typeof..., Elem<Ts, is>::operator[]...; };
}
namespace zfq {
	template<typename T> concept Tuplish =
		requires { std::tuple_size<std::remove_cvref_t<T>>::value; };

	inline Pipe constexpr expand{[]<typename T>(T&& tuple)
	{ return _tuple::Expand<T>{std::forward<T>(tuple)}; }, 1_c};
	inline Cpo<[](auto&& t) -> decltype(size(adl::dispatch(t), t))
	{ return size(adl::dispatch(t), t); }, 1> constexpr size;
	inline Cpo<[]<typename T>(T&& t)
	-> decltype(view(adl::dispatch(t), std::forward<T>(t)))
	{ return view(adl::dispatch(t), std::forward<T>(t)); }, 1> constexpr view;

	template<typename... Ts> struct Tuple
	: _tuple::Base<std::index_sequence_for<Ts...>, Ts...> {
		constexpr auto size() const { return const_<sizeof...(Ts)>; }
		template<typename U> constexpr auto operator==(U const& u) const {
			using zfq::size;
			if constexpr (decltype(size(*this) != size(u))::value) return false_;
			else return *this | expand | [&](auto&&... es) {
				return u | expand | [&...es = es](auto&&... ds)
				{ return !!(true_ & ... & (es == ds)); }; // TODO rm [es]; compiler bug
			};
		}
		template<typename U> constexpr auto operator!=(U const& u) const
		{ return !(*this == u); }
	};
	template<typename... Ts> Tuple(Ts...) -> Tuple<Ts...>;
	template<typename... Ts> Tuple(Tuple<Ts...>) -> Tuple<Tuple<Ts...>>;
	
	template<auto i, _tuple::ZfqTuplish T> constexpr auto get(T&& t)
	-> decltype(auto) { return std::forward<T>(t)[const_<i>]; }
}
namespace zfq::adl {
	template<typename B, typename F, typename T>
	constexpr auto apply(Specific<B>, F&& fn, T&& tuple)
	-> decltype(std::forward<T>(tuple).apply(fn))
	{ return std::forward<T>(tuple).apply(fn); }
	template<typename F, Tuplish T>
	constexpr decltype(auto) apply(Generic, F&& fn, T&& tuple) {
		return [&]<auto... is>(std::index_sequence<is...>) -> decltype(auto) {
			return fn(std::forward<T>(tuple)[const_<is>]...);
		}(std::make_index_sequence<decltype(zfq::size(tuple))::value>{});
	}
	template<typename B, typename T> constexpr auto size(Specific<B>, T&& t)
	-> decltype(t.size()) { return t.size(); }
	template<typename B, typename T> constexpr auto view(Specific<B>, T&& t)
	-> decltype(std::forward<T>(t).view()) { return std::forward<T>(t).view(); }
	template<Tuplish T> constexpr auto view(Generic, T&& tuple) {
		return std::forward<T>(tuple) | expand | []<typename... Es>(Es&&... es)
		{ return Tuple<Es&&...>{std::forward<Es>(es)...}; };
	}
}
namespace std {
	template<zfq::_tuple::ZfqTuplish T> struct tuple_size<T>
	{ static auto constexpr value = decltype(declval<T>().size())::value; };
	template<auto i, zfq::_tuple::ZfqTuplish T> struct tuple_element<i, T>
	{ using type = zfq::Decltype<T::typeof(zfq::const_<i>)>; };
}

#endif