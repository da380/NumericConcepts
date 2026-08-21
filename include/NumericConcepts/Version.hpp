#pragma once

/**
 * @file Version.hpp
 * @brief The version of the library, for downstream feature tests.
 *
 * @details As the vocabulary grows, a library that depends on NumericConcepts
 * may need to know which concepts are available. The macros here allow that to
 * be tested at preprocessing time:
 *
 * ```cpp
 * #include <NumericConcepts/Version.hpp>
 *
 * #if NUMERICCONCEPTS_VERSION >= NUMERICCONCEPTS_VERSION_NUMBER(0, 1, 0)
 * #include <NumericConcepts/Algebra.hpp>
 * #endif
 * ```
 *
 * The values are written here by hand rather than generated, so that copying
 * the `include` directory into another project remains a complete way to
 * vendor the library. The build checks them against the version declared in
 * `CMakeLists.txt` at configure time, so the two cannot drift apart.
 */

/** @brief The major version of the library. */
#define NUMERICCONCEPTS_VERSION_MAJOR 0

/** @brief The minor version of the library. */
#define NUMERICCONCEPTS_VERSION_MINOR 1

/** @brief The patch version of the library. */
#define NUMERICCONCEPTS_VERSION_PATCH 0

/** @brief Encodes a version triple as a single comparable integer. */
#define NUMERICCONCEPTS_VERSION_NUMBER(major, minor, patch) \
  ((major) * 10000 + (minor) * 100 + (patch))

/** @brief This version of the library, as a single comparable integer. */
#define NUMERICCONCEPTS_VERSION                                 \
  NUMERICCONCEPTS_VERSION_NUMBER(NUMERICCONCEPTS_VERSION_MAJOR, \
                                 NUMERICCONCEPTS_VERSION_MINOR, \
                                 NUMERICCONCEPTS_VERSION_PATCH)

/** @brief This version of the library, as a string literal. */
#define NUMERICCONCEPTS_VERSION_STRING "0.1.0"

namespace NumericConcepts {

/** @brief The major version of the library. */
inline constexpr int VersionMajor = NUMERICCONCEPTS_VERSION_MAJOR;

/** @brief The minor version of the library. */
inline constexpr int VersionMinor = NUMERICCONCEPTS_VERSION_MINOR;

/** @brief The patch version of the library. */
inline constexpr int VersionPatch = NUMERICCONCEPTS_VERSION_PATCH;

/** @brief The version of the library, as a string. */
inline constexpr const char* VersionString = NUMERICCONCEPTS_VERSION_STRING;

}  // namespace NumericConcepts
