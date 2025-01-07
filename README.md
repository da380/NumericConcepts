# NC

A collection of c++20 concepts for working with numeric types, which is to say integers, reals and complex numbers. 

## Installation

The library is header only and has no dependencies. 

It can be built and installed using CMake as follows:

- Clone the repository:
```
git clone https://github.com/da380/NC.git
```

- Configure the build:
```
cd NC
cmake -S . -B build
```
A prefix for the installation can be set using -DCMAKE_INSTALL_PREFIX=install. If this is not set, 
the default GNU install location will be used. 

- Build and install the library:
```
cmake --build build --target install
```

## Inclusion in another CMake project

If the library has been installed localled, then it can be 
located using the find_package method. Alternatively, it 
can be included as a dependency using FetchContent. Given the
small size and header-only nature of the library, the latter 
method will likely be the most convenient. 

Within the application directory a simple CMAKE project that uses the library is shown, with both methods of inclusion shown. 

## Acknowledgements

The CMake files within this project are based on the templated provided by https://github.com/pr0g/cmake-examples.git