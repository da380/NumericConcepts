#pragma once

#include "Numeric.hpp"

namespace NC {

// Concepts for ranges.
template <typename T>
concept IntegralRange = requires() {
  requires std::ranges::range<T>;
  requires Integral<std::ranges::range_value_t<T>>;
};

template <typename T>
concept RealRange = requires() {
  requires std::ranges::range<T>;
  requires Real<std::ranges::range_value_t<T>>;
};

template <typename T>
concept ComplexRange = requires() {
  requires std::ranges::range<T>;
  requires Complex<std::ranges::range_value_t<T>>;
};

template <typename T>
concept RealOrComplexRange = RealRange<T> or ComplexRange<T>;

template <typename T>
concept NumericRange = IntegralRange<T> or RealOrComplexRange<T>;

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

// Concepts for viewable ranges.
template <typename T>
concept IntegralViewableRange =
    IntegralRange<T> and std::ranges::viewable_range<T>;

template <typename T>
concept RealViewableRange = RealRange<T> and std::ranges::viewable_range<T>;

template <typename T>
concept ComplexViewableRange =
    ComplexRange<T> and std::ranges::viewable_range<T>;

template <typename T>
concept RealOrComplexViewableRange =
    RealOrComplexRange<T> and std::ranges::viewable_range<T>;

template <typename T>
concept NumericViewableRange =
    NumericRange<T> and std::ranges::viewable_range<T>;

} // namespace NC