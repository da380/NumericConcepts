# Examples

Each example is a complete program that both prints something and asserts what
it claims, so the set doubles as a second test suite — CTest runs them all
alongside the unit tests.

Build and run them with:

```bash
cmake -S . -B build -DNUMERICCONCEPTS_BUILD_TESTS=ON
cmake --build build
./build/examples/01-numeric_types
```

Read them in order; each assumes the one before it.

| Example                  | Header          | Covers                                                                 |
| ------------------------ | --------------- | ---------------------------------------------------------------------- |
| `01-numeric_types.cpp`   | `Numeric.hpp`   | The scalar categories, compile-time dispatch between them, the cv rule  |
| `02-precision.cpp`       | `Numeric.hpp`   | `RemoveComplex`, `AddComplex`, `SamePrecision`, `CommonPrecision`, `PromotedType` |
| `03-iterators.cpp`       | `Iterators.hpp` | Algorithms over iterator pairs, writable iterators, precision agreement |
| `04-ranges.cpp`          | `Ranges.hpp`    | Whole-range constraints, output parameters, value-type agreement        |
| `05-views.cpp`           | `Ranges.hpp`    | The view concepts, and lazy pipelines that stay constrained             |
| `06-range_strength.cpp`  | `Ranges.hpp`    | `Sized`, `RandomAccess` and `Contiguous`, and when each is needed       |
| `07-functions.cpp`       | `Functions.hpp` | Constraining callables by argument and return category                  |
| `08-scalars.cpp`         | `Algebra.hpp`   | `Field` and `Scalar`, and admitting a user-defined scalar type          |

`08-scalars.cpp` also shows the `Version.hpp` macros used as a feature test.

Several of the examples contain commented-out lines that would *not* compile.
Uncommenting one is the quickest way to see what a constraint actually buys:
the error names the concept that failed, at the call site, rather than
appearing somewhere inside a template body.
