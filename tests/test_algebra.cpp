#include <gtest/gtest.h>

#include <NumericConcepts/NumericConcepts.hpp>
#include <complex>
#include <string>

using namespace NumericConcepts;

namespace {

// A minimal forward-mode automatic-differentiation scalar, standing in for the
// kind of user-defined type the algebra concepts exist to admit. It is not a
// std::complex specialisation, so none of the concepts in Numeric.hpp apply to
// it, but it is a perfectly good field over its real type.
template <Real T>
struct Dual {
  T value{};
  T deriv{};

  Dual() = default;
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

// The extension point: declaring the associated real type is all it takes for
// Dual to satisfy Scalar.
template <NumericConcepts::Real T>
struct NumericConcepts::RealTypeTraits<Dual<T>> {
  using type = T;
};

TEST(AlgebraTests, AlgebraicStructure) {
  static_assert(AdditiveGroup<double>);
  static_assert(Ring<double>);
  static_assert(Field<double>);
  static_assert(Field<std::complex<double>>);

  // Syntactic, not axiomatic: int satisfies Field because the operations exist.
  static_assert(Field<int>);

  static_assert(!AdditiveGroup<std::string>);
  static_assert(!Field<std::string>);
  static_assert(!Field<double *>);
}

TEST(AlgebraTests, Ordering) {
  static_assert(OrderedField<float>);
  static_assert(OrderedField<double>);
  static_assert(!OrderedField<std::complex<double>>);
}

TEST(AlgebraTests, Scaling) {
  static_assert(ScalableBy<double, double>);
  static_assert(ScalableBy<std::complex<double>, double>);
  // A real cannot absorb a complex factor without changing type.
  static_assert(!ScalableBy<double, std::complex<double>>);
}

TEST(AlgebraTests, StandardTypesAreScalars) {
  static_assert(Scalar<float>);
  static_assert(Scalar<double>);
  static_assert(Scalar<std::complex<float>>);
  static_assert(Scalar<std::complex<double>>);

  // Integers have no associated real type, so they are not scalars.
  static_assert(!Scalar<int>);
  static_assert(!HasRealType<int>);

  static_assert(RealScalar<double>);
  static_assert(!RealScalar<std::complex<double>>);
  static_assert(ComplexScalar<std::complex<double>>);
  static_assert(!ComplexScalar<double>);
}

TEST(AlgebraTests, RealTypeAgreesWithRemoveComplex) {
  static_assert(std::is_same_v<RealType<double>, RemoveComplex<double>>);
  static_assert(std::is_same_v<RealType<std::complex<float>>,
                               RemoveComplex<std::complex<float>>>);
}

TEST(AlgebraTests, UserDefinedScalar) {
  using D = Dual<double>;

  static_assert(Field<D>);
  static_assert(Scalar<D>);
  static_assert(std::is_same_v<RealType<D>, double>);

  // A Dual is not its own real type, so it is a ComplexScalar in the general
  // sense, while being invisible to the closed concepts in Numeric.hpp.
  static_assert(ComplexScalar<D>);
  static_assert(!Complex<D>);
  static_assert(!RealOrComplex<D>);

  static_assert(SameRealType<double, std::complex<double>, D>);
  static_assert(!SameRealType<float, D>);
}

TEST(AlgebraTests, CvQualification) {
  // Matching the rule used by the concepts in Numeric.hpp.
  static_assert(Field<const double>);
  static_assert(Scalar<const std::complex<double>>);
  static_assert(HasRealType<const double>);
  static_assert(std::is_same_v<RealType<const std::complex<double>>, double>);
}
