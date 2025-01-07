
#include <complex>
#include <functional>
#include <ranges>
#include <string>
#include <vector>

#include "NC/Functions.hpp"
#include "NC/Numeric.hpp"
#include "NC/Ranges.hpp"

int main() {
  static_assert(NC::Real<float>);

  static_assert(NC::Real<const double>);

  static_assert(NC::Integral<int>);

  static_assert(NC::Complex<std::complex<float>>);

  static_assert(!NC::Complex<double>);

  static_assert(std::same_as<double, NC::RemoveComplex<double>>);

  static_assert(std::same_as<double, NC::RemoveComplex<std::complex<double>>>);

  static_assert(!std::same_as<double, NC::RemoveComplex<std::complex<float>>>);

  static_assert(NC::Numeric<float>);

  static_assert(NC::Numeric<int>);

  static_assert(!NC::Numeric<std::string>);

  static_assert(NC::RealRange<std::vector<double>>);

  static_assert(NC::RealRange<const std::vector<double>>);

  static_assert(!NC::RealWritableRange<const std::vector<double>>);

  static_assert(NC::RealView<std::views::all_t<std::vector<double>>>);

  static_assert(!NC::ComplexView<std::views::all_t<std::vector<double>>>);

  static_assert(NC::IntegralFunction<std::function<int(int)>, int>);

  static_assert(
      NC::RealOrComplexFunction<
          std::function<float(std::complex<float>, float)>, float, float>);

  static_assert(
      !NC::NumericFunction<std::function<float(int)>, std::complex<double>>);
}