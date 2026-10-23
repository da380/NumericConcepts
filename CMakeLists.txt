cmake_minimum_required(VERSION 3.15...3.31)

project(
    NumericConcepts
    VERSION 0.1.0
    DESCRIPTION "C++20 concepts for generic numerical programming"
    HOMEPAGE_URL "https://github.com/da380/NumericConcepts"
    LANGUAGES CXX)

# PROJECT_IS_TOP_LEVEL is only defined from CMake 3.21, so derive it by hand.
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(NUMERICCONCEPTS_IS_TOP_LEVEL ON)
else()
    set(NUMERICCONCEPTS_IS_TOP_LEVEL OFF)
endif()

include(GNUInstallDirs)

# --- The library itself ---

# Header-only, so an INTERFACE library.
add_library(${PROJECT_NAME} INTERFACE)

# The namespaced alias makes add_subdirectory()/FetchContent consumers spell the
# target exactly as find_package() consumers do.
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(
    ${PROJECT_NAME}
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Attached to the target, so consumers inherit the C++20 requirement instead of
# having to know about it.
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

# The version in Version.hpp is written by hand so that the include directory
# stays self-sufficient for vendoring; check here that it has not drifted from
# the version declared above.
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/Version.hpp
     _version_header)
foreach(_part MAJOR MINOR PATCH)
    string(REGEX MATCH "define NUMERICCONCEPTS_VERSION_${_part} ([0-9]+)"
           _match "${_version_header}")
    list(APPEND _header_version ${CMAKE_MATCH_1})
endforeach()
list(JOIN _header_version "." _header_version)
if(NOT _header_version STREQUAL PROJECT_VERSION)
    message(FATAL_ERROR
        "Version mismatch: CMakeLists.txt declares ${PROJECT_VERSION} but "
        "include/${PROJECT_NAME}/Version.hpp declares ${_header_version}. "
        "Update both, including NUMERICCONCEPTS_VERSION_STRING.")
endif()
if(NOT _version_header MATCHES
   "NUMERICCONCEPTS_VERSION_STRING \"${PROJECT_VERSION}\"")
    message(FATAL_ERROR
        "NUMERICCONCEPTS_VERSION_STRING does not match ${PROJECT_VERSION}.")
endif()

# --- Options ---

# ON even when this is a subproject, which is deliberate and unlike the usual
# top-level-only default. A library that links NumericConcepts publicly and
# exports its own targets cannot be installed unless NumericConcepts is in an
# export set too: CMake refuses with "includes target X which requires target
# NumericConcepts that is not in any export set". Since being consumed by such
# libraries is the point of this one, the rules stay on, and a consumer that
# genuinely does not want them says so.
option(NUMERICCONCEPTS_INSTALL "Generate the install rules for NumericConcepts" ON)
option(NUMERICCONCEPTS_BUILD_EXAMPLES "Build the examples" ${NUMERICCONCEPTS_IS_TOP_LEVEL})
option(NUMERICCONCEPTS_BUILD_TESTS "Build the tests" OFF)
option(NUMERICCONCEPTS_BUILD_DOCS "Build the Doxygen documentation" OFF)

# Only the prefixed spellings are read. The unprefixed BUILD_TESTS and
# BUILD_DOCS that used to be honoured here belong to whichever project is
# consuming this one, so they switched on this project's tests and docs in
# every superbuild that set them for itself.

# --- Installation and packaging ---

if(NUMERICCONCEPTS_INSTALL)
    include(CMakePackageConfigHelpers)

    # For an INTERFACE library this installs no files; it records the target in
    # the export set so that the usage requirements above can be re-created.
    install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
    )

    install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.hpp"
    )

    install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )

    # SameMajorVersion, rather than the AnyNewerVersion this used to be, matches
    # the promise the README makes: within a major version the vocabulary only
    # grows, so any later release satisfies an earlier request, but a future 2.x
    # must not silently satisfy find_package(NumericConcepts 1.0).
    #
    # ARCH_INDEPENDENT: there is nothing architecture-specific in a set of
    # headers, so a 64-bit install must not be rejected by a 32-bit consumer.
    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
        ARCH_INDEPENDENT
    )

    configure_package_config_file(
        cmake/${PROJECT_NAME}Config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )
endif()

# --- Examples, tests and documentation ---

# Called before the subdirectories are added so that examples/ can register its
# programs with CTest as well.
if(NUMERICCONCEPTS_BUILD_TESTS)
    enable_testing()
endif()

if(NUMERICCONCEPTS_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(NUMERICCONCEPTS_BUILD_DOCS)
    add_subdirectory(docs)
endif()

if(NUMERICCONCEPTS_BUILD_TESTS)
    add_subdirectory(tests)
endif()
