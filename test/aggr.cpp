#include "../zfq/aggr.hpp"

namespace {
	using zfq::size;

	static_assert([]<auto... is>(std::index_sequence<is...>) {
		return size(zfq::Tuple{is...}).value == 22;
	}(std::make_index_sequence<22>{}));

	struct Empty {} empty;
	static_assert(!zfq::requires_(Empty{}, [](auto&& e) -> decltype(size(e)) {}));
	static_assert(!decltype(
			zfq::requires_(empty, [](auto&& e) -> decltype(size(e)) {}))::value);

	struct One { int a; } one{};
	struct Two { int a, b; } two{};
	struct Three { int a, b, c; } three{};
	struct Tricky { int const& l; int&& r; int v; } tricky{42, 42};
	static_assert(decltype(size(One{}))::value == 1);
	static_assert(decltype(size(Two{}))::value == 2);
	static_assert(decltype(size(Three{}))::value == 3);
	static_assert(decltype(size(Tricky{42, 42}))::value == 3);
	static_assert(decltype(size(one))::value == 1);
	static_assert(decltype(size(two))::value == 2);
	static_assert(decltype(size(three))::value == 3);
	static_assert(decltype(size(tricky))::value == 3);

	using zfq::view;
	
	static_assert(!zfq::requires_(Empty{}, [](auto&& e) -> decltype(view(e)) {}));
	static_assert(view(One{123}) == zfq::Tuple{123});
	static_assert(view(Two{12, 34}) == zfq::Tuple{12, 34});
	static_assert(view(Three{1, 2, 3}) == zfq::Tuple{1, 2, 3});
	static_assert(view(Tricky{1, 2, 3}) == zfq::Tuple{1, 2, 3});

	static_assert(!decltype(
			zfq::requires_(empty, [](auto&& e) -> decltype(view(e)) {}))::value);
	static_assert(std::is_same_v<decltype(view(one)), zfq::Tuple<int&>>);
	static_assert(std::is_same_v<decltype(view(two)), zfq::Tuple<int&, int&>>);
	static_assert(std::is_same_v<
			decltype(view(three)), zfq::Tuple<int&, int&, int&>>);
	static_assert(std::is_same_v<
			decltype(view(tricky)), zfq::Tuple<int const&, int&, int&>>);

	static_assert(std::is_same_v<decltype(view(One{})), zfq::Tuple<int&&>>);
	static_assert(std::is_same_v<
			decltype(view(Two{})), zfq::Tuple<int&&, int&&>>);
	static_assert(std::is_same_v<
			decltype(view(Three{})), zfq::Tuple<int&&, int&&, int&&>>);
	static_assert(std::is_same_v<
			decltype(view(Tricky{42, 42})), zfq::Tuple<int const&, int&&, int&&>>);

	using zfq::expand;

	static_assert(!zfq::requires_(
			Empty{}, [](auto&& e) -> decltype(e | expand | [] {}) {}));
	static_assert(!decltype(zfq::requires_(
			empty, [](auto&& e) -> decltype(e | expand | [] {}) {}))::value);
	static_assert(One{42} | expand | [](auto _42) { return _42 == 42; });
	static_assert((
		Tricky{1, 2, 3} | expand | [](auto... es) { return (... - es); }
	) == 1 - 2 - 3);
}