#ifndef ZFQ_HEADER_TYPE
#define ZFQ_HEADER_TYPE

#include <type_traits>
#include "const.hpp"

namespace zfq {
	template<typename T> struct Type {
		using type = T;
		constexpr Type() = default;
		template<typename C>
		constexpr Type(C) requires std::is_same_v<T, typename C::type>
		{}
		template<typename C> constexpr auto operator==(C) const
		-> Const<!!std::is_same_v<T, typename C::type>> { return {}; } // TODO rm !!
		template<typename C> constexpr auto operator!=(C) const
		-> Const<!std::is_same_v<T, typename C::type>> { return {}; }
	};
	template<typename C> Type(C) -> Type<typename C::type>;
	template<typename T> inline Type<T> constexpr type;
	template<auto t> using Decltype = typename decltype(t)::type;
}

#endif