#include <gtest/gtest.h>

#include <NumericConcepts/NumericConcepts.hpp>
#include <complex>
#include <list>
#include <ranges>
#include <span>
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

TEST(RangeTests, StrengthRefinements) {
  using DoubleVec = std::vector<double>;
  using DoubleList = std::list<double>;
  using ComplexVec = std::vector<std::complex<double>>;

  // A vector is contiguous, and therefore also random-access and sized.
  static_assert(RealContiguousRange<DoubleVec>);
  static_assert(RealRandomAccessRange<DoubleVec>);
  static_assert(RealSizedRange<DoubleVec>);
  static_assert(ComplexContiguousRange<ComplexVec>);
  static_assert(NumericContiguousRange<DoubleVec>);
  static_assert(RealOrComplexRandomAccessRange<ComplexVec>);

  // A list is sized, but neither random-access nor contiguous.
  static_assert(RealSizedRange<DoubleList>);
  static_assert(!RealRandomAccessRange<DoubleList>);
  static_assert(!RealContiguousRange<DoubleList>);

  // The value type still has to match.
  static_assert(!ComplexContiguousRange<DoubleVec>);
  static_assert(!IntegralSizedRange<DoubleVec>);

  // The refinements are refinements: each implies the plain concept.
  static_assert(RealRange<DoubleVec>);
}

TEST(RangeTests, WritableStrengthRefinements) {
  using DoubleVec = std::vector<double>;

  static_assert(RealWritableContiguousRange<DoubleVec>);
  static_assert(RealWritableRandomAccessRange<DoubleVec>);
  static_assert(RealWritableSizedRange<DoubleVec>);

  // Const-ness is respected, exactly as for the plain writable concepts.
  static_assert(!RealWritableContiguousRange<const DoubleVec &>);
  static_assert(RealContiguousRange<const DoubleVec &>);

  // A span over const elements is contiguous but not writable.
  static_assert(RealContiguousRange<std::span<const double>>);
  static_assert(!RealWritableContiguousRange<std::span<const double>>);
  static_assert(RealWritableContiguousRange<std::span<double>>);
}

TEST(RangeTests, ViewsComposeWithRefinements) {
  // There are no view forms of the refinements; a view is a range, so the two
  // are combined at the point of use.
  using Span = std::span<double>;
  static_assert(RealView<Span>);
  static_assert(RealContiguousRange<Span> && std::ranges::view<Span>);
}

TEST(RangeTests, RangeValue) {
  static_assert(std::is_same_v<RangeValue<std::vector<double>>, double>);
  static_assert(
      std::is_same_v<RangeValue<const std::vector<double> &>, double>);
  static_assert(std::is_same_v<RangeValue<std::vector<std::complex<float>>>,
                               std::complex<float>>);
}

TEST(RangeTests, OutputRangeConcepts) {
  using DoubleVec = std::vector<double>;

  static_assert(RealOutputRange<DoubleVec &, double>);
  static_assert(RealWritableRange<DoubleVec>);

  // The value type is given rather than taken from the range, so writing a
  // float into a range of doubles is expressible; the Writable form cannot say
  // this, because it always uses the range's own value type.
  static_assert(RealOutputRange<DoubleVec &, float>);

  // Constness is respected exactly as for the Writable forms.
  static_assert(!RealOutputRange<const DoubleVec &, double>);
  static_assert(!RealOutputRange<std::span<const double>, double>);
  static_assert(RealOutputRange<std::span<double>, double>);

  // The category applies to the value written, not to the range.
  static_assert(!RealOutputRange<DoubleVec &, int>);
  static_assert(!ComplexOutputRange<DoubleVec &, double>);
  static_assert(IntegralOutputRange<std::vector<int> &, int>);
  static_assert(ComplexOutputRange<std::vector<std::complex<double>> &,
                                   std::complex<double>>);
  static_assert(NumericOutputRange<DoubleVec &, double>);
  static_assert(RealOrComplexOutputRange<DoubleVec &, double>);

  // The const T & spelling used by std::ranges::fill and friends works.
  static_assert(RealOutputRange<DoubleVec &, const double &>);
}
