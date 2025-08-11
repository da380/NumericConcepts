#pragma once

#include <complex>
#include <concepts>

/**
 * @file Numeric.hpp
 * @brief Defines concepts for fundamental numeric types.
 */

namespace NumericConcepts {

/**
 * @brief Concept for integral types.
 * @tparam T The type to check.
 */
template <typename T>
concept Integral = std::integral<T>;

/**
 * @brief Concept for floating-point types.
 * @tparam T The type to check.
 */
template <typename T>
concept Real = std::floating_point<T>;

/**
 * @brief Concept for single-precision floating-point types (float).
 * @tparam T The type to check.
 */
template <typename T>
concept Float = std::same_as<T, float>;

/**
 * @brief Concept for double-precision floating-point types (double).
 * @tparam T The type to check.
 */
template <typename T>
concept Double = std::same_as<T, double>;

/**
 * @brief Concept for extended-precision floating-point types (long double).
 * @tparam T The type to check.
 */
template <typename T>
concept LongDouble = std::same_as<T, long double>;

/**
 * @internal
 * @brief Helper struct to determine if a type is a std::complex specialization.
 * @tparam T The type to check.
 */
template <typename T>
struct ComplexType : public std::false_type {};

/**
 * @internal
 * @brief Specialization of ComplexType for std::complex types.
 * @tparam T The underlying floating-point type.
 */
template <typename T>
struct ComplexType<std::complex<T>> : public std::bool_constant<Real<T>> {};

/**
 * @brief Concept for complex number types (specializations of std::complex).
 * @tparam T The type to check.
 */
template <typename T>
concept Complex = ComplexType<T>::value;

/**
 * @brief Concept for types that are either real (floating-point) or complex.
 * @tparam T The type to check.
 */
template <typename T>
concept RealOrComplex = Real<T> or Complex<T>;

/**
 * @brief Concept for any numeric type, i.e., integral, real, or complex.
 * @tparam T The type to check.
 */
template <typename T>
concept Numeric = Integral<T> or RealOrComplex<T>;

/**
 * @internal
 * @brief Helper struct to extract the underlying real type from a complex type.
 * If the type is not complex, the input type is returned.
 * @tparam T The RealOrComplex type.
 */
template <RealOrComplex T>
struct RemoveComplexHelper {
  using value_type = T;
};

/**
 * @internal
 * @brief Specialization of RemoveComplexHelper for std::complex types.
 * @tparam T The underlying floating-point type of the complex number.
 */
template <RealOrComplex T>
struct RemoveComplexHelper<std::complex<T>> {
  using value_type = T;
};

/**
 * @brief Extracts the underlying floating-point precision type from a
 * RealOrComplex type.
 * @details For a complex type `std::complex<T>`, this alias resolves to `T`.
 * For a real type `T`, this alias resolves to `T`.
 * @tparam T The RealOrComplex type.
 */
template <RealOrComplex T>
using RemoveComplex = typename RemoveComplexHelper<T>::value_type;

/**
 * @brief Concept to check if a list of RealOrComplex types have the same
 * underlying precision.
 * @details For example, `SamePrecision<double, std::complex<double>>` is true.
 * @tparam T The first type to compare.
 * @tparam Ts The other types to compare against the first.
 */
template <typename T, typename... Ts>
concept SamePrecision =
    (std::same_as<RemoveComplex<T>, RemoveComplex<Ts>> && ...);

}  // namespace NumericConcepts