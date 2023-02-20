#ifndef ZFQ_HEADER_FN
#define ZFQ_HEADER_FN

#include <cassert>
#include <utility>
#include "type.hpp"

namespace zfq::_impl::fn {
	template<auto t> struct Hide: Decltype<t> {};
	inline auto constexpr hide_ttp_from_adl_impl = []<typename T>(T const&) {
		if constexpr (std::is_fundamental_v<T>) return type<T>;
		else return type<Hide<type<T>>>;
	}; // TODO remove this IDE bug workaround
	template<typename T> auto hide_ttp_from_adl(T&& t)
	-> typename decltype(hide_ttp_from_adl_impl(t))::type;

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
}
namespace zfq::adl {
	inline struct Tag {} constexpr tag;
	inline Pipe constexpr overridden_tag{[](auto&& t)
	-> decltype(adl_tag(_impl::fn::hide_ttp_from_adl(t))) { return {}; }, 1_c};
	inline Pipe constexpr tag_for = Overload{
			overridden_tag,
			[](auto&& t) requires (!requires { overridden_tag(t); }) { return tag; }};
}
namespace zfq {
	template<typename T> constexpr auto adl_tag(T const&) { return adl::tag; }
}

#endif