#pragma once

#include <complex>
#include <concepts>

namespace NumericConcepts {

// Concept for integral types.
template <typename T>
concept Integral = std::integral<T>;

// Concept for floating point types.
template <typename T>
concept Real = std::floating_point<T>;

// Concept for floating points at different precisions.
template <typename T>
concept Float = std::same_as<T, float>;

template <typename T>
concept Double = std::same_as<T, double>;

template <typename T>
concept LongDouble = std::same_as<T, long double>;

// Concept for complex types.
template <typename T> struct ComplexType : public std::false_type {};

template <typename T>
struct ComplexType<std::complex<T>> : public std::bool_constant<Real<T>> {};

template <typename T>
concept Complex = ComplexType<T>::value;

// Concept for real or complex numbers.
template <typename T>
concept RealOrComplex = Real<T> or Complex<T>;

// Concept for Numeric types, i.e., integers, reals or complex numbers.
template <typename T>
concept Numeric = Integral<T> or RealOrComplex<T>;

// Extract base type from a complex type. If the type is not
// complex the input type is returned.
template <RealOrComplex T> struct RemoveComplexHelper {
  using value_type = T;
};

template <RealOrComplex T> struct RemoveComplexHelper<std::complex<T>> {
  using value_type = T;
};

template <RealOrComplex T>
using RemoveComplex = typename RemoveComplexHelper<T>::value_type;

// Concept to check if a list to real or complex types have the same precision.
template <typename T, typename... Ts>
concept SamePrecision =
    (std::same_as<RemoveComplex<T>, RemoveComplex<Ts>> && ...);

} // namespace NumericConcepts