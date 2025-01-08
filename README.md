# Numeric Concepts

A collection of c++20 concepts for working with numeric types, which is to say integers, reals and complex numbers. 

The aim of the library is to collect a number of useful concepts in one place for inclusion in other projects. Some of the concepts are direct re-namings of those within the standard Concept Library, while the others are simple combinations thereof. Nonetheless, these are things I have found useful repeatedly.


## Installation

The library is header only and has no dependencies. 

It can be built and installed using CMake as follows:

- Clone the repository:
```
git clone https://github.com/da380/NumericConcepts.git
```

- Configure the build:
```
cd NumericConcepts
cmake -S . -B build
```
A prefix for the installation can be set through the variable ```-DCMAKE_INSTALL_PREFIX```. If this variable is not set, 
the default GNU install location will be used. 

- Build and install the library:
```
cmake --build build --target install
```

## Inclusion in another CMake project

The library is best included using CMake's  Fetch_Content method. An example of this can be seen within the application directory.

## Acknowledgements

The CMake files within this project are based on the examples provided in https://github.com/pr0g/cmake-examples.git