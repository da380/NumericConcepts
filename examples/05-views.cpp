/**
 * @file 05-views.cpp
 * @brief Numeric views, and lazy pipelines that stay constrained.
 *
 * @details A view is a range that is cheap to copy and owns nothing, so it can
 * be passed and returned by value. The View concepts are the range concepts
 * with `std::ranges::view` added, and they let a signature insist on a lazy
 * sequence rather than a container — useful when a routine returns a
 * transformation of its input without materialising it.
 */

#include <NumericConcepts/Ranges.hpp>
#include <complex>
#include <iostream>
#include <ranges>
#include <span>
#include <utility>
#include <vector>

using namespace NumericConcepts;

/**
 * @brief Returns a lazy view of the squared magnitudes of a complex sequence.
 * @details Nothing is computed and nothing is allocated until the result is
 * iterated. The return type is deduced, and the static_assert below records
 * that it is a RealView: real, because a squared magnitude is; a view, because
 * it borrows rather than owns.
 * @tparam R The complex range type.
 * @param range The complex range.
 * @return A view of the squared magnitudes.
 */
template <ComplexView R>
auto SquaredMagnitudes(R range) {
  return range | std::views::transform([](const auto &z) {
           return z.real() * z.real() + z.imag() * z.imag();
         });
}

/**
 * @brief Prints a real view.
 * @details Taking the view by value is the idiom: a view is a handle, and
 * copying it costs nothing.
 * @tparam R The view type.
 * @param range The view to print.
 * @param name A label for the output.
 */
template <RealView R>
void Print(R range, const char *name) {
  std::cout << name << " = [ ";
  for (const auto &value : range) std::cout << value << " ";
  std::cout << "]\n";
}

int main() {
  const auto data =
      std::vector<std::complex<double>>{{3.0, 4.0}, {1.0, 0.0}, {0.0, 2.0}};

  // std::views::all turns the container into a view over it; the container
  // itself is a range but not a view, since it owns its elements.
  static_assert(ComplexRange<std::vector<std::complex<double>>>);
  static_assert(!ComplexView<std::vector<std::complex<double>>>);

  // A view must be movable, so bind one to a non-const variable: a
  // `const` view does not itself satisfy std::ranges::view.
  auto magnitudes = SquaredMagnitudes(std::views::all(data));
  static_assert(RealView<decltype(magnitudes)>);
  Print(magnitudes, "|z|^2       ");

  // Views compose, and the concepts hold all the way along the pipeline.
  Print(magnitudes | std::views::filter([](double m) { return m > 1.0; }),
        "|z|^2 > 1   ");

  // An iota_view is an IntegralView: lazily generated, never stored.
  auto counting = std::views::iota(1, 6);
  static_assert(IntegralView<decltype(counting)>);
  static_assert(NumericView<decltype(counting)>);
  static_assert(!RealView<decltype(counting)>);

  Print(counting | std::views::transform([](int i) { return 1.0 / i; }),
        "1/n         ");

  // A span is the other common view: a borrowed window onto contiguous
  // storage. Over non-const elements it is writable, which containers-as-views
  // generally are not.
  auto buffer = std::vector<double>{1.0, 2.0, 3.0, 4.0};
  auto window = std::span(buffer).subspan(1, 2);
  static_assert(RealView<decltype(window)>);
  static_assert(RealWritableView<decltype(window)>);

  for (auto &value : window) value = -value;
  Print(std::views::all(std::as_const(buffer)), "buffer      ");

  static_assert(RealView<std::span<const double>>);
  static_assert(!RealWritableView<std::span<const double>>);

  return 0;
}
