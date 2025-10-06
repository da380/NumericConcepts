#include <gtest/gtest.h>

#include <NumericConcepts/NumericConcepts.hpp>
#include <complex>

using namespace NumericConcepts;

TEST(NumericTests, TypeConcepts) {
  // Test Integral
  static_assert(Integral<int>);
  static_assert(!Integral<double>);

  // Test Real
  static_assert(Real<float>);
  static_assert(Real<double>);
  static_assert(!Real<int>);

  // Test Complex
  static_assert(Complex<std::complex<float>>);
  static_assert(!Complex<double>);

  // Test RealOrComplex
  static_assert(RealOrComplex<double>);
  static_assert(RealOrComplex<std::complex<float>>);
  static_assert(!RealOrComplex<int>);

  // Test Numeric
  static_assert(Numeric<int>);
  static_assert(Numeric<float>);
  static_assert(Numeric<std::complex<double>>);
  static_assert(!Numeric<char*>);
}

TEST(NumericTests, UtilityConcepts) {
  // Test RemoveComplex
  static_assert(std::is_same_v<RemoveComplex<double>, double>);
  static_assert(std::is_same_v<RemoveComplex<std::complex<float>>, float>);

  // Test SamePrecision
  static_assert(SamePrecision<double, std::complex<double>>);
  static_assert(SamePrecision<float, std::complex<float>, float>);
  static_assert(!SamePrecision<float, double>);
  static_assert(!SamePrecision<float, std::complex<double>>);
}