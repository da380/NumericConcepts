#include <gtest/gtest.h>

#include <NumericConcepts/NumericConcepts.hpp>
#include <complex>
#include <iterator>
#include <list>
#include <type_traits>
#include <vector>

using namespace NumericConcepts;

TEST(IteratorTests, IteratorConcepts) {
  using IntVecIter = std::vector<int>::iterator;
  using DoubleVecIter = std::vector<double>::iterator;
  using ComplexListIter = std::list<std::complex<float>>::iterator;

  static_assert(IntegralIterator<IntVecIter>);
  static_assert(RealIterator<DoubleVecIter>);
  static_assert(ComplexIterator<ComplexListIter>);

  static_assert(NumericIterator<IntVecIter>);
  static_assert(NumericIterator<DoubleVecIter>);

  static_assert(RealOrComplexIterator<DoubleVecIter>);
  static_assert(RealOrComplexIterator<ComplexListIter>);
}

TEST(IteratorTests, WritableIteratorConcepts) {
  using IntVecIter = std::vector<int>::iterator;
  using DoubleVecIter = std::vector<double>::iterator;

  static_assert(IntegralWritableIterator<IntVecIter>);
  static_assert(RealWritableIterator<DoubleVecIter>);
  static_assert(NumericWritableIterator<IntVecIter>);
}

TEST(IteratorTests, PrecisionConcepts) {
  using FloatVecIter = std::vector<float>::iterator;
  using DoubleVecIter = std::vector<double>::iterator;
  using ComplexFloatListIter = std::list<std::complex<float>>::iterator;

  static_assert(SameIteratorPrecision<FloatVecIter, ComplexFloatListIter>);
  static_assert(!SameIteratorPrecision<FloatVecIter, DoubleVecIter>);
}

TEST(IteratorTests, StrengthRefinements) {
  using DoubleVecIter = std::vector<double>::iterator;
  using DoubleListIter = std::list<double>::iterator;
  using ComplexVecIter = std::vector<std::complex<double>>::iterator;

  static_assert(RealContiguousIterator<DoubleVecIter>);
  static_assert(RealRandomAccessIterator<DoubleVecIter>);
  static_assert(ComplexContiguousIterator<ComplexVecIter>);
  static_assert(NumericRandomAccessIterator<DoubleVecIter>);
  static_assert(RealOrComplexContiguousIterator<ComplexVecIter>);

  // A list iterator is bidirectional only.
  static_assert(RealIterator<DoubleListIter>);
  static_assert(!RealRandomAccessIterator<DoubleListIter>);
  static_assert(!RealContiguousIterator<DoubleListIter>);

  // A raw pointer is the archetype of a contiguous iterator.
  static_assert(RealContiguousIterator<double *>);
  static_assert(RealWritableContiguousIterator<double *>);
  static_assert(RealContiguousIterator<const double *>);
  static_assert(!RealWritableContiguousIterator<const double *>);

  static_assert(!ComplexContiguousIterator<double *>);
}

TEST(IteratorTests, ValueTypeConcepts) {
  using DoubleVecIter = std::vector<double>::iterator;
  using DoubleListIter = std::list<double>::iterator;
  using FloatVecIter = std::vector<float>::iterator;

  static_assert(std::is_same_v<IteratorValue<DoubleVecIter>, double>);
  static_assert(std::is_same_v<IteratorValue<const double *>, double>);

  // The iterators differ in type, but agree on their value type.
  static_assert(SameIteratorValueType<DoubleVecIter, DoubleListIter>);
  static_assert(!SameIterator<DoubleVecIter, DoubleListIter>);
  static_assert(!SameIteratorValueType<DoubleVecIter, FloatVecIter>);
}

TEST(IteratorTests, OutputIteratorConcepts) {
  using BackInserter = std::back_insert_iterator<std::vector<double>>;
  using OStreamIter = std::ostream_iterator<double>;
  using VecIter = std::vector<double>::iterator;

  // A pure output iterator has no value type of its own — naming
  // std::iter_value_t<BackInserter> is ill formed — so the Writable concepts
  // cannot categorise it. They report false rather than failing to compile,
  // because the substitution failure is inside a constraint. The two-argument
  // forms, which are told the value type, handle it.
  static_assert(!RealWritableIterator<BackInserter>);
  static_assert(RealOutputIterator<BackInserter, double>);
  static_assert(!RealWritableIterator<OStreamIter>);
  static_assert(RealOutputIterator<OStreamIter, double>);

  // An ordinary mutable iterator satisfies both families.
  static_assert(RealWritableIterator<VecIter>);
  static_assert(RealOutputIterator<VecIter, double>);
  static_assert(
      !RealOutputIterator<std::vector<double>::const_iterator, double>);

  // The category applies to the value type, not the iterator.
  static_assert(!ComplexOutputIterator<BackInserter, double>);
  static_assert(!RealOutputIterator<BackInserter, int>);
  static_assert(
      IntegralOutputIterator<std::back_insert_iterator<std::vector<int>>, int>);
  static_assert(ComplexOutputIterator<
                std::back_insert_iterator<std::vector<std::complex<double>>>,
                std::complex<double>>);
  static_assert(NumericOutputIterator<BackInserter, double>);
  static_assert(RealOrComplexOutputIterator<BackInserter, double>);

  // References and cv-qualification are removed before the category is tested,
  // so the idiomatic const T & spelling works.
  static_assert(RealOutputIterator<BackInserter, const double &>);
  static_assert(RealOutputIterator<VecIter, const double &>);

  // Writing a narrower type into a wider container is allowed, and is exactly
  // what the one-argument form cannot express.
  static_assert(RealOutputIterator<VecIter, float>);
  static_assert(OutputIterator<VecIter, float>);
}
