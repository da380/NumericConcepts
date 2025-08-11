#pragma once

#include <concepts>
#include <system_error>
#include <type_traits>

#include "Numeric.hpp"

/**
 * @file Functions.hpp
 * @brief Defines concepts for functions with numeric return types.
 */

namespace NumericConcepts {

/**
 * @brief A generic concept for a function.
 * @details Checks if a type F is invocable with arguments Args... and if the
 * result is convertible to a specified Value type.
 * @tparam F The invocable type (e.g., function pointer, lambda).
 * @tparam Value The required return type (or a type it's convertible to).
 * @tparam Args The argument types for the function invocation.
 */
template <typename F, typename Value, typename... Args>
concept Function = requires() {
  requires std::invocable<F, Args...>;
  requires std::convertible_to<std::invoke_result_t<F, Args...>, Value>;
};

/**
 * @brief Concept for a function that returns an integral value.
 * @tparam F The invocable type.
 * @tparam Args The argument types.
 */
template <typename F, typename... Args>
concept IntegralFunction = requires() {
  requires std::invocable<F, Args...>;
  requires Integral<std::invoke_result_t<F, Args...>>;
};

/**
 * @brief Concept for a function that returns a real (floating-point) value.
 * @tparam F The invocable type.
 * @tparam Args The argument types.
 */
template <typename F, typename... Args>
concept RealFunction = requires() {
  requires std::invocable<F, Args...>;
  requires Real<std::invoke_result_t<F, Args...>>;
};

/**
 * @brief Concept for a function that returns a complex value.
 * @tparam F The invocable type.
 * @tparam Args The argument types.
 */
template <typename F, typename... Args>
concept ComplexFunction = requires() {
  requires std::invocable<F, Args...>;
  requires Complex<std::invoke_result_t<F, Args...>>;
};

/**
 * @brief Concept for a function that returns a real or complex value.
 * @tparam F The invocable type.
 * @tparam Args The argument types.
 */
template <typename F, typename... Args>
concept RealOrComplexFunction = requires() {
  requires std::invocable<F, Args...>;
  requires RealOrComplex<std::invoke_result_t<F, Args...>>;
};

/**
 * @brief Concept for a function that returns any numeric value (integral, real,
 * or complex).
 * @tparam F The invocable type.
 * @tparam Args The argument types.
 */
template <typename F, typename... Args>
concept NumericFunction = requires() {
  requires std::invocable<F, Args...>;
  requires Numeric<std::invoke_result_t<F, Args...>>;
};

}  // namespace NumericConcepts