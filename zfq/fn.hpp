#ifndef ZFQ_HEADER_FN
#define ZFQ_HEADER_FN

#include <cassert>
#include <utility>
#include "type.hpp"

namespace zfq::adl { inline struct Tag {} constexpr tag; }
namespace zfq::_impl::fn {
	template<typename> struct Type {};
	template<template<typename...> typename> struct Template {};

	template<typename F> struct Trail: private F {
		constexpr Trail(F src): F{src} {}
		Trail(Trail&&) = delete;
		template<typename A> friend void operator|(A&&, Trail const&) = delete;
		template<typename A> friend void operator|(A&&, Trail&) = delete;
		template<typename A> friend void operator|(A&&, Trail const&&) = delete;
		template<typename A> friend constexpr auto operator|(A&& a, Trail&& me)
		-> decltype(auto) { return me(std::forward<A>(a)); }
	};
}
namespace zfq {
	template<typename F, unsigned n = 1024> struct Pipe: private F {
		constexpr Pipe() = default;
		constexpr Pipe(F src, Const<n> = {}): F{src} {}
		using F::operator();
		template<typename... As, std::enable_if_t<
			!std::is_invocable_v<F, As...> && sizeof...(As) < n, int
		> = 0> [[nodiscard]] constexpr auto operator()(As&&... as) const {
			return _impl::fn::Trail{[&, *this]<typename A>(A&& a) -> decltype(auto)
			{ return F::operator()(std::forward<A>(a), std::forward<As>(as)...); }};
		}
		template<typename A> friend constexpr auto operator|(A&& a, Pipe me)
		-> decltype(auto) { return me.F::operator()(std::forward<A>(a)); }
	};
	template<typename F, typename N> Pipe(F, N) -> Pipe<F, N::value>;
	template<auto f, auto n> using Cpo = Pipe<decltype(f), n>;
	
	template<typename... Fs> struct Overload: Fs... { using Fs::operator()...; };
	template<typename... Fs> Overload(Fs...) -> Overload<Fs...>;

	inline Pipe constexpr requires_ = []<typename A, typename F>(A&& arg, F&& fn)
	{ return const_<requires { fn(std::forward<A>(arg)); }>; };
	
	template<typename T> constexpr auto adl_tag(T const&) { return adl::tag; }
}
namespace zfq::adl {
	inline Pipe constexpr tag_for = [](auto const& t)
	-> decltype(adl_tag(adl::tag, t)) { return adl_tag(adl::tag, t); };

	template<typename T, typename... As>
	constexpr auto adl_tag(Tag, T const&, As&&...) {
		static_assert(!sizeof...(As));
		return tag;
	}
	template<typename T> constexpr auto adl_tag(Tag, T const&)
	-> decltype(adl_tag(_impl::fn::Type<T>{})) { return {}; }
	template<template<typename...> typename C, typename... As>
	constexpr auto adl_tag(Tag, C<As...> const&)
	-> decltype(adl_tag(_impl::fn::Template<C>{})) { return {}; }
}

#endif