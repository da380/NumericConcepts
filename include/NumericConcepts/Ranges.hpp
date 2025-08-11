#pragma once

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