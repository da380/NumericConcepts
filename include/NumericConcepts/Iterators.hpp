#pragma once

#include <iterator>

#include "Numeric.hpp"

namespace NumericConcepts {

// Iterator concepts
template <typename T>
concept Iterator = std::input_iterator<T>;

template <typename T>
concept IntegralIterator = Iterator<T> && Integral<std::iter_value_t<T>>;

template <typename T>
concept RealIterator = Iterator<T> && Real<std::iter_value_t<T>>;

template <typename T>
concept ComplexIterator = Iterator<T> && Complex<std::iter_value_t<T>>;

template <typename T>
concept RealOrComplexIterator =
    Iterator<T> && RealOrComplex<std::iter_value_t<T>>;

template <typename T>
concept NumericIterator = Iterator<T> && Numeric<std::iter_value_t<T>>;

// Writeable iterator concepts
template <typename T>
concept WritableIterator = std::output_iterator<T, std::iter_value_t<T>>;

template <typename T>
concept IntegralWritableIterator =
    WritableIterator<T> && Integral<std::iter_value_t<T>>;

template <typename T>
concept RealWritableIterator =
    WritableIterator<T> && Real<std::iter_value_t<T>>;

template <typename T>
concept ComplexWritableIterator =
    WritableIterator<T> && Complex<std::iter_value_t<T>>;

template <typename T>
concept RealOrComplexWritableIterator =
    WritableIterator<T> && RealOrComplex<std::iter_value_t<T>>;

template <typename T>
concept NumericWritableIterator =
    WritableIterator<T> && Numeric<std::iter_value_t<T>>;

// Return the precision or a real or complex iterator.
template <RealOrComplexIterator T>
using IteratorPrecision = RemoveComplex<std::iter_value_t<T>>;

// Check if a list of real or complex iterators are the same.
template <typename T, typename... Ts>
concept SameIterator = requires() {
  requires RealOrComplexIterator<T> && (RealOrComplexIterator<Ts> && ...);
  requires(std::same_as<T, Ts> && ...);
};

// Check if a list of real or complex iterators have the same precision.
template <typename T, typename... Ts>
concept SameIteratorPrecision = requires() {
  requires RealOrComplexIterator<T> && (RealOrComplexIterator<Ts> && ...);
  requires(std::same_as<IteratorPrecision<T>, IteratorPrecision<Ts>> && ...);
};

}  // namespace NumericConcepts