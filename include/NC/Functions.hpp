#pragma once

#include <concepts>
#include <system_error>
#include <type_traits>

#include "Numeric.hpp"

namespace NC {

// Concept for a functional with given return type and arguments.
template <typename F, typename Value, typename... Args>
concept Function = requires() {
  requires std::invocable<F, Args...>;
  requires std::convertible_to<std::invoke_result_t<F, Args...>, Value>;
};

// Concept for a integral-valued function of integral arguments.
template <typename F, typename... Args>
concept IntegralFunction = requires() {
  requires(Integral<Args> && ...);
  requires std::invocable<F, Args...>;
  requires Integral<std::invoke_result_t<F, Args...>>;
};

// Concept for a real-valued function of real arguments.
template <typename F, typename... Args>
concept RealFunction = requires() {
  requires(Real<Args> && ...);
  requires std::invocable<F, Args...>;
  requires Real<std::invoke_result_t<F, Args...>>;
};

// Concept for a complex-valued function of complex arguments.
template <typename F, typename... Args>
concept ComplexFunction = requires() {
  requires(Complex<Args> && ...);
  requires std::invocable<F, Args...>;
  requires Complex<std::invoke_result_t<F, Args...>>;
};

// Concept for a real- or complex-valued function of real or complex arguments.
template <typename F, typename... Args>
concept RealOrComplexFunction = requires() {
  requires(RealOrComplex<Args> && ...);
  requires std::invocable<F, Args...>;
  requires RealOrComplex<std::invoke_result_t<F, Args...>>;
};

// Concept for a numeric-valued function of numeric arguments.
template <typename F, typename... Args>
concept NumericFunction = requires() {
  requires(Numeric<Args> && ...);
  requires std::invocable<F, Args...>;
  requires Numeric<std::invoke_result_t<F, Args...>>;
};

} // namespace NC