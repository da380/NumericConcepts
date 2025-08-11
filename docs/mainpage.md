@mainpage NumericConcepts Library

@section intro_sec Introduction

Welcome to the **NumericConcepts** library!

This is a header-only C++20 library that provides a cohesive set of concepts for generic programming with numeric types. It leverages the power of C++ concepts to make template code safer, more readable, and easier to debug by providing clear, compile-time constraints for functions and classes that operate on numbers, iterators, and ranges.

The main goal is to simplify template metaprogramming when dealing with various numeric representations like integers, floating-point numbers (`float`, `double`), and complex numbers.

***

@section features_sec Core Features

The library is organized into several key areas of functionality:

### Numeric Type Concepts (`Numeric.hpp`)

These form the foundation of the library, allowing you to constrain templates to specific kinds of numbers.

* **Basic Types**: Concepts like `Integral`, `Real`, and `Complex` check for standard integer, floating-point, and `std::complex` types.
* **Compound Types**: Broader concepts like `RealOrComplex` and `Numeric` allow for more flexibility.
* **Precision Helpers**: Utilities like `RemoveComplex` allow you to extract the underlying floating-point type from a `RealOrComplex` type, and `SamePrecision` can check if multiple types share the same precision (e.g., `double` and `std::complex<double>`).

### Iterator Concepts (`Iterators.hpp`)

Constrain your algorithms to iterators that point to specific numeric types.

* **Read-only Iterators**: Concepts like `RealIterator` and `ComplexIterator` ensure an iterator's `value_type` is a numeric type.
* **Writable Iterators**: Concepts like `RealWritableIterator` ensure you can write specific numeric types to the iterator's position.
* **Precision Checking**: `SameIteratorPrecision` lets you verify that multiple iterators, although potentially of different types (e.g., `double*` and `std::vector<std::complex<double>>::iterator`), point to values with the same underlying floating-point precision.

### Range & View Concepts (`Ranges.hpp`)

Work with modern C++20 ranges and views in a type-safe way.

* **Numeric Ranges**: Concepts such as `IntegralRange`, `RealRange`, and `RealOrComplexRange` constrain any type that models `std::ranges::input_range` to contain specific numeric values.
* **Writable Ranges**: Concepts like `RealWritableRange` ensure the range is an `output_range` for a given numeric type.
* **Views**: The library provides parallel concepts specifically for views (e.g., `RealView`, `NumericWritableView`) by combining range concepts with `std::ranges::view`.

### Function Concepts (`Functions.hpp`)

Constrain callable types (like lambdas or function objects) based on their return value.

* **Return Type Constraints**: Concepts like `RealFunction` and `NumericFunction` check that an invocable returns a value of a specific numeric category.

***

@section usage_sec Getting Started

### Installation with CMake & `FetchContent`

For seamless integration into your project, it's recommended to use **CMake** and its `FetchContent` module. This allows CMake to download and configure the library for you automatically.

Add the following to your `CMakeLists.txt`:

```cmake
# CMakeLists.txt

# Require CMake 3.14 or higher for FetchContent
cmake_minimum_required(VERSION 3.14)
project(MyAwesomeProject CXX)

# Set the C++ standard to 20 or later
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include the FetchContent module to manage dependencies
include(FetchContent)

# Declare the location of the NumericConcepts library
FetchContent_Declare(
  NumericConcepts
  GIT_REPOSITORY  "https://github.com/da380/NumericConcepts"
  GIT_TAG        "main"
)

# Make the library's targets available to your project
FetchContent_MakeAvailable(NumericConcepts)

# Add your own executable
add_executable(MyAwesomeApp main.cpp)

# Link your app against the NumericConcepts library.
# As a header-only library, it provides an INTERFACE target.
target_link_libraries(MyAwesomeApp PRIVATE NumericConcepts)
```