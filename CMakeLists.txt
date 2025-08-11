cmake_minimum_required(VERSION 3.15)

project(
    NumericConcepts
    VERSION 0.0.1
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Define a header-only INTERFACE library
add_library(${PROJECT_NAME} INTERFACE)

# Specify the include directory for any consumer of this library
target_include_directories(
    ${PROJECT_NAME}
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Optionally, add examples for local development
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    add_subdirectory(examples)
endif()




if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)

#Optionally build documentation. 
option(BUILD_DOCS "Build the Doxygen documentation" OFF)

if(BUILD_DOCS)
  find_package(Doxygen REQUIRED)

  # Configure the Doxyfile using the template
  set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
  configure_file(Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

  # Add a target to build the documentation
  add_custom_target(
      doc ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen..."
      VERBATIM
  )

endif()

endif()


