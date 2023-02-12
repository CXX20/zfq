#ifndef ZFQ_HEADER_FN
#define ZFQ_HEADER_FN

#include <cassert>
#include <utility>
#include "type.hpp"

namespace zfq::_fn {
	template<typename F> struct Trail: private F {
		template<typename A>
		friend constexpr decltype(auto) operator|(A&& arg, Trail&& self)
		{ return self(std::forward<A>(arg)); }
		constexpr Trail(F const& src): F{src} {}
		Trail(Trail&&) = delete;
	};
}
namespace zfq::adl_barrier {
	template<auto f, u8 n = -1> struct Pipe: Decltype<f> {
		template<typename A>
		friend constexpr decltype(auto) operator|(A&& arg, Pipe const& self)
		{ return self.Decltype<f>::operator()(std::forward<A>(arg)); }
		template<typename A>
		friend constexpr decltype(auto) operator|(A&& arg, Pipe& self)
		{ return self.Decltype<f>::operator()(std::forward<A>(arg)); }
		template<typename A>
		friend constexpr decltype(auto) operator|(A&& arg, Pipe const&& self)
		{ return self.Decltype<f>::operator()(std::forward<A>(arg)); }
		template<typename A>
		friend constexpr decltype(auto) operator|(A&& arg, Pipe&& self)
		{ return self.Decltype<f>::operator()(std::forward<A>(arg)); }
		constexpr Pipe() = default;
		constexpr Pipe(Decltype<f> const& src, Const<n> = {}): Decltype<f>{src} {}
		using Decltype<f>::operator();
		template<typename... As, std::enable_if_t<
			!std::is_invocable_v<Decltype<f>, As...> && sizeof...(As) < n, int
		> = 0> [[nodiscard]] constexpr auto operator()(As&&... args) const {
			return _fn::Trail{[&, this]<typename A>(A&& arg) -> decltype(auto) {
				return Decltype<f>::operator()(
						std::forward<A>(arg), std::forward<As>(args)...);
			}};
		}
	};
	template<typename F> Pipe(F) -> Pipe<type<F>>;
	template<typename F, typename N> Pipe(F, N)
	-> Pipe<type<F>, const_<N::value>>;

	template<auto... fs> struct Overload
	: Decltype<fs>... { using Decltype<fs>::operator()...; };
	template<typename... Fs> Overload(Fs...) -> Overload<type<Fs>...>;
}
namespace zfq {
	using namespace adl_barrier; // TODO extract
	
	template<auto f, auto n> using Cpo = Pipe<type<decltype(f)>, n>;
	
	template<typename T> concept Meta = // TODO extract
		std::is_empty_v<std::remove_cvref_t<T>> &&
		std::is_trivial_v<std::remove_cvref_t<T>>;
	inline Pipe constexpr meta_assert = []<typename T>(T)
	{ static_assert(T::value); };
	inline Pipe constexpr assert_ = Overload{
			[](auto&& t) { std::is_constant_evaluated() && !t ? throw : assert(t); },
			[](Meta auto&& t) { meta_assert(t); }};
}

#endif