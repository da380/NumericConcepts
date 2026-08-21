/**
 * @file 02-precision.cpp
 * @brief Working with the precision underlying a real or complex type.
 *
 * @details A numerical routine usually cares less about whether its data is
 * real or complex than about whether everything is at the same precision. These
 * utilities let a signature say so, and let it name the types it returns.
 */

#include <NumericConcepts/Numeric.hpp>
#include <complex>
#include <iostream>
#include <vector>

using namespace NumericConcepts;

/**
 * @brief Multiplies two numbers of possibly different type.
 * @details The return type comes from PromotedType: the common precision of the
 * arguments, made complex if either argument is. So a `float` times a
 * `std::complex<double>` yields a `std::complex<double>`, with no narrowing on
 * the way.
 * @tparam T The type of the first factor.
 * @tparam U The type of the second factor.
 * @param a The first factor.
 * @param b The second factor.
 * @return The product, at the promoted type.
 */
template <RealOrComplex T, RealOrComplex U>
PromotedType<T, U> Multiply(T a, U b) {
  using Result = PromotedType<T, U>;
  return Result(a) * Result(b);
}

/**
 * @brief Forms the complex sequence z = x + iy from two real sequences.
 * @details The element type is named with AddComplex, the dual of
 * RemoveComplex, so the function does not have to be told what to return.
 * SamePrecision rejects a `float` sequence paired with a `double` one before
 * any silent conversion can happen.
 * @tparam T The precision of the real parts.
 * @tparam U The precision of the imaginary parts.
 * @param x The real parts.
 * @param y The imaginary parts.
 * @return The combined complex sequence.
 */
template <Real T, Real U>
requires SamePrecision<T, U>
std::vector<AddComplex<T>> MakeComplex(const std::vector<T> &x,
                                       const std::vector<U> &y) {
  auto z = std::vector<AddComplex<T>>{};
  z.reserve(x.size());
  for (std::size_t i = 0; i < x.size(); ++i) z.emplace_back(x[i], y[i]);
  return z;
}

int main() {
  // RemoveComplex answers "at what precision is this stored?", and gives the
  // same answer for a real type and the complex type built on it.
  static_assert(std::is_same_v<RemoveComplex<double>, double>);
  static_assert(std::is_same_v<RemoveComplex<std::complex<double>>, double>);
  static_assert(std::is_same_v<AddComplex<double>, std::complex<double>>);

  // SamePrecision is the usual guard on a multi-argument signature. It allows
  // real to be mixed with complex, which is nearly always wanted, while
  // rejecting float mixed with double, which nearly never is.
  static_assert(SamePrecision<double, std::complex<double>>);
  static_assert(SamePrecision<float, std::complex<float>, float>);
  static_assert(!SamePrecision<float, double>);

  // Where mixing precisions is intended, CommonPrecision names the precision
  // to work at and PromotedType the full result type.
  static_assert(std::is_same_v<CommonPrecision<float, double>, double>);
  static_assert(std::is_same_v<PromotedType<float, std::complex<double>>,
                               std::complex<double>>);
  static_assert(std::is_same_v<PromotedType<double, float>, double>);

  std::cout << "2.0f * 3.0             = " << Multiply(2.0f, 3.0) << "\n"
            << "2.0f * (1+2i)          = "
            << Multiply(2.0f, std::complex<double>{1.0, 2.0}) << "\n";

  const auto x = std::vector<double>{1.0, 2.0, 3.0};
  const auto y = std::vector<double>{0.5, -0.5, 0.0};
  std::cout << "x + iy                 = ";
  for (const auto &z : MakeComplex(x, y)) std::cout << z << " ";
  std::cout << "\n";

  // MakeComplex(x, std::vector<float>{...}) would not compile: the precisions
  // disagree, and the constraint says so at the call site rather than deep
  // inside the body.

  return 0;
}
