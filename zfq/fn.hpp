#ifndef ZFQ_HEADER_FN
#define ZFQ_HEADER_FN

#include "type.hpp"

namespace zfq::_pipe {
	struct Immovable { // HACK should be somewhere else
		constexpr Immovable() = default;
		Immovable(Immovable&&) = delete;
	};

	template<Type f> struct Trail: Decltype<f>, Immovable {
		template<typename A> friend auto operator|(A&&, Trail const&) = delete;
		template<typename A> friend auto operator|(A&&, Trail&) = delete;
		template<typename A> friend constexpr auto operator|(A&& arg, Trail&& me)
		-> decltype(auto) { return me(std::forward<A>(arg)); }
	private: using Decltype<f>::operator();
	};
	template<typename F> Trail(F) -> Trail<Type<F>{}>; // HACK for IDE: not type<>
}
namespace zfq::adl_barrier {
	template<auto f> struct Pipe: private decltype(f) {
		template<typename A> friend constexpr auto operator|(A&& a, Pipe const& me)
		-> decltype(auto) { return me.decltype(f)::operator()(std::forward<A>(a)); }
		template<typename A> friend constexpr auto operator|(A&& a, Pipe& me)
		-> decltype(auto) { return me.decltype(f)::operator()(std::forward<A>(a)); }
		template<typename A> friend constexpr auto operator|(A&& a, Pipe&& me)
		-> decltype(auto) { return me.decltype(f)::operator()(std::forward<A>(a)); }
		using decltype(f)::operator();
		template<typename... As, std::enable_if_t<
			!std::is_invocable_v<decltype(f), As...>, int
		> = 0> [[nodiscard]] constexpr auto operator()(As&&... args) const {
			return _pipe::Trail{[&]<typename A>(A&& arg)
			-> decltype(f(std::declval<A>(), std::declval<As>()...))
			{ return f(std::forward<A>(arg), std::forward<As>(args)...); }};
		}
	};
	template<typename F> Pipe(F) -> Pipe<F{}>;

	template<auto... fs> struct Overload: Decltype<fs>...
	{ using Decltype<fs>::operator()...; }; // HACK for IDE, should be: Type... fs
	template<typename... Fs> Overload(Fs...) -> Overload<type<Fs>...>;
}
namespace zfq { using namespace adl_barrier; }

#endif