/**
 * @file 08-scalars.cpp
 * @brief Admitting a user-defined scalar type to the vocabulary.
 *
 * @details Everything so far has used the *closed* concepts: a type is Real or
 * Complex by being a built-in floating-point type or a `std::complex`, and by
 * nothing else. That is the right default, but it shuts out scalar types that
 * behave perfectly well — dual numbers for automatic differentiation, interval
 * arithmetic, extended precision, a quantity carrying units.
 *
 * `Algebra.hpp` describes a type by the arithmetic it supports instead. A
 * routine constrained on Scalar accepts anything that can do field arithmetic
 * and has declared an associated real type, so a user type joins the vocabulary
 * by specialising a single trait.
 */

#include <NumericConcepts/Version.hpp>

#if NUMERICCONCEPTS_VERSION >= NUMERICCONCEPTS_VERSION_NUMBER(0, 1, 0)
#include <NumericConcepts/Algebra.hpp>
#include <NumericConcepts/Numeric.hpp>
#else
#error "This example needs NumericConcepts 0.1.0 or later."
#endif

#include <complex>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

using namespace NumericConcepts;

namespace {

/**
 * @brief A forward-mode automatic-differentiation scalar.
 * @details Carries a value and its derivative with respect to one seeded
 * variable. Arithmetic on it propagates both, so evaluating any expression
 * built from `+ - * /` yields the derivative of that expression for free.
 *
 * Nothing here mentions NumericConcepts: this is just a type with operators.
 * @tparam T The underlying real type.
 */
template <Real T>
struct Dual {
  T value{};  ///< The value of the quantity.
  T deriv{};  ///< Its derivative with respect to the seeded variable.

  Dual() = default;

  /**
   * @brief Constructs a dual number.
   * @param v The value.
   * @param d The derivative; zero by default, giving a constant.
   */
  Dual(T v, T d = T{}) : value(v), deriv(d) {}

  bool operator==(const Dual &) const = default;

  Dual operator+() const { return *this; }
  Dual operator-() const { return {-value, -deriv}; }

  Dual &operator+=(const Dual &o) {
    value += o.value;
    deriv += o.deriv;
    return *this;
  }
  Dual &operator-=(const Dual &o) {
    value -= o.value;
    deriv -= o.deriv;
    return *this;
  }
  Dual &operator*=(const Dual &o) {
    deriv = deriv * o.value + value * o.deriv;
    value *= o.value;
    return *this;
  }
  Dual &operator/=(const Dual &o) {
    deriv = (deriv * o.value - value * o.deriv) / (o.value * o.value);
    value /= o.value;
    return *this;
  }

  friend Dual operator+(Dual a, const Dual &b) { return a += b; }
  friend Dual operator-(Dual a, const Dual &b) { return a -= b; }
  friend Dual operator*(Dual a, const Dual &b) { return a *= b; }
  friend Dual operator/(Dual a, const Dual &b) { return a /= b; }
};

}  // namespace

/**
 * @brief Declares the real type underlying Dual.
 * @details This specialisation is the whole of the opt-in. With it, Dual<T>
 * satisfies Scalar, RealType<Dual<T>> is T, and every routine constrained on
 * Scalar accepts it.
 * @tparam T The underlying real type.
 */
template <NumericConcepts::Real T>
struct NumericConcepts::RealTypeTraits<Dual<T>> {
  using type = T;
};

/**
 * @brief Evaluates a polynomial by Horner's method.
 * @details Constrained on Scalar rather than RealOrComplex, so it works for any
 * type with field arithmetic — including one written long after this routine
 * was.
 * @tparam T The scalar type.
 * @tparam CoefficientRange The type of the coefficient sequence.
 * @param coefficients The coefficients, from the highest power down.
 * @param x The point at which to evaluate.
 * @return The value of the polynomial at x.
 */
template <Scalar T, std::ranges::input_range CoefficientRange>
requires std::convertible_to<std::ranges::range_value_t<CoefficientRange>, T>
T EvaluatePolynomial(const CoefficientRange &coefficients, const T &x) {
  auto result = T{};
  for (const auto &c : coefficients) result = result * x + T(c);
  return result;
}

int main() {
  // p(t) = 2t^3 - t + 4.
  const auto coefficients = std::vector<double>{2.0, 0.0, -1.0, 4.0};

  std::cout << "p(3), double          = "
            << EvaluatePolynomial(coefficients, 3.0) << "\n";

  // The same routine over a complex argument, unchanged.
  std::cout << "p(i), complex<double> = "
            << EvaluatePolynomial(coefficients, std::complex<double>{0.0, 1.0})
            << "\n";

  // And over a dual number seeded at t = 3, which returns p(3) and
  // p'(3) = 6t^2 - 1 = 53 together. The routine knows nothing about
  // differentiation; the scalar type does it.
  const auto p = EvaluatePolynomial(coefficients, Dual<double>{3.0, 1.0});
  std::cout << "p(3), Dual<double>    = " << p.value << "\n"
            << "p'(3)                 = " << p.deriv << "\n";

  // The algebraic concepts, from weakest to strongest.
  static_assert(AdditiveGroup<double>);
  static_assert(Ring<double>);
  static_assert(Field<double>);
  static_assert(Field<std::complex<double>>);
  static_assert(Field<Dual<double>>);
  static_assert(!Field<std::string>);

  // These are syntactic requirements named after the structures they model.
  // They cannot check the axioms: int satisfies Field because the operations
  // exist, even though integer division is not a multiplicative inverse.
  static_assert(Field<int>);

  // Ordering separates the real-like scalars from the rest.
  static_assert(OrderedField<double>);
  static_assert(!OrderedField<std::complex<double>>);

  // Scalar adds the associated real type, and closure under scaling by it.
  static_assert(Scalar<double>);
  static_assert(Scalar<std::complex<float>>);
  static_assert(Scalar<Dual<double>>);
  static_assert(ScalableBy<std::complex<double>, double>);

  // An integer has no associated real type, so it is not a Scalar.
  static_assert(!Scalar<int>);
  static_assert(!HasRealType<int>);

  // RealScalar and ComplexScalar are the open forms of Real and Complex. A
  // Dual is built over a real type, so it counts as complex in this sense,
  // while remaining invisible to the closed concepts.
  static_assert(RealScalar<double>);
  static_assert(ComplexScalar<std::complex<double>>);
  static_assert(ComplexScalar<Dual<double>>);
  static_assert(!RealOrComplex<Dual<double>>);

  // RealType is the open form of RemoveComplex, and the two agree wherever
  // both apply.
  static_assert(std::is_same_v<RealType<std::complex<float>>, float>);
  static_assert(std::is_same_v<RealType<std::complex<float>>,
                               RemoveComplex<std::complex<float>>>);
  static_assert(std::is_same_v<RealType<Dual<double>>, double>);

  // SameRealType is the open form of SamePrecision, and spans both worlds.
  static_assert(SameRealType<double, std::complex<double>, Dual<double>>);
  static_assert(!SameRealType<float, Dual<double>>);

  return 0;
}
