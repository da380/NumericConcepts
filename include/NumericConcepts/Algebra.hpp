#pragma once

#include <complex>
#include <concepts>
#include <type_traits>

#include "Numeric.hpp"

/**
 * @file Algebra.hpp
 * @brief Defines concepts describing the arithmetic a scalar type supports,
 * rather than which standard type it happens to be.
 *
 * @details The concepts in Numeric.hpp are closed: a type is Real or Complex
 * only if it is a built-in floating-point type or a `std::complex`
 * specialisation. That is exactly what is wanted most of the time, but it shuts
 * out scalar types that behave perfectly well arithmetically — dual numbers for
 * automatic differentiation, interval arithmetic, extended precision, or a
 * quantity carrying units.
 *
 * The concepts here describe such a type by what it can do. A user-defined
 * scalar joins the vocabulary by satisfying them, and by specialising
 * RealTypeTraits to declare its associated real type.
 *
 * These are *syntactic* requirements named after the algebraic structures they
 * are modelled on. They cannot check the axioms: floating-point addition is not
 * associative, and `int` satisfies Field even though integer division is not a
 * multiplicative inverse. The names describe the operations a template may use,
 * which is what a constraint is for.
 *
 * As elsewhere in the library, top-level cv-qualification is removed before
 * the requirements are applied, so `Field<const double>` is satisfied.
 */

namespace NumericConcepts {

namespace Internal {

// clang-format off
// Left unformatted: clang-format reads the `*` in `a * b` inside a
// requires-expression as a pointer declarator and closes up the spacing.

/**
 * @internal
 * @brief The requirements of AdditiveGroup, on a cv-unqualified type.
 */
template <typename T>
concept AdditiveGroupOf = std::regular<T> && requires(T a, const T b) {
  { +a } -> std::convertible_to<T>;
  { -a } -> std::convertible_to<T>;
  { a + b } -> std::convertible_to<T>;
  { a - b } -> std::convertible_to<T>;
  { a += b } -> std::same_as<T &>;
  { a -= b } -> std::same_as<T &>;
};

/**
 * @internal
 * @brief The requirements of Ring, on a cv-unqualified type.
 */
template <typename T>
concept RingOf = AdditiveGroupOf<T> && requires(T a, const T b) {
  { a * b } -> std::convertible_to<T>;
  { a *= b } -> std::same_as<T &>;
};

/**
 * @internal
 * @brief The requirements of Field, on a cv-unqualified type.
 */
template <typename T>
concept FieldOf = RingOf<T> && requires(T a, const T b) {
  { a / b } -> std::convertible_to<T>;
  { a /= b } -> std::same_as<T &>;
};

/**
 * @internal
 * @brief The default associated real type: empty unless T is RealOrComplex.
 *
 * Selected on an ordinary bool argument rather than by constraining a
 * specialisation whose argument list matches the primary template's parameter
 * list. That form is valid C++20, but is rejected by MSVC and by Clang before
 * version 16, both of which this library supports.
 */
template <typename T, bool = RealOrComplex<T>>
struct DefaultRealType {};

/**
 * @internal
 * @brief The associated real type of a standard real or complex type.
 */
template <typename T>
struct DefaultRealType<T, true> {
  using type = RemoveComplex<T>;
};

/**
 * @internal
 * @brief The requirements of ScalableBy, on cv-unqualified types.
 */
template <typename T, typename S>
concept ScalableByOf = requires(T a, const S s) {
  { a * s } -> std::convertible_to<T>;
  { s * a } -> std::convertible_to<T>;
  { a *= s } -> std::same_as<T &>;
};

// clang-format on

}  // namespace Internal

/**
 * @brief Concept for a type closed under addition and subtraction.
 * @details Requires `std::regular`, unary `+` and `-`, binary `+` and `-`, and
 * the corresponding compound assignments. A value-initialised `T{}` is taken to
 * be the additive identity.
 * @tparam T The type to check.
 */
template <typename T>
concept AdditiveGroup = Internal::AdditiveGroupOf<std::remove_cv_t<T>>;

/**
 * @brief Concept for a type that is additionally closed under multiplication.
 * @tparam T The type to check.
 */
template <typename T>
concept Ring = Internal::RingOf<std::remove_cv_t<T>>;

/**
 * @brief Concept for a type that is additionally closed under division.
 * @details Division by a zero element is, as ever, the caller's problem.
 * @tparam T The type to check.
 */
template <typename T>
concept Field = Internal::FieldOf<std::remove_cv_t<T>>;

/**
 * @brief Concept for a Field on which the arithmetic operations are ordered.
 * @details Satisfied by the floating-point types, and by a user-defined real
 * scalar; not by any complex type.
 * @tparam T The type to check.
 */
template <typename T>
concept OrderedField = Field<T> && std::totally_ordered<T>;

/**
 * @brief Concept for a type whose values can be scaled by values of another.
 * @details Multiplication must be available in both orders and as a compound
 * assignment, and must return something convertible to T. This is the
 * requirement that makes T a vector space, or a module, over S.
 * @tparam T The type being scaled.
 * @tparam S The type scaling it.
 */
template <typename T, typename S>
concept ScalableBy =
    Internal::ScalableByOf<std::remove_cv_t<T>, std::remove_cv_t<S>>;

/**
 * @brief Customisation point declaring the real type associated with a scalar.
 * @details The primary template already covers every type satisfying
 * RealOrComplex, for which the answer is RemoveComplex. Any other type has no
 * associated real type until one is declared, and a user-defined scalar
 * declares its own:
 *
 * ```cpp
 * template <NumericConcepts::Real T>
 * struct NumericConcepts::RealTypeTraits<MyDual<T>> {
 *   using type = T;
 * };
 * ```
 *
 * Specialise it for the cv-unqualified type; RealType removes cv-qualification
 * before consulting it.
 * @tparam T The scalar type.
 */
template <typename T>
struct RealTypeTraits : Internal::DefaultRealType<T> {};

/**
 * @brief Concept for a type that has declared an associated real type.
 * @tparam T The type to check.
 */
template <typename T>
concept HasRealType = requires {
  typename RealTypeTraits<std::remove_cv_t<T>>::type;
  requires Real<typename RealTypeTraits<std::remove_cv_t<T>>::type>;
};

/**
 * @brief The real type associated with a scalar type.
 * @details Agrees with RemoveComplex wherever RemoveComplex is defined, but is
 * open: it is also available for user-defined scalars that specialise
 * RealTypeTraits. Use RemoveComplex when the argument is known to be a standard
 * real or complex type, and RealType when it may be anything scalar.
 * @tparam T The scalar type.
 */
template <HasRealType T>
using RealType = typename RealTypeTraits<std::remove_cv_t<T>>::type;

/**
 * @brief Concept for a type usable as the scalar of a numerical algorithm.
 * @details A Field with an associated real type, closed under scaling by that
 * real type. Satisfied by the floating-point types, by every `std::complex`
 * specialisation over them, and by any user-defined scalar that meets the same
 * requirements.
 * @tparam T The type to check.
 */
template <typename T>
concept Scalar = Field<T> && HasRealType<T> && ScalableBy<T, RealType<T>>;

/**
 * @brief Concept for a Scalar that is its own real type.
 * @tparam T The type to check.
 */
template <typename T>
concept RealScalar =
    Scalar<T> && std::same_as<std::remove_cv_t<T>, RealType<T>>;

/**
 * @brief Concept for a Scalar that is not its own real type.
 * @details The generalisation of Complex: satisfied by `std::complex<T>`, and
 * by a user-defined scalar built over a real type.
 * @tparam T The type to check.
 */
template <typename T>
concept ComplexScalar = Scalar<T> && !RealScalar<T>;

/**
 * @brief Concept to check if a list of scalars share an associated real type.
 * @details The open counterpart of SamePrecision, which it agrees with on
 * standard real and complex types.
 * @tparam T The first type to compare.
 * @tparam Ts The other types to compare against the first.
 */
template <typename T, typename... Ts>
concept SameRealType = requires() {
  requires HasRealType<T> && (HasRealType<Ts> && ...);
  requires(std::same_as<RealType<T>, RealType<Ts>> && ...);
};

}  // namespace NumericConcepts
