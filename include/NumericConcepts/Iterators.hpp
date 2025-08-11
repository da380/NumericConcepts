#pragma once

#include <iterator>

#include "Numeric.hpp"

/**
 * @file Iterators.hpp
 * @brief Defines concepts for iterators over numeric types.
 */

namespace NumericConcepts {

/**
 * @brief Concept for a type that satisfies the std::input_iterator
 * requirements.
 * @tparam T The type to check.
 */
template <typename T>
concept Iterator = std::input_iterator<T>;

/**
 * @brief Concept for an iterator whose value type is integral.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept IntegralIterator = Iterator<T> && Integral<std::iter_value_t<T>>;

/**
 * @brief Concept for an iterator whose value type is real (floating-point).
 * @tparam T The iterator type to check.
 */
template <typename T>
concept RealIterator = Iterator<T> && Real<std::iter_value_t<T>>;

/**
 * @brief Concept for an iterator whose value type is complex.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept ComplexIterator = Iterator<T> && Complex<std::iter_value_t<T>>;

/**
 * @brief Concept for an iterator whose value type is real or complex.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept RealOrComplexIterator =
    Iterator<T> && RealOrComplex<std::iter_value_t<T>>;

/**
 * @brief Concept for an iterator whose value type is numeric.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept NumericIterator = Iterator<T> && Numeric<std::iter_value_t<T>>;

/**
 * @brief Concept for a type that is a writable iterator.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept WritableIterator = std::output_iterator<T, std::iter_value_t<T>>;

/**
 * @brief Concept for a writable iterator whose value type is integral.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept IntegralWritableIterator =
    WritableIterator<T> && Integral<std::iter_value_t<T>>;

/**
 * @brief Concept for a writable iterator whose value type is real.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept RealWritableIterator =
    WritableIterator<T> && Real<std::iter_value_t<T>>;

/**
 * @brief Concept for a writable iterator whose value type is complex.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept ComplexWritableIterator =
    WritableIterator<T> && Complex<std::iter_value_t<T>>;

/**
 * @brief Concept for a writable iterator whose value type is real or complex.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept RealOrComplexWritableIterator =
    WritableIterator<T> && RealOrComplex<std::iter_value_t<T>>;

/**
 * @brief Concept for a writable iterator whose value type is numeric.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept NumericWritableIterator =
    WritableIterator<T> && Numeric<std::iter_value_t<T>>;

/**
 * @brief Type alias to get the underlying precision of a real or complex
 * iterator's value type.
 * @tparam T The RealOrComplexIterator type.
 */
template <RealOrComplexIterator T>
using IteratorPrecision = RemoveComplex<std::iter_value_t<T>>;

/**
 * @brief Concept to check if a list of iterators are of the exact same type.
 * @tparam T The first iterator type to compare.
 * @tparam Ts The other iterator types to compare.
 */
template <typename T, typename... Ts>
concept SameIterator = requires() {
  requires RealOrComplexIterator<T> && (RealOrComplexIterator<Ts> && ...);
  requires(std::same_as<T, Ts> && ...);
};

/**
 * @brief Concept to check if a list of real or complex iterators have the same
 * underlying precision.
 * @tparam T The first iterator type to compare.
 * @tparam Ts The other iterator types to compare.
 */
template <typename T, typename... Ts>
concept SameIteratorPrecision = requires() {
  requires RealOrComplexIterator<T> && (RealOrComplexIterator<Ts> && ...);
  requires(std::same_as<IteratorPrecision<T>, IteratorPrecision<Ts>> && ...);
};

}  // namespace NumericConcepts