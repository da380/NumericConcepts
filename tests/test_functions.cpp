#include <gtest/gtest.h>

#include <NumericConcepts/NumericConcepts.hpp>

using namespace NumericConcepts;

// A sample function for testing
double sample_real_func(int, float) { return 0.0; }

TEST(FunctionTests, FunctionConcepts) {
  auto int_lambda = [](double) -> int { return 1; };
  auto double_lambda = []() { return 3.14; };
  auto complex_lambda = [](int) -> std::complex<float> { return {1, 1}; };

  // Test specific return types
  static_assert(IntegralFunction<decltype(int_lambda), double>);
  static_assert(RealFunction<decltype(double_lambda)>);
  static_assert(ComplexFunction<decltype(complex_lambda), int>);

  // Test function pointers
  static_assert(RealFunction<decltype(&sample_real_func), int, float>);

  // Test general concepts
  static_assert(RealOrComplexFunction<decltype(double_lambda)>);
  static_assert(NumericFunction<decltype(int_lambda), double>);
}

TEST(FunctionTests, GenericFunctionConcept) {
  auto short_lambda = []() -> short { return 1; };
  // Test convertibility: short is convertible to int
  static_assert(Function<decltype(short_lambda), int>);
  // Test non-convertibility (compile error if uncommented)
  // static_assert(Function<decltype(short_lambda), float>);
}