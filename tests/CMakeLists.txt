# Prefer a GTest that is already available; fall back to fetching one.
find_package(GTest QUIET)

if(NOT GTest_FOUND)
    include(FetchContent)

    # Suppresses the "timestamps of extracted files" warning; the policy is only
    # known to CMake 3.24 and later.
    if(POLICY CMP0135)
        cmake_policy(SET CMP0135 NEW)
    endif()

    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
        URL_HASH SHA256=7b42b4d6ed48810c5362c265a17faebe90dc2373c885e5216439d37927f02926
    )
    FetchContent_MakeAvailable(googletest)
endif()

add_executable(run_tests
    test_numeric.cpp
    test_algebra.cpp
    test_iterators.cpp
    test_ranges.cpp
    test_functions.cpp
    test_version.cpp
)

target_link_libraries(run_tests PRIVATE
    GTest::gtest_main
    NumericConcepts::NumericConcepts
)

include(GoogleTest)
gtest_discover_tests(run_tests)

# The headers are self-contained: each must compile on its own, with no other
# NumericConcepts header included first.
foreach(_header Numeric Algebra Iterators Ranges Functions Version
                NumericConcepts)
    set(_src ${CMAKE_CURRENT_BINARY_DIR}/self_contained_${_header}.cpp)
    file(WRITE ${_src} "#include <NumericConcepts/${_header}.hpp>\n")
    list(APPEND _self_contained_sources ${_src})
endforeach()

add_library(test_self_contained_headers OBJECT ${_self_contained_sources})
target_link_libraries(test_self_contained_headers PRIVATE NumericConcepts::NumericConcepts)
