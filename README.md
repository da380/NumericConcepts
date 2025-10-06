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
-   **CMake** 3.15+ (for the recommended installation method).

---

## Installation

You can integrate `NumericConcepts` into your project in two ways. Using `find_package` with a pre-installed version is recommended for managing dependencies cleanly.

### Method 1: `find_package` (Recommended)

This method allows you to build and install the library once and then link to it from any project. Note that this because this is 
a header-only library, the build step is trivial, while install just copies the header files to the desired location. 

**1. Build and Install `NumericConcepts`**

First, clone and install the library. You can specify a local install path using `-DCMAKE_INSTALL_PREFIX`.

```bash
# Clone the repository
git clone https://github.com/da380/NumericConcepts.git
cd NumericConcepts

# Configure and build
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=~/local

# Install
cmake --install build
```

**2. Link from your project**

In your project's `CMakeLists.txt`, use find_package to locate the library and then link to the imported target.

```cmake
# In your CMakeLists.txt
find_package(NumericConcepts REQUIRED)

target_link_libraries(YourApp INTERFACE NumericConcepts::NumericConcepts)
```

If you installed `NumericConcepts` to a custom location (like `~/local` above), you must tell CMake where to look for it. You can do this in one of the following ways:

- **Option A (Command Line):** Pass the install prefix to CMake when you configure your project. This is the most common method.

```bash
# Tell CMake where to find dependencies
cmake -S . -B build -DCMAKE_PREFIX_PATH=~/local
```

- **Option B (Environment Variable):** Set the `CMAKE_PREFIX_PATH` environment variable before running CMake.

```bash
export CMAKE_PREFIX_PATH=~/local
cmake -S . -B build
```


**Note on Managing `CMAKE_PREFIX_PATH`**

The export command overwrites the variable. If you need to add multiple, unrelated paths, separate them with a colon (`:`) on Linux/macOS or a semicolon (`;`) on Windows.

```bash
# Search in ~/local first, then /opt/custom-libs
export CMAKE_PREFIX_PATH="~/local:/opt/custom-libs"
```
To add a new path without losing the existing value, you can prepend it like this:

```bash
# Prepend a new path to the existing CMAKE_PREFIX_PATH
export CMAKE_PREFIX_PATH="/path/to/new/lib:${CMAKE_PREFIX_PATH}"
```

### Method 2: `FetchContent`

This method downloads and builds the library as part of your project's own configuration step. It is convenient but less ideal for managing shared dependencies.

Add the following to your `CMakeLists.txt`:

```cmake
include(FetchContent)

FetchContent_Declare(
  NumericConcepts
  GIT_REPOSITORY "https://github.com/da380/NumericConcepts.git"
  GIT_TAG        "main"                                           
)

FetchContent_MakeAvailable(NumericConcepts)

# Link to your target
target_link_libraries(YourApp INTERFACE NumericConcepts)
```

## Documentation

See the [wiki](https://github.com/da380/NumericConcepts/wiki) page for detail of the library and examples of its use. 
