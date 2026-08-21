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
  static_assert(!Numeric<char *>);
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

TEST(NumericTests, PrecisionSpecificConcepts) {
  static_assert(Float<float>);
  static_assert(Double<double>);
  static_assert(LongDouble<long double>);

  static_assert(ComplexFloat<std::complex<float>>);
  static_assert(ComplexDouble<std::complex<double>>);
  static_assert(ComplexLongDouble<std::complex<long double>>);

  static_assert(!ComplexFloat<std::complex<double>>);
  static_assert(!ComplexDouble<double>);
}

TEST(NumericTests, CvQualification) {
  // Top-level cv-qualification is removed before a scalar concept is applied,
  // so the real and complex sides behave alike.
  static_assert(Real<const double>);
  static_assert(Complex<const std::complex<double>>);
  static_assert(Float<const float>);
  static_assert(ComplexDouble<volatile std::complex<double>>);
  static_assert(RealOrComplex<const std::complex<float>>);

  static_assert(std::is_same_v<RemoveComplex<const double>, double>);
  static_assert(
      std::is_same_v<RemoveComplex<const std::complex<double>>, double>);
  static_assert(SamePrecision<const double, double>);

  // References are not stripped.
  static_assert(!Real<double &>);
  static_assert(!Complex<std::complex<double> &>);
}

TEST(NumericTests, AddComplex) {
  static_assert(std::is_same_v<AddComplex<double>, std::complex<double>>);
  static_assert(std::is_same_v<AddComplex<float>, std::complex<float>>);
  static_assert(
      std::is_same_v<AddComplex<std::complex<float>>, std::complex<float>>);
  // The dual of RemoveComplex.
  static_assert(std::is_same_v<RemoveComplex<AddComplex<double>>, double>);
}

TEST(NumericTests, CommonPrecisionAndPromotion) {
  static_assert(std::is_same_v<CommonPrecision<float>, float>);
  static_assert(std::is_same_v<CommonPrecision<float, double>, double>);
  static_assert(
      std::is_same_v<CommonPrecision<float, std::complex<double>>, double>);
  static_assert(std::is_same_v<CommonPrecision<float, float, float>, float>);

  static_assert(std::is_same_v<PromotedType<double, double>, double>);
  static_assert(std::is_same_v<PromotedType<float, float>, float>);
  static_assert(std::is_same_v<PromotedType<double, std::complex<float>>,
                               std::complex<double>>);
  static_assert(std::is_same_v<PromotedType<std::complex<float>, float>,
                               std::complex<float>>);
  static_assert(std::is_same_v<PromotedType<float>, float>);
}

TEST(NumericTests, RemoveComplexHelperStillWorks) {
  // Retained for compatibility with code written against the original name.
  static_assert(
      std::is_same_v<RemoveComplexHelper<std::complex<double>>::value_type,
                     double>);
  static_assert(std::is_same_v<RemoveComplexHelper<float>::value_type, float>);
}
