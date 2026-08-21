/**
 * @file 01-numeric_types.cpp
 * @brief The scalar categories: Integral, Real, Complex and their unions.
 *
 * @details Every other concept in the library is built on these five, so this
 * is the place to start. They are *closed*: a type satisfies them by being a
 * built-in arithmetic type or a `std::complex` specialisation, and by nothing
 * else. Example 08 shows the open counterpart.
 */

#include <NumericConcepts/Numeric.hpp>
#include <NumericConcepts/Version.hpp>
#include <complex>
#include <iostream>
#include <string>

using namespace NumericConcepts;

/**
 * @brief Names the category a numeric type falls into.
 * @details The categories overlap — every Real is also a RealOrComplex and a
 * Numeric — so `if constexpr` tests them from the most specific outwards. This
 * is the usual shape of a routine that must do something different for real and
 * complex data.
 * @tparam T The numeric type to describe.
 * @return The name of the narrowest category T belongs to.
 */
template <Numeric T>
std::string Describe() {
  if constexpr (Integral<T>) {
    return "integral";
  } else if constexpr (Float<T>) {
    return "real, single precision";
  } else if constexpr (Double<T>) {
    return "real, double precision";
  } else if constexpr (LongDouble<T>) {
    return "real, extended precision";
  } else if constexpr (ComplexFloat<T>) {
    return "complex, single precision";
  } else if constexpr (ComplexDouble<T>) {
    return "complex, double precision";
  } else {
    return "numeric";
  }
}

int main() {
  std::cout << "NumericConcepts " << VersionString << "\n\n";

  std::cout << "int                  : " << Describe<int>() << "\n"
            << "float                : " << Describe<float>() << "\n"
            << "double               : " << Describe<double>() << "\n"
            << "complex<float>       : " << Describe<std::complex<float>>()
            << "\n"
            << "complex<double>      : " << Describe<std::complex<double>>()
            << "\n";

  // The categories nest as you would expect.
  static_assert(Integral<int>);
  static_assert(Real<double>);
  static_assert(Complex<std::complex<double>>);
  static_assert(RealOrComplex<double> and RealOrComplex<std::complex<double>>);
  static_assert(Numeric<int> and Numeric<double> and
                Numeric<std::complex<float>>);

  // An integer is Numeric but not RealOrComplex: the distinction matters
  // wherever a routine needs a floating-point precision to work at.
  static_assert(!RealOrComplex<int>);

  // Complex is only over floating-point types. std::complex<int> is a valid
  // C++ type but not a numeric one for these purposes, and the library says so
  // rather than letting it through.
  static_assert(!Complex<std::complex<int>>);

  // Nothing else qualifies, which is the point: Describe<std::string>() and
  // Describe<double*>() would both fail to compile.
  static_assert(!Numeric<std::string>);
  static_assert(!Numeric<double *>);

  // Top-level const and volatile are removed before a concept is applied;
  // references are not. Apply std::remove_cvref_t if a type may be a reference.
  static_assert(Real<const double>);
  static_assert(Complex<const std::complex<double>>);
  static_assert(!Real<double &>);

  return 0;
}
