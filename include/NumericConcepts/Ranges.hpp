#pragma once

#include "Numeric.hpp"

namespace NumericConcepts {

// Concepts for ranges.
template <typename T>
concept IntegralRange = requires() {
  requires std::ranges::input_range<T>;
  requires Integral<std::ranges::range_value_t<T>>;
};

template <typename T>
concept RealRange = requires() {
  requires std::ranges::input_range<T>;
  requires Real<std::ranges::range_value_t<T>>;
};

template <typename T>
concept ComplexRange = requires() {
  requires std::ranges::input_range<T>;
  requires Complex<std::ranges::range_value_t<T>>;
};

template <typename T>
concept RealOrComplexRange = RealRange<T> or ComplexRange<T>;

template <typename T>
concept NumericRange = IntegralRange<T> or RealOrComplexRange<T>;

// Concepts for ranges whose values can be written to.
template <typename T>
concept IntegralWritableRange = requires() {
  requires std::ranges::output_range<T, std::ranges::range_value_t<T>>;
  requires Integral<std::ranges::range_value_t<T>>;
};

template <typename T>
concept RealWritableRange = requires() {
  requires std::ranges::output_range<T, std::ranges::range_value_t<T>>;
  requires Real<std::ranges::range_value_t<T>>;
};

template <typename T>
concept ComplexWritableRange = requires() {
  requires std::ranges::output_range<T, std::ranges::range_value_t<T>>;
  requires Complex<std::ranges::range_value_t<T>>;
};

template <typename T>
concept RealOrComplexWritableRange =
    RealWritableRange<T> or ComplexWritableRange<T>;

template <typename T>
concept NumericWritableRange =
    IntegralWritableRange<T> or RealOrComplexWritableRange<T>;

// Concepts for views.
template <typename T>
concept IntegralView = IntegralRange<T> and std::ranges::view<T>;

template <typename T>
concept RealView = RealRange<T> and std::ranges::view<T>;

template <typename T>
concept ComplexView = ComplexRange<T> and std::ranges::view<T>;

template <typename T>
concept RealOrComplexView = RealOrComplexRange<T> and std::ranges::view<T>;

template <typename T>
concept NumericView = NumericRange<T> and std::ranges::view<T>;

// Concepts for views whose values can be written to.
template <typename T>
concept IntegralWritableView =
    IntegralWritableRange<T> and std::ranges::view<T>;

template <typename T>
concept RealWritableView = RealWritableRange<T> and std::ranges::view<T>;

template <typename T>
concept ComplexWritableView = ComplexWritableRange<T> and std::ranges::view<T>;

template <typename T>
concept RealOrComplexWritableView =
    RealOrComplexWritableRange<T> and std::ranges::view<T>;

template <typename T>
concept NumericWritableView = NumericWritableRange<T> and std::ranges::view<T>;

// Concept to check if a list of ranges have the same value type.
template <typename T, typename... Ts>
concept SameRangeValueType = requires() {
  requires std::ranges::range<T> && (std::ranges::range<Ts> && ...);
  requires(std::same_as<std::ranges::range_value_t<T>,
                        std::ranges::range_value_t<Ts>> &&
           ...);
};

// Return the precision or a real or complex range.
template <RealOrComplexRange T>
using RangePrecision = RemoveComplex<std::ranges::range_value_t<T>>;

// Concept to check if a list of real or complex ranges have the same precision.
template <typename T, typename... Ts>
concept SameRangePrecision = requires() {
  requires RealOrComplexRange<T> && (RealOrComplexRange<Ts> && ...);
  requires(std::same_as<RangePrecision<T>, RangePrecision<Ts>> && ...);
};

} // namespace NumericConcepts