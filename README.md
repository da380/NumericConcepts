# NumericConcepts

[![CI](https://github.com/da380/NumericConcepts/actions/workflows/ci.yml/badge.svg)](https://github.com/da380/NumericConcepts/actions/workflows/ci.yml)
![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![Header only](https://img.shields.io/badge/header--only-yes-blue.svg)
![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)

A header-only C++20 library of concepts for generic numerical programming.

Its purpose is to provide one shared vocabulary across a family of numerical
libraries, so that they all name the same constraints the same way instead of
each growing a private `is_complex` or `RealRange`. There is no runtime code and
there are no types — only names for the constraints that keep recurring when
writing templates over `float`, `double`, `std::complex<T>` and ranges of them.

```cpp
#include <NumericConcepts/NumericConcepts.hpp>

using namespace NumericConcepts;

// Accepts a real or complex range, and a real or complex range that can be
// written to, provided the two share an underlying precision.
template <RealOrComplexRange In, RealOrComplexWritableRange Out>
  requires SameRangePrecision<In, Out>
void Scale(const In& in, Out& out, RangePrecision<In> factor);
```

## Contents

Everything lives in namespace `NumericConcepts`. Concept names are a *category*
prefix plus a *kind* suffix, and every combination of the two exists.

| Category        | Satisfied by                             |
| --------------- | ---------------------------------------- |
| `Integral`      | `std::integral` types                    |
| `Real`          | `std::floating_point` types              |
| `Complex`       | `std::complex<T>` for floating-point `T` |
| `RealOrComplex` | `Real` or `Complex`                      |
| `Numeric`       | `Integral`, `Real` or `Complex`          |

| Kind               | Example                    | Requires                     |
| ------------------ | -------------------------- | ---------------------------- |
| *(none)*           | `Real<T>`                  | the type itself              |
| `Iterator`         | `RealIterator<T>`          | `std::input_iterator`        |
| `WritableIterator` | `RealWritableIterator<T>`  | `std::output_iterator`       |
| `OutputIterator`   | `RealOutputIterator<T, V>` | writable with a named value type |
| `Range`            | `RealRange<T>`             | `std::ranges::input_range`   |
| `WritableRange`    | `RealWritableRange<T>`     | `std::ranges::output_range`  |
| `OutputRange`      | `RealOutputRange<T, V>`    | writable with a named value type |
| `View`             | `RealView<T>`              | range, plus `std::ranges::view` |
| `WritableView`     | `RealWritableView<T>`      | writable range, plus view    |
| `Function`         | `RealFunction<F, Args...>` | `std::invocable`, return type in the category |

The `Writable` kinds take the value type from the sequence; the `Output` kinds
are told it, which makes them more general and lets them accept the pure output
iterators — `std::back_insert_iterator`, `std::ostream_iterator` — whose value
type is `void`.

Ranges and iterators also take a *strength* — `Sized`, `RandomAccess` or
`Contiguous` — placed before the kind, so the full pattern is
`Category [Writable] [Strength] Kind`: `RealContiguousRange`,
`ComplexWritableSizedRange`, `NumericRandomAccessIterator`. The unqualified
concepts require only an input range, which is the weakest useful requirement;
ask for a strength when the algorithm indexes, needs a size up front, or hands
its data to a C or Fortran routine.

Alongside them are the precision utilities `RemoveComplex`, `AddComplex`,
`CommonPrecision`, `PromotedType`, `IteratorValue`, `IteratorPrecision`,
`RangeValue` and `RangePrecision`, and the agreement concepts `SamePrecision`,
`SameIterator`, `SameIteratorValueType`, `SameIteratorPrecision`, `SameRange`,
`SameRangeValueType` and `SameRangePrecision`.

Throughout, top-level cv-qualification is removed before a concept is applied
and references are not, so `Complex<const std::complex<double>>` holds while
`Complex<std::complex<double>&>` does not.

### Open scalars

The concepts above are closed: a type is `Real` or `Complex` only by being a
built-in floating-point type or a `std::complex`. `Algebra.hpp` provides the
open counterpart, describing a type by the arithmetic it supports — `Field`,
`OrderedField`, `ScalableBy`, `Scalar`, `RealScalar`, `ComplexScalar` — so that
a dual number for automatic differentiation, an interval, or an extended
precision type can satisfy your interfaces. A type joins by specialising one
trait:

```cpp
template <NumericConcepts::Real T>
struct NumericConcepts::RealTypeTraits<Dual<T>> {
  using type = T;
};

static_assert(NumericConcepts::Scalar<Dual<double>>);
```

Constrain on `RealOrComplex` when the algorithm depends on the representation,
and on `Scalar` when it only depends on the arithmetic.

## Examples

`examples/` holds a graded set of programs covering each header in turn, from
`01-numeric_types` to `08-scalars`. They assert as much as they print, and are
run by CTest alongside the unit tests. See [`examples/README.md`](examples/README.md)
for the index.

```bash
cmake -S . -B build -DNUMERICCONCEPTS_BUILD_TESTS=ON
cmake --build build
./build/examples/01-numeric_types
```

### Headers

`Numeric.hpp`, `Algebra.hpp`, `Iterators.hpp`, `Ranges.hpp`, `Functions.hpp` and
`Version.hpp` may each be included on their own, or all at once via
`NumericConcepts.hpp`.

`Version.hpp` defines `NUMERICCONCEPTS_VERSION` and
`NUMERICCONCEPTS_VERSION_NUMBER(a, b, c)`, so a dependent library can test which
concepts are present.

Full documentation, including the design rationale, is in
[`docs/mainpage.md`](docs/mainpage.md) and in the Doxygen output built by
`-DNUMERICCONCEPTS_BUILD_DOCS=ON`. That output is not published anywhere yet;
[`docs/publishing-documentation.md`](docs/publishing-documentation.md) is the
recipe for putting it on GitHub Pages.

## Requirements

- A C++20 compiler. GCC 10+, Clang 16+ and MSVC 19.29+ are exercised by CI.
  The headers themselves compile with Clang 14, but that release paired with
  libstdc++ 12 mishandles some standard `<ranges>` pipelines, so it is not
  claimed as supported.
- CMake 3.15 or later.

## Using it

Linking the `NumericConcepts::NumericConcepts` target is all that is needed: it
carries both the include directory and the C++20 requirement, so your project
does not have to set `CMAKE_CXX_STANDARD` on its account.

### FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(
  NumericConcepts
  GIT_REPOSITORY https://github.com/da380/NumericConcepts.git
  GIT_TAG        v0.1.0)
FetchContent_MakeAvailable(NumericConcepts)

target_link_libraries(YourApp PRIVATE NumericConcepts::NumericConcepts)
```

Pin a tag rather than `main` so that the dependency is reviewable.

### find_package

Build and install once:

```bash
git clone https://github.com/da380/NumericConcepts.git
cd NumericConcepts
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=~/local
cmake --install build
```

Then, from your project:

```cmake
find_package(NumericConcepts 0.1 REQUIRED)
target_link_libraries(YourApp PRIVATE NumericConcepts::NumericConcepts)
```

If the install prefix is not a standard system location, point CMake at it when
configuring your project:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=~/local
```

`CMAKE_PREFIX_PATH` may also be set as an environment variable, and takes a
list separated by `:` on Unix or `;` on Windows.

### Vendoring

The library is five headers with no dependencies beyond the standard library, so
copying `include/NumericConcepts` into your own tree is a perfectly reasonable
option.

## Build options

All options are off by default unless noted, and are only of interest when
building this repository directly.

| Option                            | Default        | Effect                                |
| --------------------------------- | -------------- | ------------------------------------- |
| `NUMERICCONCEPTS_BUILD_TESTS`     | `OFF`          | Build the GoogleTest suite            |
| `NUMERICCONCEPTS_BUILD_EXAMPLES`  | on if top-level | Build the programs in `examples/`     |
| `NUMERICCONCEPTS_BUILD_DOCS`      | `OFF`          | Build the Doxygen documentation       |
| `NUMERICCONCEPTS_INSTALL`         | `ON`           | Generate the install rules, even as a subproject |

```bash
cmake -S . -B build -DNUMERICCONCEPTS_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

The unprefixed `BUILD_TESTS` and `BUILD_DOCS` still work, but are deprecated.

`NUMERICCONCEPTS_INSTALL` stays on when NumericConcepts is a subproject, which
differs from the common top-level-only default. A library that links it
publicly and exports its own targets cannot be installed otherwise — CMake
refuses with *"requires target NumericConcepts that is not in any export
set"* — and being consumed by such libraries is the point of this one. Turn it
off if your project really does not want the rules.

## Versioning

Releases are tagged `vMAJOR.MINOR.PATCH`. Within a major version the vocabulary
only grows: a minor release may add concepts, but will avoid changing the
meaning of an existing one.

The installed package declares `SameMajorVersion` compatibility, so
`find_package(NumericConcepts 0.1)` is satisfied by any later `0.x` and not by
a future `1.0`.

## Licence

BSD 3-Clause. See [LICENSE](LICENSE).
