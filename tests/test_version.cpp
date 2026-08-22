#include <gtest/gtest.h>

#include <NumericConcepts/Version.hpp>
#include <string>

TEST(VersionTests, MacrosAgreeWithConstants) {
  static_assert(NumericConcepts::VersionMajor == NUMERICCONCEPTS_VERSION_MAJOR);
  static_assert(NumericConcepts::VersionMinor == NUMERICCONCEPTS_VERSION_MINOR);
  static_assert(NumericConcepts::VersionPatch == NUMERICCONCEPTS_VERSION_PATCH);

  EXPECT_EQ(std::string(NumericConcepts::VersionString),
            std::to_string(NUMERICCONCEPTS_VERSION_MAJOR) + "." +
                std::to_string(NUMERICCONCEPTS_VERSION_MINOR) + "." +
                std::to_string(NUMERICCONCEPTS_VERSION_PATCH));
}

TEST(VersionTests, VersionNumberIsOrdered) {
  static_assert(NUMERICCONCEPTS_VERSION ==
                NUMERICCONCEPTS_VERSION_NUMBER(NUMERICCONCEPTS_VERSION_MAJOR,
                                               NUMERICCONCEPTS_VERSION_MINOR,
                                               NUMERICCONCEPTS_VERSION_PATCH));
  static_assert(NUMERICCONCEPTS_VERSION_NUMBER(0, 1, 0) >
                NUMERICCONCEPTS_VERSION_NUMBER(0, 0, 99));
  static_assert(NUMERICCONCEPTS_VERSION_NUMBER(1, 0, 0) >
                NUMERICCONCEPTS_VERSION_NUMBER(0, 99, 99));
}
