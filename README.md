# Numeric Concepts

A collection of c++20 concepts for working with numeric types, which is to say integers, reals and complex numbers. 

The aim of the library is to collect a number of useful concepts in one place for inclusion in other projects. Some of the concepts are direct re-namings of those within the standard Concept Library, while the others are simple combinations thereof. Nonetheless, these are things I have found useful repeatedly.

## Examples

The aim is to help working with numeric types along with containers of such types. By a numeric type, I mean integers, real and complex numbers. 

As a first example, suppose you have a templated function of a single argument:
```c++
template <typename T>
auto f(T i) { return i+1;}
```
if it is required that the type of the argument be integral, we can instead write:
```c++
template <NumericConcepts::Integral T>
auto f(T i) {return std::abs(i);}
```
In this case, the concept is just a renaming of std::integral from the Concepts Library, and so little is actually gained. The use of this library is collecting and combining together a number of such concepts.
For example, suppose our function can take in any numeric type, we could 
then write 
```c++
template <NumericConcepts::Numeric T>
auto f(T i) {return std::abs(i);}
```

A second example relates to complex numbers (implemented as instances of ```std::complex```) for which, currently, there are not pre-defined concepts in the standard library. First, we can require that a templated function is restricted to complex types:
```c++
template <NumericConcepts::Complex T>
auto f(T i) {return std::arg(i);}
```
We can also extract the base type from a ```std::complex``` variable
which, for example, allows use to check that arithmetic operations are 
well-defined:
```c++
template<NumericConcepts::Real T, NumericConcepts::Complex S>
requires std::same_as<T, NumericConcepts::RemoveComplex<S>>
auto f(T a, S x) {return a * s;}
```

Next, we can consider containers of numeric types. Suppose we 
have a templated function that acts on ranges over real numbers. 
This can be enforced by use of ```NumericConcepts::RealRange```, while analogous 
concepts are defined for other numeric types along with views and viewable ranges. 
As an example, consider a function that takes in a numeric view and doubles its values:
```c++
template <NumericWritableView V>
void f(V view) {    
    for(auto& val : view) {
        val *= static_cast<std::ranges::range_value_t<V>>(2);
    }
}
```



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

If the library has been installed localled, then it can be 
located using the find_package method. Alternatively, it 
can be included as a dependency using FetchContent. Given the
small size and header-only nature of the library, the latter 
method will likely be the most convenient. 

Within the application directory a simple CMake project that uses the library is shown. Both methods of inclusion implemented, with 
the option used set by the variable logical ```-DLibraryInstalled```.

## Acknowledgements

The CMake files within this project are based on the examples provided in https://github.com/pr0g/cmake-examples.git