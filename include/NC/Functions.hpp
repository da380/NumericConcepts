#pragma once

#include <concepts>
#include <system_error>

#include "Numeric.hpp"

namespace NC {

// Concept for a function of integer arguments that returns an integer.
template <typename F, typename... Args>
concept IntegralFunction = requires() {
  requires(Integral<Args> && ...);
  requires std::invocable<F, Args...>;
  requires Integral<std::invoke_result_t<F, Args...>>;
};

// Concept for a function of real arguments that returns an real.
template <typename F, typename... Args>
concept RealFunction = requires() {
  requires(Real<Args> && ...);
  requires std::invocable<F, Args...>;
  requires Real<std::invoke_result_t<F, Args...>>;
};

// Concept for a function of complex arguments that returns a complex number.
template <typename F, typename... Args>
concept ComplexFunction = requires() {
  requires(Complex<Args> && ...);
  requires std::invocable<F, Args...>;
  requires Complex<std::invoke_result_t<F, Args...>>;
};

// Concept for a function of real or complex arguments that returns a real or
// complex number.
template <typename F, typename... Args>
concept RealOrComplexFunction = requires() {
  requires(RealOrComplex<Args> && ...);
  requires std::invocable<F, Args...>;
  requires RealOrComplex<std::invoke_result_t<F, Args...>>;
};

// Concept for a function of numeric arguments that returns a numeric value.
template <typename F, typename... Args>
concept NumericFunction = requires() {
  requires(Numeric<Args> && ...);
  requires std::invocable<F, Args...>;
  requires Numeric<std::invoke_result_t<F, Args...>>;
};

// Concept for a function of complex arguments that returns a real value.
template <typename F, typename... Args>
concept ComplexToRealFunction = requires() {
  requires(Complex<Args> && ...);
  requires std::invocable<F, Args...>;
  requires Real<std::invoke_result_t<F, Args...>>;
};

} // namespace NC