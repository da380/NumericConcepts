@mainpage NumericConcepts

@tableofcontents

@section intro_sec Introduction

**NumericConcepts** is a header-only C++20 library of concepts for generic
numerical programming. It exists to give a *single, shared vocabulary* to a
family of numerical libraries: rather than each one growing its own
`is_complex`, `RealRange` or `SamePrecision`, they all name the same things the
same way, and their interfaces compose without translation.

The library is deliberately small and deliberately unopinionated. It adds no
runtime code, no types and no algorithms — only names for constraints that come
up over and over when writing templates over `float`, `double`,
`std::complex<T>` and ranges of them.

Some of the concepts duplicate, or thinly wrap, ones in `<concepts>`,
`<iterator>` and `<ranges>`. That is intentional: `NumericConcepts::Integral`
and `std::integral` mean the same thing, but having the whole vocabulary spelled
consistently in one namespace makes constrained signatures read evenly, and
gives a single place to refine a definition later.

@section install_sec Getting started

@subsection install_fetch With FetchContent

```cmake
cmake_minimum_required(VERSION 3.15)
project(MyProject LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
  NumericConcepts
  GIT_REPOSITORY https://github.com/da380/NumericConcepts.git
  GIT_TAG        v0.1.0)
FetchContent_MakeAvailable(NumericConcepts)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE NumericConcepts::NumericConcepts)
```

@subsection install_find With find_package

```cmake
find_package(NumericConcepts 0.1 REQUIRED)
target_link_libraries(my_app PRIVATE NumericConcepts::NumericConcepts)
```

Either way, linking the target is enough: it carries both the include directory
and the C++20 requirement, so there is no need to set `CMAKE_CXX_STANDARD`
yourself. See the README for installation details.

@subsection install_use Using it

```cpp
#include <NumericConcepts/NumericConcepts.hpp>

// Or, if you prefer to include only what you need:
#include <NumericConcepts/Numeric.hpp>
#include <NumericConcepts/Ranges.hpp>
```

Everything lives in namespace `NumericConcepts`.

@section naming_sec How the names are built

Almost every concept in the library is a *category* prefix attached to a *kind*
suffix, which makes the whole set predictable once the two halves are known.

The **categories** classify the scalar type involved:

| Category         | Satisfied by                                          |
| ---------------- | ----------------------------------------------------- |
| `Integral`       | `std::integral` types                                  |
| `Real`           | `std::floating_point` types                            |
| `Complex`        | `std::complex<T>` for floating-point `T`               |
| `RealOrComplex`  | `Real` or `Complex`                                    |
| `Numeric`        | `Integral`, `Real` or `Complex`                        |

The **kinds** say what the category applies to:

| Kind                  | Example                     | Meaning                                        |
| --------------------- | --------------------------- | ---------------------------------------------- |
| *(none)*              | `Real<T>`                   | the type itself                                 |
| `Iterator`            | `RealIterator<T>`           | `std::input_iterator` with that value type      |
| `WritableIterator`    | `RealWritableIterator<T>`   | `std::output_iterator` with that value type     |
| `OutputIterator`      | `RealOutputIterator<T, V>`  | writable with a *named* value type `V`          |
| `Range`               | `RealRange<T>`              | `std::ranges::input_range` with that value type |
| `WritableRange`       | `RealWritableRange<T>`      | `std::ranges::output_range` with that value type |
| `OutputRange`         | `RealOutputRange<T, V>`     | writable with a *named* value type `V`          |
| `View`                | `RealView<T>`               | a `Range` that is also a `std::ranges::view`    |
| `WritableView`        | `RealWritableView<T>`       | a `WritableRange` that is also a view           |
| `Function`            | `RealFunction<F, Args...>`  | invocable returning that category               |

So `ComplexWritableView` is "a view of `std::complex` values that can be written
through", and `NumericIterator` is "an input iterator over any numeric type".
Every combination in the two tables exists.

The `Writable` and `Output` kinds are two ways of asking the same question. The
`Writable` forms take the value type from the sequence itself, and so describe a
mutable sequence of numbers. The `Output` forms take it as a second template
argument, which makes them both more general — `RealOutputRange<R, float>` is
"a range I may write `float`s into", whatever `R` holds — and applicable to the
pure output iterators, `std::back_insert_iterator` and `std::ostream_iterator`,
whose value type is `void` and which therefore satisfy no `Writable` concept at
all. In the `Output` forms the category is tested against the value type with
references and cv-qualification removed, so the idiomatic `const T &` spelling
works alongside a plain `T`.

Ranges and iterators additionally take a **strength**, inserted before the kind,
which says how the sequence may be traversed:

| Strength       | Ranges                        | Iterators                     |
| -------------- | ----------------------------- | ----------------------------- |
| `Sized`        | `std::ranges::sized_range`    | —                             |
| `RandomAccess` | `std::ranges::random_access_range` | `std::random_access_iterator` |
| `Contiguous`   | `std::ranges::contiguous_range`    | `std::contiguous_iterator`    |

giving names such as `RealContiguousRange`, `ComplexWritableSizedRange` and
`NumericRandomAccessIterator`. The full pattern is therefore

```
Category [Writable] [Strength] Kind
```

There are no view forms of the strengths, because a view is a range; write
`RealContiguousRange<V> and std::ranges::view<V>` where one is needed.

@section design_sec Design notes

@subsection design_strength Range and iterator strength

The unqualified range and iterator concepts require only
`std::ranges::input_range` and `std::input_iterator`. That is the weakest useful
requirement, and it is the default on purpose: an algorithm that really can
consume a single-pass range should say so.

Numerical code usually needs more, and the strength refinements above supply it
without the caller writing the same three lines each time:

```cpp
// Before: every consumer wrote its own refinement.
template <typename T>
concept SampledSignal = requires {
  requires NumericConcepts::RealRange<T>;
  requires std::ranges::random_access_range<T>;
  requires std::ranges::sized_range<T>;
};

// Now:
template <NumericConcepts::RealRandomAccessRange T>
void Resample(const T& signal);
```

The strengths are independent, and are not implied by one another beyond the
implications the standard concepts already carry: `std::vector<double>` models
all three, `std::list<double>` only `Sized`, and an infinite
`std::ranges::iota_view` none of them. Ask for exactly what the algorithm uses.

@subsection design_precision Precision

`RemoveComplex<T>` maps a `RealOrComplex` type to its underlying floating-point
precision, so `RemoveComplex<std::complex<double>>` and `RemoveComplex<double>`
are both `double`. `SamePrecision`, `SameIteratorPrecision` and
`SameRangePrecision` build on it, and are the usual way to reject a
mixed-precision call:

```cpp
template <NumericConcepts::RealOrComplexRange XRange,
          NumericConcepts::RealOrComplexRange YRange>
  requires NumericConcepts::SameRangePrecision<XRange, YRange>
auto Interpolate(const XRange& x, const YRange& y);
```

This deliberately allows `x` to be real while `y` is complex — a very common
pairing — while still rejecting a `float` sampled against a `double`.

Three further aliases name the types such a routine needs:

| Alias                    | Meaning                                                |
| ------------------------ | ------------------------------------------------------ |
| `AddComplex<T>`          | the complex type at T's precision; the dual of `RemoveComplex` |
| `CommonPrecision<Ts...>` | `std::common_type_t` of the arguments' precisions       |
| `PromotedType<Ts...>`    | that common precision, made complex if any argument is  |

`PromotedType` is usually the right return type for a mixed computation:
`PromotedType<double, std::complex<float>>` is `std::complex<double>`.

@subsection design_cv Const, references and value types

One rule holds throughout: **top-level cv-qualification is removed before a
concept is applied, and references are not**. So `Real<const double>`,
`Complex<const std::complex<double>>` and `Field<const double>` are all
satisfied, while `Real<double&>` is not. Apply `std::remove_cvref_t` if the type
may be a reference.

`RemoveComplex` follows the same rule and strips cv from its result, so
`RemoveComplex<const std::complex<double>>` is `double` and
`SamePrecision<const double, double>` holds. Precision is a question about
`float` against `double`, not about constness.

The range and iterator concepts test `std::ranges::range_value_t` and
`std::iter_value_t`, which are already stripped of references and
cv-qualification. `RealRange<const std::vector<double>&>` is therefore
satisfied, while `RealWritableRange<const std::vector<double>&>` is not, which
is the intended behaviour.

@subsection design_open Closed and open concepts

`Real`, `Complex` and everything built on them are *closed*: a type satisfies
them only by being a built-in floating-point type or a `std::complex`
specialisation. That is what makes them cheap to check and precise to reason
about, and it is what most code wants.

It also shuts out scalar types that behave perfectly well — dual numbers for
automatic differentiation, interval arithmetic, extended precision, a quantity
carrying units. `Algebra.hpp` provides the *open* counterpart, describing a type
by the arithmetic it supports rather than by what it is:

| Concept              | Requires                                              |
| -------------------- | ------------------------------------------------------ |
| `AdditiveGroup<T>`   | `std::regular`, unary and binary `+`/`-`, `+=`, `-=`    |
| `Ring<T>`            | additionally `*` and `*=`                               |
| `Field<T>`           | additionally `/` and `/=`                               |
| `OrderedField<T>`    | a `Field` that is `std::totally_ordered`                |
| `ScalableBy<T, S>`   | `T` multipliable by `S` in either order, and `*=`       |
| `Scalar<T>`          | a `Field` with a real type, scalable by it              |
| `RealScalar<T>`      | a `Scalar` that is its own real type                    |
| `ComplexScalar<T>`   | a `Scalar` that is not                                  |

These are syntactic requirements named after the structures they model. They
cannot check the axioms: floating-point addition is not associative, and `int`
satisfies `Field` even though integer division is not a multiplicative inverse.
The names say which operations a template may use, which is what a constraint is
for.

A user-defined scalar joins the vocabulary by specialising one trait:

```cpp
template <NumericConcepts::Real T>
struct NumericConcepts::RealTypeTraits<Dual<T>> {
  using type = T;
};

static_assert(NumericConcepts::Scalar<Dual<double>>);
static_assert(std::same_as<NumericConcepts::RealType<Dual<double>>, double>);
```

`RealType<T>` is the open form of `RemoveComplex<T>`, and the two agree wherever
both are defined. Constrain on `RealOrComplex` when the algorithm depends on the
representation; constrain on `Scalar` when it only depends on the arithmetic.

@subsection design_version Version

`Version.hpp` defines `NUMERICCONCEPTS_VERSION_MAJOR`, `_MINOR` and `_PATCH`,
along with `NUMERICCONCEPTS_VERSION` and `NUMERICCONCEPTS_VERSION_NUMBER(a, b, c)`
for comparing them, so a dependent library can test which concepts exist:

```cpp
#if NUMERICCONCEPTS_VERSION >= NUMERICCONCEPTS_VERSION_NUMBER(0, 1, 0)
#include <NumericConcepts/Algebra.hpp>
#endif
```

The same numbers are available as `NumericConcepts::VersionMajor` and friends.
They are written by hand so that copying the `include` directory remains a
complete way to vendor the library; the build checks them against the version in
`CMakeLists.txt`, so the two cannot drift.

@section headers_sec The headers

| Header                            | Contents                                       |
| --------------------------------- | ---------------------------------------------- |
| `NumericConcepts/Numeric.hpp`     | Scalar categories, `RemoveComplex`, `AddComplex`, `PromotedType`, `SamePrecision` |
| `NumericConcepts/Algebra.hpp`     | `Field`, `Scalar`, `RealTypeTraits` and the open vocabulary |
| `NumericConcepts/Iterators.hpp`   | Iterator concepts, `IteratorValue`, `IteratorPrecision`, `SameIterator` |
| `NumericConcepts/Ranges.hpp`      | Range and view concepts, `RangeValue`, `RangePrecision`, `SameRange` |
| `NumericConcepts/Functions.hpp`   | Concepts on invocables and their return types  |
| `NumericConcepts/Version.hpp`     | Version macros and constants                   |
| `NumericConcepts/NumericConcepts.hpp` | Convenience header including all of the above |

Each header is self-contained and may be included on its own.

@section example_sec Examples

The `examples/` directory holds a graded set of programs, each covering one part
of the library. They assert as much as they print, and CTest runs them alongside
the unit tests.

| Example                  | Covers                                                          |
| ------------------------ | ---------------------------------------------------------------- |
| @ref 01-numeric_types.cpp | The scalar categories and compile-time dispatch between them    |
| @ref 02-precision.cpp     | `RemoveComplex`, `AddComplex`, `CommonPrecision`, `PromotedType` |
| @ref 03-iterators.cpp     | Algorithms over iterator pairs, and writable iterators           |
| @ref 04-ranges.cpp        | Whole-range constraints and output parameters                    |
| @ref 05-views.cpp         | The view concepts, and lazy pipelines                            |
| @ref 06-range_strength.cpp | `Sized`, `RandomAccess` and `Contiguous`, and when each is needed |
| @ref 07-functions.cpp     | Constraining callables by argument and return category           |
| @ref 08-scalars.cpp       | `Field`, `Scalar`, and admitting a user-defined scalar type      |

Several of them contain commented-out lines that would not compile.
Uncommenting one shows what a constraint actually buys: the error names the
concept that failed, at the call site, rather than somewhere inside a template
body.
