#include <gtest/gtest.h>

#include <NumericConcepts/NumericConcepts.hpp>
#include <complex>
#include <vector>

using namespace NumericConcepts;

TEST(RangeTests, RangeConcepts) {
  using IntVec = std::vector<int>;
  using DoubleVec = std::vector<double>;
  using ComplexVec = std::vector<std::complex<float>>;

  static_assert(IntegralRange<IntVec>);
  static_assert(RealRange<DoubleVec>);
  static_assert(ComplexRange<ComplexVec>);
  static_assert(NumericRange<IntVec>);
  static_assert(RealOrComplexRange<ComplexVec>);
}

TEST(RangeTests, WritableRangeConcepts) {
  using IntVec = std::vector<int>;
  static_assert(IntegralWritableRange<IntVec>);
  static_assert(NumericWritableRange<IntVec>);
}

TEST(RangeTests, PrecisionConcepts) {
  using FloatVec = std::vector<float>;
  using DoubleVec = std::vector<double>;
  using ComplexFloatVec = std::vector<std::complex<float>>;

  static_assert(SameRangePrecision<FloatVec, ComplexFloatVec>);
  static_assert(!SameRangePrecision<FloatVec, DoubleVec>);
}