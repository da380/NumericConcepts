/**
 * @file 07-functions.cpp
 * @brief Constraining callables by what they return.
 *
 * @details A routine that takes a function — a quadrature rule, a root finder,
 * an optimiser — needs to say what it will call the function with and what it
 * expects back. The Function concepts carry both: the argument types are the
 * trailing template parameters, and the category is the prefix.
 */

#include <NumericConcepts/Functions.hpp>
#include <cmath>
#include <complex>
#include <iostream>
#include <numbers>
#include <vector>

using namespace NumericConcepts;

/**
 * @brief Integrates a real function of one real variable by the midpoint rule.
 * @details `RealFunction<F, T>` reads as "F, called with a T, returns a Real".
 * Passing a function that returns void, or one that cannot be called with a
 * double at all, fails at the call site.
 * @tparam T The precision to work at.
 * @tparam F The type of the integrand.
 * @param f The integrand.
 * @param a The lower limit.
 * @param b The upper limit.
 * @param n The number of subintervals.
 * @return An approximation to the integral of f from a to b.
 */
template <Real T, RealFunction<T> F>
T Integrate(F &&f, T a, T b, std::size_t n) {
  const auto h = (b - a) / static_cast<T>(n);
  auto sum = T{};
  for (std::size_t i = 0; i < n; ++i)
    sum += f(a + (static_cast<T>(i) + T{0.5}) * h);
  return sum * h;
}

/**
 * @brief Samples any numeric function of one variable onto a grid.
 * @details Constrained on NumericFunction, so the sample type follows from the
 * function rather than being fixed in advance: a real integrand gives a vector
 * of reals, a complex one a vector of complex numbers.
 * @tparam T The argument type.
 * @tparam F The type of the function to sample.
 * @param f The function to sample.
 * @param points The points at which to sample it.
 * @return The sampled values.
 */
template <Real T, NumericFunction<T> F>
auto Sample(F &&f, const std::vector<T> &points) {
  using Value = std::invoke_result_t<F, T>;
  auto values = std::vector<Value>{};
  values.reserve(points.size());
  for (const auto &x : points) values.push_back(f(x));
  return values;
}

/**
 * @brief A free function, to show that the concepts are not lambda-specific.
 * @param x The argument.
 * @return The square of x.
 */
double Square(double x) { return x * x; }

int main() {
  std::cout << "int_0^1 x^2 dx        = " << Integrate(Square, 0.0, 1.0, 1000)
            << "\n"
            << "int_0^pi sin(x) dx    = "
            << Integrate([](double x) { return std::sin(x); }, 0.0,
                         std::numbers::pi_v<double>, 1000)
            << "\n";

  const auto grid = std::vector<double>{0.0, 0.5, 1.0};

  std::cout << "sample(x -> x^2)      = ";
  for (auto v : Sample(Square, grid)) std::cout << v << " ";
  std::cout << "\n";

  // The same routine, now producing complex samples, with no overload needed.
  std::cout << "sample(x -> e^{ix})   = ";
  for (auto v : Sample([](double x) { return std::polar(1.0, x); }, grid))
    std::cout << v << " ";
  std::cout << "\n";

  // The categories work exactly as they do for values.
  auto real_lambda = [](double x) { return x * x; };
  auto complex_lambda = [](double x) { return std::complex<double>{x, x}; };
  auto integral_lambda = [](double x) { return static_cast<int>(x); };

  static_assert(RealFunction<decltype(real_lambda), double>);
  static_assert(ComplexFunction<decltype(complex_lambda), double>);
  static_assert(IntegralFunction<decltype(integral_lambda), double>);
  static_assert(NumericFunction<decltype(integral_lambda), double>);
  static_assert(RealOrComplexFunction<decltype(complex_lambda), double>);

  static_assert(!RealFunction<decltype(complex_lambda), double>);

  // Argument types are part of the concept: this lambda is not callable with
  // two doubles, and is not a RealFunction of two doubles.
  static_assert(!RealFunction<decltype(real_lambda), double, double>);

  // Functions of several arguments, and free functions, are handled the same.
  static_assert(RealFunction<decltype(&Square), double>);
  static_assert(RealFunction<decltype([](double x, double y) { return x + y; }),
                             double, double>);

  // The general Function concept fixes the return type rather than a category,
  // and allows a conversion to it.
  static_assert(Function<decltype(integral_lambda), long, double>);
  static_assert(!Function<decltype(complex_lambda), double, double>);

  return 0;
}
