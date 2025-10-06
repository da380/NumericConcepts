cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0135 NEW)

project(
    NumericConcepts
    VERSION 0.0.1
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

# Define a header-only INTERFACE library
add_library(${PROJECT_NAME} INTERFACE)

# Specify the include directory for any consumer of this library
target_include_directories(
    ${PROJECT_NAME}
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)


# --- Installation and Packaging ---
include(CMakePackageConfigHelpers)

# Install the library target and add it to an export set.
# For an INTERFACE library, this installs no files, but makes the target available for export.
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
)

# [FIX] Install the public header files.
# This is the crucial step that makes the headers available to downstream projects.
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install the exported targets file, which references the installed headers
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# Generate and install the package version file
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Generate the main config file from a template
configure_package_config_file(
    cmake/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# Install the generated config and version files
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
# --- End of Installation and Packaging ---


# --- Standalone Project Guard for examples, docs, and tests ---
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    add_subdirectory(examples)

    #Optionally build documentation.
    option(BUILD_DOCS "Build the Doxygen documentation" OFF)

    if(BUILD_DOCS)
      find_package(Doxygen REQUIRED)
      set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
      configure_file(Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
      add_custom_target(
          doc ALL
          COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
          COMMENT "Generating API documentation with Doxygen..."
          VERBATIM
      )
    endif()

    # --- Optional GTest Integration ---
    option(BUILD_TESTS "Build the tests for NumericConcepts" OFF)

    if(BUILD_TESTS)
        enable_testing()
        include(FetchContent)

        # Tell googletest not to install itself
        set(INSTALL_GTEST OFF)

        FetchContent_Declare(
          googletest
          URL https://github.com/google/googletest/archive/v1.14.0.zip
        )
        FetchContent_MakeAvailable(googletest)

        add_subdirectory(tests)
    endif()
    # --- End of Optional GTest Integration ---

endif()
