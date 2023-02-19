#include "../zfq/aggregate.hpp"

namespace {
	using zfq::size;

	static_assert([]<auto... is>(std::index_sequence<is...>) {
		zfq::apply([](auto...) {}, zfq::Tuple{zfq::const_<is>...});
		return true;
	}(std::make_index_sequence<22>{}));

	struct Empty {} empty;
	struct One { int a; } one{123};
	struct Two { int a, b; } two{12, 34};
	struct Three { int a, b, c; } three{1, 2, 3};
	struct Tricky { int const& l; int&& r; int v; } tricky{1, 2, 3};
	static_assert(decltype(size(empty))::value == 0);
	static_assert(decltype(size(one))::value == 1);
	static_assert(decltype(size(two))::value == 2);
	static_assert(decltype(size(three))::value == 3);
	static_assert(decltype(size(tricky))::value == 3);
	static_assert(decltype(size(Tricky{1, 2, 3}))::value == 3);

	using zfq::view;
	
	static_assert(view(Empty{}) == zfq::Tuple{});
	static_assert(view(One{123}) == zfq::Tuple{123});
	static_assert(view(Two{12, 34}) == zfq::Tuple{12, 34});
	static_assert(view(Three{1, 2, 3}) == zfq::Tuple{1, 2, 3});
	static_assert(view(Tricky{1, 2, 3}) == zfq::Tuple{1, 2, 3});

	static_assert(std::is_same_v<decltype(view(empty)), zfq::Tuple<>>);
	static_assert(std::is_same_v<decltype(view(one)), zfq::Tuple<int&>>);
	static_assert(std::is_same_v<decltype(view(two)), zfq::Tuple<int&, int&>>);
	static_assert(std::is_same_v<
			decltype(view(three)), zfq::Tuple<int&, int&, int&>>);
	static_assert(std::is_same_v<
			decltype(view(tricky)), zfq::Tuple<int const&, int&, int&>>);

	static_assert(std::is_same_v<decltype(view(Empty{})), zfq::Tuple<>>);
	static_assert(std::is_same_v<decltype(view(One{})), zfq::Tuple<int&&>>);
	static_assert(std::is_same_v<
			decltype(view(Two{})), zfq::Tuple<int&&, int&&>>);
	static_assert(std::is_same_v<
			decltype(view(Three{})), zfq::Tuple<int&&, int&&, int&&>>);
	static_assert(std::is_same_v<
			decltype(view(Tricky{42, 42})), zfq::Tuple<int const&, int&&, int&&>>);

	using zfq::apply;

	static_assert(apply([] { return true; }, Empty{}));
	static_assert(
			apply([](auto... es) { return (... * es); }, Tricky{2, 5, 7}) ==
			2 * 5 * 7);
}