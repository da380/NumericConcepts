#pragma once

#include <complex>
#include <concepts>
#include <type_traits>

/**
 * @file Numeric.hpp
 * @brief Defines concepts for fundamental numeric types.
 *
 * @details The concepts in this file are applied to a type after any top-level
 * cv-qualification is removed, so that `Real<const double>` and
 * `Complex<const std::complex<double>>` behave alike. References are not
 * stripped; apply `std::remove_cvref_t` if the type may be a reference.
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
concept Float = std::same_as<std::remove_cv_t<T>, float>;

/**
 * @brief Concept for double-precision floating-point types (double).
 * @tparam T The type to check.
 */
template <typename T>
concept Double = std::same_as<std::remove_cv_t<T>, double>;

/**
 * @brief Concept for extended-precision floating-point types (long double).
 * @tparam T The type to check.
 */
template <typename T>
concept LongDouble = std::same_as<std::remove_cv_t<T>, long double>;

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
concept Complex = ComplexType<std::remove_cv_t<T>>::value;

/**
 * @brief Concept for single-precision complex types (std::complex<float>).
 * @tparam T The type to check.
 */
template <typename T>
concept ComplexFloat = std::same_as<std::remove_cv_t<T>, std::complex<float>>;

/**
 * @brief Concept for double-precision complex types (std::complex<double>).
 * @tparam T The type to check.
 */
template <typename T>
concept ComplexDouble = std::same_as<std::remove_cv_t<T>, std::complex<double>>;

/**
 * @brief Concept for extended-precision complex types
 * (std::complex<long double>).
 * @tparam T The type to check.
 */
template <typename T>
concept ComplexLongDouble =
    std::same_as<std::remove_cv_t<T>, std::complex<long double>>;

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
 * @brief Implementation of RemoveComplex, applied to a cv-unqualified type.
 * @tparam T The type whose underlying precision is wanted.
 */
template <typename T>
struct RemoveComplexImpl {
  using type = T;
};

/**
 * @internal
 * @brief Specialization of RemoveComplexImpl for std::complex types.
 * @tparam T The underlying floating-point type of the complex number.
 */
template <typename T>
struct RemoveComplexImpl<std::complex<T>> {
  using type = T;
};

/**
 * @brief Extracts the underlying floating-point precision type from a
 * RealOrComplex type.
 * @details For a complex type `std::complex<T>`, this alias resolves to `T`.
 * For a real type `T`, this alias resolves to `T`. Top-level cv-qualification
 * is removed, so that `RemoveComplex<const double>` is `double`.
 * @tparam T The RealOrComplex type.
 */
template <RealOrComplex T>
using RemoveComplex = typename RemoveComplexImpl<std::remove_cv_t<T>>::type;

/**
 * @brief The complex type built on the precision of a RealOrComplex type.
 * @details `AddComplex<double>` and `AddComplex<std::complex<double>>` are both
 * `std::complex<double>`. This is the dual of RemoveComplex, and names the
 * complex result type of an operation on a possibly-real input.
 * @tparam T The RealOrComplex type.
 */
template <RealOrComplex T>
using AddComplex = std::complex<RemoveComplex<T>>;

/**
 * @internal
 * @brief Retained for compatibility; prefer the RemoveComplex alias.
 * @tparam T The RealOrComplex type.
 */
template <RealOrComplex T>
struct RemoveComplexHelper {
  using value_type = RemoveComplex<T>;
};

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

/**
 * @brief The common floating-point precision of a list of RealOrComplex types.
 * @details Equal to `std::common_type_t` of the underlying precisions, so
 * `CommonPrecision<float, std::complex<double>>` is `double`. Use it to name
 * the precision at which a mixed-precision computation should be carried out.
 * @tparam T The first type.
 * @tparam Ts The remaining types.
 */
template <RealOrComplex T, RealOrComplex... Ts>
using CommonPrecision =
    std::common_type_t<RemoveComplex<T>, RemoveComplex<Ts>...>;

/**
 * @brief The natural result type of an operation on a list of RealOrComplex
 * types.
 * @details The result has the CommonPrecision of the arguments, and is complex
 * if any argument is. So `PromotedType<double, double>` is `double`, while
 * `PromotedType<float, std::complex<double>>` is `std::complex<double>`.
 * @tparam T The first type.
 * @tparam Ts The remaining types.
 */
template <RealOrComplex T, RealOrComplex... Ts>
using PromotedType = std::conditional_t<(Complex<T> or ... or Complex<Ts>),
                                        std::complex<CommonPrecision<T, Ts...>>,
                                        CommonPrecision<T, Ts...>>;

}  // namespace NumericConcepts
