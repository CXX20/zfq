# The `zfq` C++ library

## Motivation

Most of the time C++ is used as (and considered to be) either something hacky and unsafe (C-style) or slow and wordy (Java-style). However, this is actually about what we call "C with classes" - old C++ versions with ancient approaches.

What we want is quick and pretty programs but they need to be built on top of modern zero-cost primitives, most of which can only be implemented in C++ (at least among mainstream languages). That's why the `zfq` C++ library was created. Thanks to its compile time tests, there's also no need to worry about memory corruptions or resource leaks.

The name `zfq` is `std` upside down, symbolizing a fresh reinterpretation of what a language extension library is.

## Features

`zfq` aims at helping with fundamental C++ and STL problems:

- Even constexpr-for-the-caller function arguments are not constexpr-for-the-callee, so things like `tuple[42]` or `static_or_usual_assert(arg)` don't work. Solution: [built-in-like NTTP wrapper](./zfq/const.hpp).

- ADL inspects the namespaces of TTPs, so `apply(fn, my::Tuple{std::tuple{}})` can't just choose `my::apply` because there's also `std::apply`. Solution: [tag "multidispatching"](./zfq/fn.hpp). Besides, it provides "ordered" `fn | apply(tuple) | do_sth_else` chaining instead of `do_sth_else(apply(fn, tuple))`.

- Pack expansion statements like `template<typename... Ts> struct Foo { Ts... ts; }` are not allowed. `std::tuple` tries to aid but it's slow due to recursion and constructor argument moves instead of in-place member initialization. Solution: [aggregate tuple](./zfq/tuple.hpp).

- Memberwise looping/indexing is not allowed even for aggregate types. Solution: [aggregate-to-tuple hack](./zfq/aggregate.hpp);

## Limitations

To preserve its features, `zfq` has to give up some things:

- Being easy to understand for non-C++ (including C with classes) programmers. Some certain programming background is needed anyway. If you don't have it, questions are welcome.

- Backwards-compatible API. In a language with introspection, which C++ kinda is, almost any change is breaking: e.g. `requires(zfq::Foo t) { t.update(); }` produces a different result after `.update()` is added. Moreover, snowball-like degradation of the API you get isn't justified by someone who can neither adopt an update nor stay with an older version.

- FFI. You can't integrate C++ API into languages with a weaker type system, which means any (mainstream?) language. That's almost never helpful anyway.

- Stable ABI. "True" C++ ABI isn't really possible - you can't link a constexpr function, a function template etc.

## Usage

The whole `zfq` library and its git repository are freely distributed under the [GNU GPLv3 license](./LICENSE.md). To use `zfq`, download it and `#include` its [source code](./zfq/). To run tests, just build [the `.cpp` files](./test/).

## Extension and integration

**How it works:** The library introduces the idea of "tag multidispatching" - unless a member function implementation is found, all `zfq` CPOs ADL-search (based on the "top level" type of the "main" parameter) for the tag to be dispatched on, defaulting to the `zfq` tag. Afterwards, the tag is used as the first parameter in an unqualified function call.

**How to use:** Say, there's a `zfq::foo(sth)` CPO which you want to extend for `yours::Sth`. Here are the options, from easiest to hardest:

- If providing a member function is OK, overload `sth.foo()`.

- If having a `yours::foo` function is OK (e.g. there's no `yours::foo` object to conflict), overload `yours::foo(zfq::adl::Tag, Sth)`.

- If the implementation should work for all types from `yours`, overload `yours::adl::Tag yours::adl_tag(T const&)` and `yours::adl::foo(Tag, T)`.

- If the implementation should be program-wide generic (unless overridden with a custom `adl_tag()`), overload `zfq::adl::foo(Tag, T)`.