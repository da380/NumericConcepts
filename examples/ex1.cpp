
#include <complex>
#include <iostream>
#include <list>
#include <vector>

// Include the main header for the NumericConcepts library.
// Assumes the library was installed, so we use the subdirectory.
#include <NumericConcepts/NumericConcepts.hpp>

// Use the library's namespace for convenience.
using namespace NumericConcepts;

/**
 * @brief A generic function that prints information about a range of numbers.
 * @details This function is constrained by the `NumericRange` concept, meaning
 * it will only compile if the argument is a range of numeric values
 * (integral, real, or complex).
 *
 * @tparam R The type of the range, which must satisfy NumericRange.
 * @param range The range of numbers to process.
 */
template <NumericRange R>
void print_range_info(const R& range, const std::string& name) {
  std::cout << "--- Processing range: " << name << " ---\n";

  // Use `if constexpr` to execute different code paths at compile time
  // based on the specific numeric concept the range satisfies.
  if constexpr (IntegralRange<R>) {
    std::cout << "Type: Integral Range\n";
  } else if constexpr (RealRange<R>) {
    std::cout << "Type: Real (Floating-Point) Range\n";
  } else if constexpr (ComplexRange<R>) {
    std::cout << "Type: Complex Range\n";
  }

  // Iterate through the range and print its elements.
  std::cout << "Values: [ ";
  for (const auto& value : range) {
    std::cout << value << " ";
  }
  std::cout << "]\n\n";
}

int main() {
  // Create containers with different numeric types.
  std::vector<int> integers = {1, 2, 3};
  std::list<double> doubles = {1.1, 2.2, 3.3};
  std::vector<std::complex<float>> complexes = {{1, 1}, {2, -2}};
  std::vector<unsigned int> unsigned_integers = {100, 200};

  // Call our generic function with different kinds of numeric ranges.
  // The compiler correctly deduces the type and applies the concept.
  print_range_info(integers, "std::vector<int>");
  print_range_info(doubles, "std::list<double>");
  print_range_info(complexes, "std::vector<std::complex<float>>");
  print_range_info(unsigned_integers, "std::vector<unsigned int>");

  // --- COMPILE-TIME SAFETY ---
  // The following line will cause a compilation error because
  // std::vector<std::string> does not satisfy the `NumericRange` concept. This
  // is the primary benefit of your library: preventing logic errors at compile
  // time.
  //
  // std::vector<std::string> strings = {"hello", "world"};
  // print_range_info(strings, "std::vector<std::string>"); // <-- This will
  // fail to compile!

  return 0;
}
