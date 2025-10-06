# Create an executable for the tests
add_executable(run_tests
    test_numeric.cpp
    test_iterators.cpp
    test_ranges.cpp
    test_functions.cpp
)

# Link the test executable against gtest and your library
target_link_libraries(run_tests PRIVATE
    GTest::gtest_main
    NumericConcepts
)

# Automatically discover and add tests to CTest
include(GoogleTest)
gtest_discover_tests(run_tests)