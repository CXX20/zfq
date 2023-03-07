#include "../zfq/type.hpp"

namespace {
	using zfq::type;

	static_assert(type<int> == std::type_identity<int>{});
	static_assert(type<int> != std::type_identity<unsigned>{});

	using zfq::Type;

	static_assert(std::is_same_v<decltype(Type{type<int>}), Type<int>>);
	static_assert(std::is_same_v<
			decltype(Type{std::type_identity<int>{}}), Type<int>>);

	using zfq::Decltype;
	
	static_assert(std::is_same_v<Decltype<type<int const&>>, int const&>);
}