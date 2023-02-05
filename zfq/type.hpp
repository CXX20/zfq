#ifndef ZFQ_HEADER_TYPE
#define ZFQ_HEADER_TYPE

#include "value.hpp"

namespace zfq {
	template<typename T> struct Type {
		using type = T;
		constexpr Type() = default;
		template<typename C, std::enable_if_t<
				std::is_same_v<T, typename C::type>, int> = 0> constexpr Type(C) {}
		template<typename... As> constexpr auto operator()(As&&... as) const
		-> decltype(T{std::declval<As>()...}) { return T{std::forward<As>(as)...}; }
		template<typename C> constexpr auto operator==(C) const // HACK for IDE: !!
		-> Value<!!std::is_same_v<T, typename C::type>> { return {}; }
		template<typename C> constexpr auto operator!=(C) const
		-> Value<!std::is_same_v<T, typename C::type>> { return {}; }
	};
	template<typename C> Type(C) -> Type<typename C::type>;
	template<typename T> inline Type<T> constexpr type;

	template<auto t> using Decltype = typename decltype(t)::type;
}

#endif