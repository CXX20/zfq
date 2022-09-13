# The `zfq` C++ library

## Motivation

Most of the time C++ is used as (and considered to be) something either hacky and unsafe (C-style) or costly to develop and run (Java-style). However, it is none of these when used idiomatically, i.e. without ancient approaches and API.

`zfq` is `std` upside down, symbolizing fresh "language-extension" library primitives.

## Features

`zfq` is:

- Minimalistic - you can always just jump into the code and see how everything works.
- Type safe, UB-free, leak-free - any violations are caught in tests at compile time.
- Zero-overhead, tooling-friendly - no ineffective compiler outputs, no broken IDEs.

## Limitations

To preserve all its features, the library abandons the following:

- Old C++ standards.
- Providing FFI.
- Being easy to understand for non-C++ (including C-with-classes) programmers.
- Some `std` interfaces, e.g. those with member `typedef`s (like we haven't had `decltype` for ages).

## Licensing

The whole repository, including the `zfq` library, is freely distributed under the [GNU GPLv3 license](./LICENSE.md).

## Installation

Just download the repository and `#include` its [source code](./zfq/).

Simply build [the `.cpp` files](./test/) to run tests - they're performed at compile time.