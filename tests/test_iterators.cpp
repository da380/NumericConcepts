#include <gtest/gtest.h>

#include <NumericConcepts/NumericConcepts.hpp>
#include <complex>
#include <list>
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