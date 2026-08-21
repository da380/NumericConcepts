/**
 * @file 03-iterators.cpp
 * @brief Constraining algorithms written against iterator pairs.
 *
 * @details The iterator concepts mirror the scalar categories: RealIterator is
 * an input iterator whose value type is Real, and so on. The Writable forms
 * additionally require that the iterator can be assigned through, which is what
 * distinguishes an output position from an input one.
 */

#include <NumericConcepts/Iterators.hpp>
#include <complex>
#include <iostream>
#include <iterator>
#include <list>
#include <vector>

using namespace NumericConcepts;

/**
 * @brief Sums a sequence given as an iterator pair.
 * @details Constrained on NumericIterator, so it accepts integers, reals and
 * complex numbers alike, and on nothing stronger than an input iterator, since
 * a single pass is all it makes. IteratorValue names the accumulator type.
 * @tparam Iter The iterator type.
 * @param first The start of the sequence.
 * @param last One past its end.
 * @return The sum of the elements.
 */
template <NumericIterator Iter>
IteratorValue<Iter> Sum(Iter first, Iter last) {
  auto sum = IteratorValue<Iter>{};
  for (; first != last; ++first) sum += *first;
  return sum;
}

/**
 * @brief Writes a scaled copy of a sequence to an output position.
 * @details The input and output iterators may be of quite different types — a
 * list iterator read into a vector iterator, say — and their value types may
 * differ in whether they are real or complex. SameIteratorPrecision requires
 * only that they agree on precision, which is the condition under which no
 * accuracy is silently lost.
 * @tparam InIter The input iterator type.
 * @tparam OutIter The output iterator type.
 * @param first The start of the input sequence.
 * @param last One past its end.
 * @param out The start of the output sequence.
 * @param factor The scale factor.
 */
template <RealOrComplexIterator InIter, RealOrComplexWritableIterator OutIter>
requires SameIteratorPrecision<InIter, OutIter>
void ScaleInto(InIter first, InIter last, OutIter out,
               IteratorPrecision<InIter> factor) {
  for (; first != last; ++first, ++out) *out = *first * factor;
}

/**
 * @brief Writes a scaled copy of a sequence to any output position.
 * @details The same routine again, constrained the other way. The Writable
 * concepts take the value type from the iterator, so they describe a mutable
 * iterator into existing storage and exclude the pure output iterators, whose
 * value type is `void`. The two-argument Output concepts are told the value
 * type instead, so this version also accepts `std::back_inserter` and
 * `std::ostream_iterator`, which grow or consume their target as they go.
 * @tparam InIter The input iterator type.
 * @tparam OutIter The output iterator type.
 * @param first The start of the input sequence.
 * @param last One past its end.
 * @param out The output position.
 * @param factor The scale factor.
 */
template <RealOrComplexIterator InIter, typename OutIter>
requires RealOrComplexOutputIterator<OutIter, IteratorValue<InIter>>
void ScaleOnto(InIter first, InIter last, OutIter out,
               IteratorPrecision<InIter> factor) {
  for (; first != last; ++first, ++out) *out = *first * factor;
}

int main() {
  const auto integers = std::vector<int>{1, 2, 3, 4};
  const auto reals = std::list<double>{1.5, 2.5, 3.5};
  const auto complexes =
      std::vector<std::complex<double>>{{1.0, 1.0}, {2.0, -2.0}};

  std::cout << "sum(integers)  = " << Sum(integers.begin(), integers.end())
            << "\n"
            << "sum(reals)     = " << Sum(reals.begin(), reals.end()) << "\n"
            << "sum(complexes) = " << Sum(complexes.begin(), complexes.end())
            << "\n";

  // A real input scaled into a complex output: different value types, same
  // precision, so the constraint is satisfied.
  auto out = std::vector<std::complex<double>>(reals.size());
  ScaleInto(reals.begin(), reals.end(), out.begin(), 2.0);
  std::cout << "2 * reals      = ";
  for (const auto &z : out) std::cout << z << " ";
  std::cout << "\n";

  auto scaled = std::vector<std::complex<double>>(complexes.size());
  ScaleInto(complexes.begin(), complexes.end(), scaled.begin(), 2.0);
  std::cout << "2 * complexes  = ";
  for (const auto &z : scaled) std::cout << z << " ";
  std::cout << "\n";

  // ScaleOnto accepts an output position that is not storage at all: here the
  // results go straight to a stream, and then into a vector that grows to meet
  // them.
  std::cout << "2 * reals      = ";
  ScaleOnto(reals.begin(), reals.end(),
            std::ostream_iterator<double>(std::cout, " "), 2.0);
  std::cout << "\n";

  auto grown = std::vector<double>{};
  ScaleOnto(reals.begin(), reals.end(), std::back_inserter(grown), 3.0);
  std::cout << "3 * reals      = ";
  for (auto value : grown) std::cout << value << " ";
  std::cout << "\n";

  // A vector iterator can be written through; a const_iterator cannot, and the
  // Writable concepts are what express that difference.
  static_assert(RealWritableIterator<std::vector<double>::iterator>);
  static_assert(!RealWritableIterator<std::vector<double>::const_iterator>);
  static_assert(RealIterator<std::vector<double>::const_iterator>);

  // The Writable concepts describe a mutable iterator into existing storage,
  // which is what a numerical kernel writes its results through. A pure output
  // iterator has no value type to categorise, so it satisfies none of them —
  // but it does satisfy the two-argument Output forms.
  static_assert(!RealWritableIterator<std::ostream_iterator<double>>);
  static_assert(RealOutputIterator<std::ostream_iterator<double>, double>);
  static_assert(
      RealOutputIterator<std::back_insert_iterator<std::vector<double>>,
                         double>);

  // The Output form is the more general one for ordinary iterators too: it can
  // say "an iterator over doubles I may write floats through", which the
  // Writable form, fixed to the iterator's own value type, cannot.
  static_assert(RealOutputIterator<std::vector<double>::iterator, float>);

  // SameIterator is the strict form, requiring identical types; the value-type
  // and precision forms are the ones usually wanted.
  using VecIter = std::vector<double>::iterator;
  using ListIter = std::list<double>::iterator;
  static_assert(SameIteratorValueType<VecIter, ListIter>);
  static_assert(SameIteratorPrecision<VecIter, ListIter>);
  static_assert(!SameIterator<VecIter, ListIter>);

  return 0;
}
