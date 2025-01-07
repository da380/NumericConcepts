
#include <complex>
#include <functional>
#include <ranges>
#include <string>
#include <vector>

#include "NumericConcepts/Functions.hpp"
#include "NumericConcepts/Numeric.hpp"
#include "NumericConcepts/Ranges.hpp"

int main() {
  static_assert(NumericConcepts::Real<float>);

  static_assert(NumericConcepts::Real<const double>);

  static_assert(NumericConcepts::Integral<int>);

  static_assert(NumericConcepts::Complex<std::complex<float>>);

  static_assert(!NumericConcepts::Complex<double>);

  static_assert(std::same_as<double, NumericConcepts::RemoveComplex<double>>);

  static_assert(
      std::same_as<double,
                   NumericConcepts::RemoveComplex<std::complex<double>>>);

  static_assert(
      !std::same_as<double,
                    NumericConcepts::RemoveComplex<std::complex<float>>>);

  static_assert(NumericConcepts::Numeric<float>);

  static_assert(NumericConcepts::Numeric<int>);

  static_assert(!NumericConcepts::Numeric<std::string>);

  static_assert(NumericConcepts::RealRange<std::vector<double>>);

  static_assert(NumericConcepts::RealRange<const std::vector<double>>);

  static_assert(!NumericConcepts::RealWritableRange<const std::vector<double>>);

  static_assert(
      NumericConcepts::RealView<std::views::all_t<std::vector<double>>>);

  static_assert(
      !NumericConcepts::ComplexView<std::views::all_t<std::vector<double>>>);

  static_assert(
      NumericConcepts::IntegralFunction<std::function<int(int)>, int>);

  static_assert(
      NumericConcepts::RealOrComplexFunction<
          std::function<float(std::complex<float>, float)>, float, float>);

  static_assert(!NumericConcepts::NumericFunction<std::function<float(int)>,
                                                  std::complex<double>>);

  static_assert(
      !NumericConcepts::NumericFunction<std::function<std::string(int)>, int>);
}