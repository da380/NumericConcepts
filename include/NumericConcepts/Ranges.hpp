#pragma once

#include <ranges>
#include <type_traits>

#include "Numeric.hpp"

/**
 * @file Ranges.hpp
 * @brief Defines concepts for C++20 ranges over numeric types.
 */

namespace NumericConcepts {

/**
 * @brief Concept for an input range whose value type is integral.
 * @tparam T The range type to check.
 */
template <typename T>
concept IntegralRange = requires() {
  requires std::ranges::input_range<T>;
  requires Integral<std::ranges::range_value_t<T>>;
};

/**
 * @brief Concept for an input range whose value type is real (floating-point).
 * @tparam T The range type to check.
 */
template <typename T>
concept RealRange = requires() {
  requires std::ranges::input_range<T>;
  requires Real<std::ranges::range_value_t<T>>;
};

/**
 * @brief Concept for an input range whose value type is complex.
 * @tparam T The range type to check.
 */
template <typename T>
concept ComplexRange = requires() {
  requires std::ranges::input_range<T>;
  requires Complex<std::ranges::range_value_t<T>>;
};

/**
 * @brief Concept for an input range whose value type is real or complex.
 * @tparam T The range type to check.
 */
template <typename T>
concept RealOrComplexRange = RealRange<T> or ComplexRange<T>;

/**
 * @brief Concept for an input range whose value type is numeric.
 * @tparam T The range type to check.
 */
template <typename T>
concept NumericRange = IntegralRange<T> or RealOrComplexRange<T>;

/**
 * @brief Concept for a writable (output) range whose value type is integral.
 * @tparam T The range type to check.
 */
template <typename T>
concept IntegralWritableRange = requires() {
  requires std::ranges::output_range<T, std::ranges::range_value_t<T>>;
  requires Integral<std::ranges::range_value_t<T>>;
};

/**
 * @brief Concept for a writable (output) range whose value type is real.
 * @tparam T The range type to check.
 */
template <typename T>
concept RealWritableRange = requires() {
  requires std::ranges::output_range<T, std::ranges::range_value_t<T>>;
  requires Real<std::ranges::range_value_t<T>>;
};

/**
 * @brief Concept for a writable (output) range whose value type is complex.
 * @tparam T The range type to check.
 */
template <typename T>
concept ComplexWritableRange = requires() {
  requires std::ranges::output_range<T, std::ranges::range_value_t<T>>;
  requires Complex<std::ranges::range_value_t<T>>;
};

/**
 * @brief Concept for a writable (output) range whose value type is real or
 * complex.
 * @tparam T The range type to check.
 */
template <typename T>
concept RealOrComplexWritableRange =
    RealWritableRange<T> or ComplexWritableRange<T>;

/**
 * @brief Concept for a writable (output) range whose value type is numeric.
 * @tparam T The range type to check.
 */
template <typename T>
concept NumericWritableRange =
    IntegralWritableRange<T> or RealOrComplexWritableRange<T>;

/**
 * @name Output ranges
 *
 * The counterpart of the output iterators. The Writable concepts take the value
 * type from the range itself, so they ask "can this range be written through
 * with its own elements?". These take it as an argument, and ask "can a Value
 * be written into this range?", which is the more general question: a
 * `std::vector<double>` is a RealOutputRange for `float` as well as for
 * `double`.
 *
 * The category is tested against the value type with references and
 * cv-qualification removed, so the idiomatic `const T &` spelling — the one
 * `std::ranges::fill` and friends use — works as well as a plain `T`.
 * @{
 */

/**
 * @brief Concept for a range that a integral Value can be written into.
 * @tparam T The range type to check.
 * @tparam Value The type to be written into it.
 */
template <typename T, typename Value>
concept IntegralOutputRange = requires() {
  requires std::ranges::output_range<T, Value>;
  requires Integral<std::remove_cvref_t<Value>>;
};

/**
 * @brief Concept for a range that a real (floating-point) Value can be written
 * into.
 * @tparam T The range type to check.
 * @tparam Value The type to be written into it.
 */
template <typename T, typename Value>
concept RealOutputRange = requires() {
  requires std::ranges::output_range<T, Value>;
  requires Real<std::remove_cvref_t<Value>>;
};

/**
 * @brief Concept for a range that a complex Value can be written into.
 * @tparam T The range type to check.
 * @tparam Value The type to be written into it.
 */
template <typename T, typename Value>
concept ComplexOutputRange = requires() {
  requires std::ranges::output_range<T, Value>;
  requires Complex<std::remove_cvref_t<Value>>;
};

/**
 * @brief Concept for a range that a real or complex Value can be written into.
 * @tparam T The range type to check.
 * @tparam Value The type to be written into it.
 */
template <typename T, typename Value>
concept RealOrComplexOutputRange = requires() {
  requires std::ranges::output_range<T, Value>;
  requires RealOrComplex<std::remove_cvref_t<Value>>;
};

/**
 * @brief Concept for a range that a numeric Value can be written into.
 * @tparam T The range type to check.
 * @tparam Value The type to be written into it.
 */
template <typename T, typename Value>
concept NumericOutputRange = requires() {
  requires std::ranges::output_range<T, Value>;
  requires Numeric<std::remove_cvref_t<Value>>;
};

/** @} */

/**
 * @name Strength refinements
 *
 * The concepts above require only `std::ranges::input_range`, which is the
 * weakest useful requirement. Numerical code frequently needs more: a size
 * known up front, indexing, or contiguous storage to hand to a C or Fortran
 * routine. These refinements add exactly that, and nothing else.
 *
 * There are no view counterparts: a view is a range, so combine the refinement
 * with `std::ranges::view` directly where one is required.
 * @{
 */

/**
 * @brief Concept for a sized range whose size is known in constant time, whose
 * value type is integral.
 * @tparam T The range type to check.
 */
template <typename T>
concept IntegralSizedRange = requires() {
  requires IntegralRange<T>;
  requires std::ranges::sized_range<T>;
};

/**
 * @brief Concept for a sized range whose size is known in constant time, whose
 * value type is real (floating-point).
 * @tparam T The range type to check.
 */
template <typename T>
concept RealSizedRange = requires() {
  requires RealRange<T>;
  requires std::ranges::sized_range<T>;
};

/**
 * @brief Concept for a sized range whose size is known in constant time, whose
 * value type is complex.
 * @tparam T The range type to check.
 */
template <typename T>
concept ComplexSizedRange = requires() {
  requires ComplexRange<T>;
  requires std::ranges::sized_range<T>;
};

/**
 * @brief Concept for a sized range whose size is known in constant time, whose
 * value type is real or complex.
 * @tparam T The range type to check.
 */
template <typename T>
concept RealOrComplexSizedRange = requires() {
  requires RealOrComplexRange<T>;
  requires std::ranges::sized_range<T>;
};

/**
 * @brief Concept for a sized range whose size is known in constant time, whose
 * value type is numeric.
 * @tparam T The range type to check.
 */
template <typename T>
concept NumericSizedRange = requires() {
  requires NumericRange<T>;
  requires std::ranges::sized_range<T>;
};

/**
 * @brief Concept for a writable sized range whose size is known in constant
 * time, whose value type is integral.
 * @tparam T The range type to check.
 */
template <typename T>
concept IntegralWritableSizedRange = requires() {
  requires IntegralWritableRange<T>;
  requires std::ranges::sized_range<T>;
};

/**
 * @brief Concept for a writable sized range whose size is known in constant
 * time, whose value type is real (floating-point).
 * @tparam T The range type to check.
 */
template <typename T>
concept RealWritableSizedRange = requires() {
  requires RealWritableRange<T>;
  requires std::ranges::sized_range<T>;
};

/**
 * @brief Concept for a writable sized range whose size is known in constant
 * time, whose value type is complex.
 * @tparam T The range type to check.
 */
template <typename T>
concept ComplexWritableSizedRange = requires() {
  requires ComplexWritableRange<T>;
  requires std::ranges::sized_range<T>;
};

/**
 * @brief Concept for a writable sized range whose size is known in constant
 * time, whose value type is real or complex.
 * @tparam T The range type to check.
 */
template <typename T>
concept RealOrComplexWritableSizedRange = requires() {
  requires RealOrComplexWritableRange<T>;
  requires std::ranges::sized_range<T>;
};

/**
 * @brief Concept for a writable sized range whose size is known in constant
 * time, whose value type is numeric.
 * @tparam T The range type to check.
 */
template <typename T>
concept NumericWritableSizedRange = requires() {
  requires NumericWritableRange<T>;
  requires std::ranges::sized_range<T>;
};

/**
 * @brief Concept for a random-access range that can be indexed in constant
 * time, whose value type is integral.
 * @tparam T The range type to check.
 */
template <typename T>
concept IntegralRandomAccessRange = requires() {
  requires IntegralRange<T>;
  requires std::ranges::random_access_range<T>;
};

/**
 * @brief Concept for a random-access range that can be indexed in constant
 * time, whose value type is real (floating-point).
 * @tparam T The range type to check.
 */
template <typename T>
concept RealRandomAccessRange = requires() {
  requires RealRange<T>;
  requires std::ranges::random_access_range<T>;
};

/**
 * @brief Concept for a random-access range that can be indexed in constant
 * time, whose value type is complex.
 * @tparam T The range type to check.
 */
template <typename T>
concept ComplexRandomAccessRange = requires() {
  requires ComplexRange<T>;
  requires std::ranges::random_access_range<T>;
};

/**
 * @brief Concept for a random-access range that can be indexed in constant
 * time, whose value type is real or complex.
 * @tparam T The range type to check.
 */
template <typename T>
concept RealOrComplexRandomAccessRange = requires() {
  requires RealOrComplexRange<T>;
  requires std::ranges::random_access_range<T>;
};

/**
 * @brief Concept for a random-access range that can be indexed in constant
 * time, whose value type is numeric.
 * @tparam T The range type to check.
 */
template <typename T>
concept NumericRandomAccessRange = requires() {
  requires NumericRange<T>;
  requires std::ranges::random_access_range<T>;
};

/**
 * @brief Concept for a writable random-access range that can be indexed in
 * constant time, whose value type is integral.
 * @tparam T The range type to check.
 */
template <typename T>
concept IntegralWritableRandomAccessRange = requires() {
  requires IntegralWritableRange<T>;
  requires std::ranges::random_access_range<T>;
};

/**
 * @brief Concept for a writable random-access range that can be indexed in
 * constant time, whose value type is real (floating-point).
 * @tparam T The range type to check.
 */
template <typename T>
concept RealWritableRandomAccessRange = requires() {
  requires RealWritableRange<T>;
  requires std::ranges::random_access_range<T>;
};

/**
 * @brief Concept for a writable random-access range that can be indexed in
 * constant time, whose value type is complex.
 * @tparam T The range type to check.
 */
template <typename T>
concept ComplexWritableRandomAccessRange = requires() {
  requires ComplexWritableRange<T>;
  requires std::ranges::random_access_range<T>;
};

/**
 * @brief Concept for a writable random-access range that can be indexed in
 * constant time, whose value type is real or complex.
 * @tparam T The range type to check.
 */
template <typename T>
concept RealOrComplexWritableRandomAccessRange = requires() {
  requires RealOrComplexWritableRange<T>;
  requires std::ranges::random_access_range<T>;
};

/**
 * @brief Concept for a writable random-access range that can be indexed in
 * constant time, whose value type is numeric.
 * @tparam T The range type to check.
 */
template <typename T>
concept NumericWritableRandomAccessRange = requires() {
  requires NumericWritableRange<T>;
  requires std::ranges::random_access_range<T>;
};

/**
 * @brief Concept for a contiguous range whose elements are stored contiguously,
 * whose value type is integral.
 * @tparam T The range type to check.
 */
template <typename T>
concept IntegralContiguousRange = requires() {
  requires IntegralRange<T>;
  requires std::ranges::contiguous_range<T>;
};

/**
 * @brief Concept for a contiguous range whose elements are stored contiguously,
 * whose value type is real (floating-point).
 * @tparam T The range type to check.
 */
template <typename T>
concept RealContiguousRange = requires() {
  requires RealRange<T>;
  requires std::ranges::contiguous_range<T>;
};

/**
 * @brief Concept for a contiguous range whose elements are stored contiguously,
 * whose value type is complex.
 * @tparam T The range type to check.
 */
template <typename T>
concept ComplexContiguousRange = requires() {
  requires ComplexRange<T>;
  requires std::ranges::contiguous_range<T>;
};

/**
 * @brief Concept for a contiguous range whose elements are stored contiguously,
 * whose value type is real or complex.
 * @tparam T The range type to check.
 */
template <typename T>
concept RealOrComplexContiguousRange = requires() {
  requires RealOrComplexRange<T>;
  requires std::ranges::contiguous_range<T>;
};

/**
 * @brief Concept for a contiguous range whose elements are stored contiguously,
 * whose value type is numeric.
 * @tparam T The range type to check.
 */
template <typename T>
concept NumericContiguousRange = requires() {
  requires NumericRange<T>;
  requires std::ranges::contiguous_range<T>;
};

/**
 * @brief Concept for a writable contiguous range whose elements are stored
 * contiguously, whose value type is integral.
 * @tparam T The range type to check.
 */
template <typename T>
concept IntegralWritableContiguousRange = requires() {
  requires IntegralWritableRange<T>;
  requires std::ranges::contiguous_range<T>;
};

/**
 * @brief Concept for a writable contiguous range whose elements are stored
 * contiguously, whose value type is real (floating-point).
 * @tparam T The range type to check.
 */
template <typename T>
concept RealWritableContiguousRange = requires() {
  requires RealWritableRange<T>;
  requires std::ranges::contiguous_range<T>;
};

/**
 * @brief Concept for a writable contiguous range whose elements are stored
 * contiguously, whose value type is complex.
 * @tparam T The range type to check.
 */
template <typename T>
concept ComplexWritableContiguousRange = requires() {
  requires ComplexWritableRange<T>;
  requires std::ranges::contiguous_range<T>;
};

/**
 * @brief Concept for a writable contiguous range whose elements are stored
 * contiguously, whose value type is real or complex.
 * @tparam T The range type to check.
 */
template <typename T>
concept RealOrComplexWritableContiguousRange = requires() {
  requires RealOrComplexWritableRange<T>;
  requires std::ranges::contiguous_range<T>;
};

/**
 * @brief Concept for a writable contiguous range whose elements are stored
 * contiguously, whose value type is numeric.
 * @tparam T The range type to check.
 */
template <typename T>
concept NumericWritableContiguousRange = requires() {
  requires NumericWritableRange<T>;
  requires std::ranges::contiguous_range<T>;
};

/** @} */

/**
 * @brief Concept for a view whose value type is integral.
 * @tparam T The view type to check.
 */
template <typename T>
concept IntegralView = IntegralRange<T> and std::ranges::view<T>;

/**
 * @brief Concept for a view whose value type is real.
 * @tparam T The view type to check.
 */
template <typename T>
concept RealView = RealRange<T> and std::ranges::view<T>;

/**
 * @brief Concept for a view whose value type is complex.
 * @tparam T The view type to check.
 */
template <typename T>
concept ComplexView = ComplexRange<T> and std::ranges::view<T>;

/**
 * @brief Concept for a view whose value type is real or complex.
 * @tparam T The view type to check.
 */
template <typename T>
concept RealOrComplexView = RealOrComplexRange<T> and std::ranges::view<T>;

/**
 * @brief Concept for a view whose value type is numeric.
 * @tparam T The view type to check.
 */
template <typename T>
concept NumericView = NumericRange<T> and std::ranges::view<T>;

/**
 * @brief Concept for a writable view whose value type is integral.
 * @tparam T The view type to check.
 */
template <typename T>
concept IntegralWritableView =
    IntegralWritableRange<T> and std::ranges::view<T>;

/**
 * @brief Concept for a writable view whose value type is real.
 * @tparam T The view type to check.
 */
template <typename T>
concept RealWritableView = RealWritableRange<T> and std::ranges::view<T>;

/**
 * @brief Concept for a writable view whose value type is complex.
 * @tparam T The view type to check.
 */
template <typename T>
concept ComplexWritableView = ComplexWritableRange<T> and std::ranges::view<T>;

/**
 * @brief Concept for a writable view whose value type is real or complex.
 * @tparam T The view type to check.
 */
template <typename T>
concept RealOrComplexWritableView =
    RealOrComplexWritableRange<T> and std::ranges::view<T>;

/**
 * @brief Concept for a writable view whose value type is numeric.
 * @tparam T The view type to check.
 */
template <typename T>
concept NumericWritableView = NumericWritableRange<T> and std::ranges::view<T>;

/**
 * @brief Concept to check if a list of ranges have the same value type.
 * @tparam T The first range type to compare.
 * @tparam Ts The other range types to compare.
 */
template <typename T, typename... Ts>
concept SameRangeValueType = requires() {
  requires std::ranges::range<T> && (std::ranges::range<Ts> && ...);
  requires(std::same_as<std::ranges::range_value_t<T>,
                        std::ranges::range_value_t<Ts>> &&
           ...);
};

/**
 * @brief Type alias to get the value type of a range.
 * @details A spelling of `std::ranges::range_value_t` that mirrors
 * IteratorValue, so that the range and iterator vocabularies read the same way.
 * @tparam T The range type.
 */
template <std::ranges::range T>
using RangeValue = std::ranges::range_value_t<T>;

/**
 * @brief Type alias to get the underlying precision of a real or complex range.
 * @tparam T The RealOrComplexRange type.
 */
template <RealOrComplexRange T>
using RangePrecision = RemoveComplex<std::ranges::range_value_t<T>>;

/**
 * @brief Concept to check if a list of real or complex ranges are the exact
 * same type.
 * @tparam T The first range type to compare.
 * @tparam Ts The other range types to compare.
 */
template <typename T, typename... Ts>
concept SameRange = requires() {
  requires RealOrComplexRange<T> && (RealOrComplexRange<Ts> && ...);
  requires(std::same_as<T, Ts> && ...);
};

/**
 * @brief Concept to check if a list of real or complex ranges have the same
 * underlying precision.
 * @tparam T The first range type to compare.
 * @tparam Ts The other range types to compare.
 */
template <typename T, typename... Ts>
concept SameRangePrecision = requires() {
  requires RealOrComplexRange<T> && (RealOrComplexRange<Ts> && ...);
  requires(std::same_as<RangePrecision<T>, RangePrecision<Ts>> && ...);
};

}  // namespace NumericConcepts
