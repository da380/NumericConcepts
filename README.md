# NumericConcepts

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)

A header-only C++20 library providing a cohesive set of concepts for generic programming with numeric types. `NumericConcepts` makes your template code safer, more readable, and easier to debug by providing clear, compile-time constraints.

---

## Features

-   **Type Concepts**: Constrain templates to `Integral`, `Real`, `Complex`, `RealOrComplex`, and `Numeric` types.
-   **Iterator Concepts**: Type-safe concepts for both read-only and writable iterators over numeric types.
-   **Range & View Concepts**: Modern C++20 concepts for numeric ranges and views, like `RealRange` and `ComplexWritableView`.
-   **Function Concepts**: Constrain callables based on their numeric return types (e.g., `RealFunction`).

---

## Requirements

-   A **C++20** compatible compiler (e.g., GCC 10+, Clang 12+, MSVC v19.29+).
-   **CMake** 3.14+ (for the recommended installation method).

---

## Installation

The recommended way to integrate this library is with CMake's `FetchContent`. Add the following to your `CMakeLists.txt`:

```cmake
include(FetchContent)

FetchContent_Declare(
  NumericConcepts
  GIT_REPOSITORY "[https://github.com/your-org/NumericConcepts.git](https://github.com/your-org/NumericConcepts.git)" #<-- Replace with the actual repository URL
  GIT_TAG        "main" #<-- Or a specific version/tag like v1.0.0
)

FetchContent_MakeAvailable(NumericConcepts)

# Link to your target
target_link_libraries(YourApp PRIVATE NumericConcepts)