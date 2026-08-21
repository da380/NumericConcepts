/**
 * @file 06-range_strength.cpp
 * @brief Asking for indexing, a known size, or contiguous storage.
 *
 * @details The plain range concepts require only `std::ranges::input_range`,
 * the weakest useful requirement. Numerical code often needs more, and the
 * strength refinements say which "more" without the caller writing the same
 * few lines each time. The pattern is
 * `Category [Writable] [Strength] Range`, with Strength one of Sized,
 * RandomAccess or Contiguous.
 */

#include <NumericConcepts/Ranges.hpp>
#include <deque>
#include <forward_list>
#include <iostream>
#include <list>
#include <numeric>
#include <ranges>
#include <span>
#include <vector>

using namespace NumericConcepts;

/**
 * @brief The arithmetic mean of a sequence.
 * @details Needs the number of elements before it can divide, so it asks for a
 * Sized range. A `std::list` qualifies; an unbounded generator does not.
 * @tparam R The range type.
 * @param range The range to average.
 * @return The mean of the elements.
 */
template <RealSizedRange R>
RangeValue<R> Mean(const R &range) {
  auto sum = RangeValue<R>{};
  for (const auto &value : range) sum += value;
  return sum / static_cast<RangeValue<R>>(std::ranges::size(range));
}

/**
 * @brief Linear interpolation of tabulated data at a point.
 * @details Indexes both sequences, so it asks for RandomAccess. Written against
 * a list it would still compile if the constraint were merely RealRange, but
 * would be quadratic; the constraint makes the requirement explicit and the
 * error immediate.
 * @tparam XRange The type of the abscissae.
 * @tparam YRange The type of the ordinates.
 * @param x The abscissae, in increasing order.
 * @param y The ordinates.
 * @param xi The point at which to interpolate.
 * @return The interpolated value.
 */
template <RealRandomAccessRange XRange, RealRandomAccessRange YRange>
requires SameRangePrecision<XRange, YRange>
RangeValue<YRange> Interpolate(const XRange &x, const YRange &y,
                               RangePrecision<XRange> xi) {
  auto i = std::size_t{0};
  while (i + 2 < std::ranges::size(x) && x[i + 1] < xi) ++i;
  const auto t = (xi - x[i]) / (x[i + 1] - x[i]);
  return y[i] + t * (y[i + 1] - y[i]);
}

/**
 * @brief Stands in for a C or Fortran routine taking a pointer and a length.
 * @param data A pointer to the first element.
 * @param n The number of elements.
 * @return The sum of the elements.
 */
double LegacySum(const double *data, std::size_t n) {
  return std::accumulate(data, data + n, 0.0);
}

/**
 * @brief Hands a sequence to a routine that requires contiguous storage.
 * @details This is the case where the constraint is not a matter of efficiency
 * but of correctness: `std::ranges::data` exists only for a contiguous range,
 * so nothing weaker can be passed through. Asking for it in the signature turns
 * what would be a confusing error inside the body into a clear one at the call.
 * @tparam R The range type.
 * @param range The range to pass on.
 * @return The result from the legacy routine.
 */
template <RealContiguousRange R>
double SumViaLegacyApi(const R &range) {
  return LegacySum(std::ranges::data(range), std::ranges::size(range));
}

int main() {
  const auto vector = std::vector<double>{1.0, 2.0, 3.0, 4.0};
  const auto list = std::list<double>{1.0, 2.0, 3.0, 4.0};
  const auto deque = std::deque<double>{1.0, 2.0, 3.0, 4.0};

  // Every container here is sized, so all three can be averaged.
  std::cout << "mean(vector) = " << Mean(vector) << "\n"
            << "mean(list)   = " << Mean(list) << "\n"
            << "mean(deque)  = " << Mean(deque) << "\n";

  // A deque is random-access but not contiguous; a list is neither.
  const auto x = std::vector<double>{0.0, 1.0, 2.0};
  const auto y = std::deque<double>{0.0, 10.0, 40.0};
  std::cout << "interp(0.5)  = " << Interpolate(x, y, 0.5) << "\n"
            << "interp(1.5)  = " << Interpolate(x, y, 1.5) << "\n";

  // Only the contiguous containers reach the legacy routine.
  std::cout << "legacy(vec)  = " << SumViaLegacyApi(vector) << "\n"
            << "legacy(span) = " << SumViaLegacyApi(std::span(vector)) << "\n";

  // Mean(std::views::iota(1))          — not sized
  // Interpolate(std::list{...}, y, 0.5) — not random-access
  // SumViaLegacyApi(deque)              — not contiguous
  //
  // The table below is what each container actually models.
  static_assert(RealSizedRange<std::vector<double>>);
  static_assert(RealRandomAccessRange<std::vector<double>>);
  static_assert(RealContiguousRange<std::vector<double>>);

  static_assert(RealSizedRange<std::deque<double>>);
  static_assert(RealRandomAccessRange<std::deque<double>>);
  static_assert(!RealContiguousRange<std::deque<double>>);

  static_assert(RealSizedRange<std::list<double>>);
  static_assert(!RealRandomAccessRange<std::list<double>>);

  static_assert(RealRange<std::forward_list<double>>);
  static_assert(!RealSizedRange<std::forward_list<double>>);

  // The strengths apply to every category, and to the writable forms.
  static_assert(ComplexContiguousRange<std::vector<std::complex<double>>>);
  static_assert(IntegralRandomAccessRange<std::vector<int>>);
  static_assert(NumericSizedRange<std::list<int>>);
  static_assert(RealWritableContiguousRange<std::span<double>>);
  static_assert(!RealWritableContiguousRange<std::span<const double>>);

  // There are no view forms; combine with std::ranges::view where needed.
  static_assert(RealContiguousRange<std::span<double>> &&
                std::ranges::view<std::span<double>>);

  return 0;
}
