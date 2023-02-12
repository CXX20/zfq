#ifndef ZFQ_HEADER_TYPE
#define ZFQ_HEADER_TYPE

#include "const.hpp"

namespace zfq {
	template<typename T> struct Type {
		using type = T;
		constexpr Type() = default;
		template<typename U>
		constexpr Type(U) requires std::is_same_v<T, typename U::type>
		{}
		template<typename U> constexpr auto operator==(U) const
		{ return const_<std::is_same_v<T, typename U::type>>; }
		template<typename U> constexpr auto operator!=(U u) const
		{ return !(*this == u); }
	};
	template<typename U> Type(U) -> Type<typename U::type>;
	template<typename T> inline Type<T> constexpr type;
	template<auto t> using Decltype = typename decltype(t)::type;
}

#endif