# The `zfq` C++ library

## Motivation

Most of the time C++ is used as (and considered to be) something either hacky and unsafe (C-style) or costly to develop and run (Java-style). This library helps to kill those myths by supporting modern approaches and API.

`zfq` is `std` upside down, symbolizing fresh "language extension" library primitives.

## Features

`zfq` is:

- Minimalistic - you can always jump into the code and quickly understand how everything works.
- Type safe, UB-free, leak-free - any violations are caught in tests at compile time.
- Zero-overhead, tooling-friendly - no ineffective compiler outputs, no broken IDEs.

## Limitations

To preserve all its features, the library abandons the following:

- Old C++ standards.
- Providing FFI.
- Being easy to understand for non-C++ (including C-with-classes) programmers.
- Some ancient `std` interfaces.

## Licensing

The whole `zfq` library is freely distributed under the [GNU GPLv3 license](./LICENSE.md).

## Installation

Just download the repository and `#include` its [source code](./zfq/).

Tests are performed at compile time. To run them, simply build [the `.cpp` files](./test/).