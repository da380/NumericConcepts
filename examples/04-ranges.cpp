/**
 * @file 04-ranges.cpp
 * @brief Constraining algorithms on whole ranges rather than iterator pairs.
 *
 * @details The range concepts are the ones most code should reach for. They say
 * what a sequence contains without saying how it is stored, so the same routine
 * takes a vector, a list, an array or a view. Output parameters are expressed
 * with the Writable forms, which reject a range whose elements cannot be
 * assigned to.
 */

#include <NumericConcepts/Ranges.hpp>
#include <array>
#include <complex>
#include <iostream>
#include <list>
#include <span>
#include <string>
#include <vector>

using namespace NumericConcepts;

/**
 * @brief Prints a range, labelled by the category of its values.
 * @details Constrained on NumericRange, so it accepts any sequence of numbers
 * and nothing else. The `if constexpr` chain picks the description at compile
 * time from the same concepts used in example 01.
 * @tparam R The range type.
 * @param range The range to print.
 * @param name A label for the range.
 */
template <NumericRange R>
void Print(const R &range, const std::string &name) {
  if constexpr (IntegralRange<R>) {
    std::cout << "integral ";
  } else if constexpr (RealRange<R>) {
    std::cout << "real     ";
  } else if constexpr (ComplexRange<R>) {
    std::cout << "complex  ";
  }
  std::cout << name << " = [ ";
  for (const auto &value : range) std::cout << value << " ";
  std::cout << "]\n";
}

/**
 * @brief Fills a range with a constant.
 * @details The Writable form is the whole point here: passing a
 * `const std::vector<double>&`, or a `std::span<const double>`, fails to
 * compile, because neither can be assigned through. RangeValue names the type
 * of the value to write.
 * @tparam R The range type.
 * @param range The range to fill.
 * @param value The value to write into every element.
 */
template <NumericWritableRange R>
void Fill(R &&range, RangeValue<R> value) {
  for (auto &element : range) element = value;
}

/**
 * @brief Adds one range into another, element by element.
 * @details The output is real or complex and writable; the input need only be
 * readable. SameRangePrecision allows a real range to be accumulated into a
 * complex one, while rejecting a mismatch of precision.
 * @tparam Out The type of the range accumulated into.
 * @tparam In The type of the range read from.
 * @param out The range to add into.
 * @param in The range to add.
 */
template <RealOrComplexWritableRange Out, RealOrComplexRange In>
requires SameRangePrecision<Out, In>
void AddInto(Out &&out, const In &in) {
  auto i = std::ranges::begin(in);
  for (auto &element : out) element += *i++;
}

int main() {
  const auto integers = std::vector<int>{1, 2, 3};
  const auto reals = std::list<double>{1.5, 2.5, 3.5};
  const auto complexes =
      std::array<std::complex<float>, 2>{{{1.0f, 1.0f}, {2.0f, -2.0f}}};

  Print(integers, "vector<int>       ");
  Print(reals, "list<double>      ");
  Print(complexes, "array<complex<f>> ");

  // Print(std::vector<std::string>{"a"}, "strings") would not compile: a range
  // of strings is not a NumericRange.

  auto buffer = std::vector<double>(4);
  Fill(buffer, 1.0);
  Print(buffer, "filled            ");

  // A real range added into a complex one: same precision, different value
  // types, which is exactly the case SameRangePrecision is meant to allow.
  auto accumulator = std::vector<std::complex<double>>(3);
  AddInto(accumulator, std::vector<double>{1.0, 2.0, 3.0});
  AddInto(accumulator, std::vector<std::complex<double>>{
                           {0.0, 1.0}, {0.0, 2.0}, {0.0, 3.0}});
  Print(accumulator, "accumulated       ");

  // Writability follows the element type, not the container. A span over const
  // doubles is a perfectly good RealRange but is not writable.
  static_assert(RealWritableRange<std::vector<double>>);
  static_assert(RealWritableRange<std::span<double>>);
  static_assert(RealRange<std::span<const double>>);
  static_assert(!RealWritableRange<std::span<const double>>);
  static_assert(!RealWritableRange<const std::vector<double> &>);

  // SameRangeValueType is the stricter sibling of SameRangePrecision, for
  // routines that genuinely need identical element types.
  static_assert(SameRangeValueType<std::vector<double>, std::list<double>>);
  static_assert(!SameRangeValueType<std::vector<double>,
                                    std::vector<std::complex<double>>>);
  static_assert(SameRangePrecision<std::vector<double>,
                                   std::vector<std::complex<double>>>);

  return 0;
}
