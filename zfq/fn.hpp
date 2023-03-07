#ifndef ZFQ_HEADER_FN
#define ZFQ_HEADER_FN

#include <utility>
#include "type.hpp"

namespace zfq::_fn {
	template<typename F> struct [[nodiscard]] Trail: private F {
		constexpr Trail(F src): F{src} {}
		Trail(Trail&&) = delete;
		template<typename A> friend constexpr auto operator|(A&& arg, Trail&& me)
		-> decltype(std::declval<F const&>()(std::forward<A>(arg)))
		{ return me(std::forward<A>(arg)); }
	};

	template<auto t> struct Hide: Decltype<t> {};
	template<auto t> requires std::is_fundamental_v<Decltype<t>> struct Hide<t>
	{};
}
namespace zfq {
	template<typename F, unsigned n = 4096> struct Pipe: F {
		constexpr Pipe() = default;
		constexpr Pipe(F src, Const<n> = {}): F{src} {}
		using F::operator();
		template<typename... As> constexpr auto operator()(As&&... tail) const
		requires (!std::invocable<F, As...> && sizeof...(As) < n) {
			F const& self = *this;
			return _fn::Trail{[&]<typename A>(A&& arg)
			-> decltype(self(std::forward<A>(arg), std::forward<As>(tail)...))
			{ return self(std::forward<A>(arg), std::forward<As>(tail)...); }};
		}
		template<typename A> friend constexpr auto operator|(A&& arg, Pipe self)
		-> decltype(std::declval<F const&>()(std::forward<A>(arg)))
		{ return self.F::operator()(std::forward<A>(arg)); }
	};
	template<typename F, typename N> Pipe(F, N) -> Pipe<F, N::value>;
	template<auto f, auto n> using Cpo = Pipe<decltype(f), n>;
	
	template<typename... Fs> struct Overload: Fs... { using Fs::operator()...; };
	template<typename... Fs> Overload(Fs...) -> Overload<Fs...>;

	inline Pipe constexpr requires_{[]<typename A, typename F>(A&& arg, F&& fn)
	{ return const_<requires { fn(std::forward<A>(arg)); }>; }, 2_c};
}
namespace zfq::adl {
	struct Generic {};
	
	template<typename T, typename = Generic> struct Specific: T
	{ constexpr operator Generic() const { return {}; } };
	template<typename T> Specific(T) -> Specific<T>;

	inline Pipe constexpr tag_for{[]<typename T>(T const&)
	-> decltype(adl_tag(std::declval<_fn::Hide<type<T>>>())) { return {}; }, 1_c};
	inline Pipe constexpr dispatch{Overload{
		[](auto&& t) requires requires { tag_for(t); }
		{ return Specific{tag_for(t)}; },
		[](auto&&) { return Specific{Generic{}}; }
	}, 1_c};
}
namespace zfq {
	template<typename T> constexpr adl::Generic adl_tag(T const&) { return {}; }
}

#endif