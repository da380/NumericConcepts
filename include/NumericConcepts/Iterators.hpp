#pragma once

#include <iterator>
#include <type_traits>

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
 * @name Output iterators
 *
 * The Writable concepts above describe a *mutable iterator into existing
 * storage*: they take the value type from the iterator itself, via
 * `std::iter_value_t`. That is what a numerical kernel writes its results
 * through, but it excludes the pure output iterators — `std::ostream_iterator`,
 * `std::back_insert_iterator` and their kind — whose value type is `void`,
 * leaving nothing to categorise.
 *
 * These concepts name the value type explicitly instead, so they accept both.
 * Use them when an algorithm knows what it produces and does not care where it
 * goes:
 *
 * ```cpp
 * template <RealIterator In, typename Out>
 *   requires RealOutputIterator<Out, IteratorValue<In>>
 * void Transform(In first, In last, Out out);
 * ```
 *
 * The category is tested against the value type with references and
 * cv-qualification removed, so the idiomatic `const T &` spelling works as
 * well as a plain `T`.
 * @{
 */

/**
 * @brief Concept for an iterator that a Value can be written to.
 * @details Unlike WritableIterator, the value type is given rather than taken
 * from the iterator, so an iterator with no value type of its own qualifies.
 * @tparam T The iterator type to check.
 * @tparam Value The type to be written through it.
 */
template <typename T, typename Value>
concept OutputIterator = std::output_iterator<T, Value>;

/**
 * @brief Concept for an iterator that a integral Value can be written to.
 * @tparam T The iterator type to check.
 * @tparam Value The type to be written through it.
 */
template <typename T, typename Value>
concept IntegralOutputIterator =
    OutputIterator<T, Value> && Integral<std::remove_cvref_t<Value>>;

/**
 * @brief Concept for an iterator that a real (floating-point) Value can be
 * written to.
 * @tparam T The iterator type to check.
 * @tparam Value The type to be written through it.
 */
template <typename T, typename Value>
concept RealOutputIterator =
    OutputIterator<T, Value> && Real<std::remove_cvref_t<Value>>;

/**
 * @brief Concept for an iterator that a complex Value can be written to.
 * @tparam T The iterator type to check.
 * @tparam Value The type to be written through it.
 */
template <typename T, typename Value>
concept ComplexOutputIterator =
    OutputIterator<T, Value> && Complex<std::remove_cvref_t<Value>>;

/**
 * @brief Concept for an iterator that a real or complex Value can be written
 * to.
 * @tparam T The iterator type to check.
 * @tparam Value The type to be written through it.
 */
template <typename T, typename Value>
concept RealOrComplexOutputIterator =
    OutputIterator<T, Value> && RealOrComplex<std::remove_cvref_t<Value>>;

/**
 * @brief Concept for an iterator that a numeric Value can be written to.
 * @tparam T The iterator type to check.
 * @tparam Value The type to be written through it.
 */
template <typename T, typename Value>
concept NumericOutputIterator =
    OutputIterator<T, Value> && Numeric<std::remove_cvref_t<Value>>;

/** @} */

/**
 * @brief Concept for a type that satisfies the std::random_access_iterator
 * requirements.
 * @tparam T The type to check.
 */
template <typename T>
concept RandomAccessIterator = std::random_access_iterator<T>;

/**
 * @brief Concept for a type that satisfies the std::contiguous_iterator
 * requirements.
 * @tparam T The type to check.
 */
template <typename T>
concept ContiguousIterator = std::contiguous_iterator<T>;

/**
 * @brief Concept for a random-access iterator whose value type is integral.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept IntegralRandomAccessIterator =
    RandomAccessIterator<T> && Integral<std::iter_value_t<T>>;

/**
 * @brief Concept for a random-access iterator whose value type is real
 * (floating-point).
 * @tparam T The iterator type to check.
 */
template <typename T>
concept RealRandomAccessIterator =
    RandomAccessIterator<T> && Real<std::iter_value_t<T>>;

/**
 * @brief Concept for a random-access iterator whose value type is complex.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept ComplexRandomAccessIterator =
    RandomAccessIterator<T> && Complex<std::iter_value_t<T>>;

/**
 * @brief Concept for a random-access iterator whose value type is real or
 * complex.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept RealOrComplexRandomAccessIterator =
    RandomAccessIterator<T> && RealOrComplex<std::iter_value_t<T>>;

/**
 * @brief Concept for a random-access iterator whose value type is numeric.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept NumericRandomAccessIterator =
    RandomAccessIterator<T> && Numeric<std::iter_value_t<T>>;

/**
 * @brief Concept for a writable random-access iterator whose value type is
 * integral.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept IntegralWritableRandomAccessIterator =
    RandomAccessIterator<T> && WritableIterator<T> &&
    Integral<std::iter_value_t<T>>;

/**
 * @brief Concept for a writable random-access iterator whose value type is real
 * (floating-point).
 * @tparam T The iterator type to check.
 */
template <typename T>
concept RealWritableRandomAccessIterator =
    RandomAccessIterator<T> && WritableIterator<T> &&
    Real<std::iter_value_t<T>>;

/**
 * @brief Concept for a writable random-access iterator whose value type is
 * complex.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept ComplexWritableRandomAccessIterator =
    RandomAccessIterator<T> && WritableIterator<T> &&
    Complex<std::iter_value_t<T>>;

/**
 * @brief Concept for a writable random-access iterator whose value type is real
 * or complex.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept RealOrComplexWritableRandomAccessIterator =
    RandomAccessIterator<T> && WritableIterator<T> &&
    RealOrComplex<std::iter_value_t<T>>;

/**
 * @brief Concept for a writable random-access iterator whose value type is
 * numeric.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept NumericWritableRandomAccessIterator =
    RandomAccessIterator<T> && WritableIterator<T> &&
    Numeric<std::iter_value_t<T>>;

/**
 * @brief Concept for a contiguous iterator whose value type is integral.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept IntegralContiguousIterator =
    ContiguousIterator<T> && Integral<std::iter_value_t<T>>;

/**
 * @brief Concept for a contiguous iterator whose value type is real
 * (floating-point).
 * @tparam T The iterator type to check.
 */
template <typename T>
concept RealContiguousIterator =
    ContiguousIterator<T> && Real<std::iter_value_t<T>>;

/**
 * @brief Concept for a contiguous iterator whose value type is complex.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept ComplexContiguousIterator =
    ContiguousIterator<T> && Complex<std::iter_value_t<T>>;

/**
 * @brief Concept for a contiguous iterator whose value type is real or complex.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept RealOrComplexContiguousIterator =
    ContiguousIterator<T> && RealOrComplex<std::iter_value_t<T>>;

/**
 * @brief Concept for a contiguous iterator whose value type is numeric.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept NumericContiguousIterator =
    ContiguousIterator<T> && Numeric<std::iter_value_t<T>>;

/**
 * @brief Concept for a writable contiguous iterator whose value type is
 * integral.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept IntegralWritableContiguousIterator =
    ContiguousIterator<T> && WritableIterator<T> &&
    Integral<std::iter_value_t<T>>;

/**
 * @brief Concept for a writable contiguous iterator whose value type is real
 * (floating-point).
 * @tparam T The iterator type to check.
 */
template <typename T>
concept RealWritableContiguousIterator =
    ContiguousIterator<T> && WritableIterator<T> && Real<std::iter_value_t<T>>;

/**
 * @brief Concept for a writable contiguous iterator whose value type is
 * complex.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept ComplexWritableContiguousIterator =
    ContiguousIterator<T> && WritableIterator<T> &&
    Complex<std::iter_value_t<T>>;

/**
 * @brief Concept for a writable contiguous iterator whose value type is real or
 * complex.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept RealOrComplexWritableContiguousIterator =
    ContiguousIterator<T> && WritableIterator<T> &&
    RealOrComplex<std::iter_value_t<T>>;

/**
 * @brief Concept for a writable contiguous iterator whose value type is
 * numeric.
 * @tparam T The iterator type to check.
 */
template <typename T>
concept NumericWritableContiguousIterator =
    ContiguousIterator<T> && WritableIterator<T> &&
    Numeric<std::iter_value_t<T>>;

/**
 * @brief Type alias to get the value type of an iterator.
 * @details A spelling of `std::iter_value_t` that mirrors RangeValue, so that
 * the iterator and range vocabularies read the same way.
 * @tparam T The iterator type.
 */
template <Iterator T>
using IteratorValue = std::iter_value_t<T>;

/**
 * @brief Type alias to get the underlying precision of a real or complex
 * iterator's value type.
 * @tparam T The RealOrComplexIterator type.
 */
template <RealOrComplexIterator T>
using IteratorPrecision = RemoveComplex<std::iter_value_t<T>>;

/**
 * @brief Concept to check if a list of iterators have the same value type.
 * @details The counterpart of SameRangeValueType. The iterators themselves may
 * differ in type; only their value types must agree.
 * @tparam T The first iterator type to compare.
 * @tparam Ts The other iterator types to compare.
 */
template <typename T, typename... Ts>
concept SameIteratorValueType = requires() {
  requires Iterator<T> && (Iterator<Ts> && ...);
  requires(std::same_as<std::iter_value_t<T>, std::iter_value_t<Ts>> && ...);
};

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
