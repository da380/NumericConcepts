# set the minimum required version of cmake to use
# needed to set version and policy settings before invoking other commands
cmake_minimum_required(VERSION 3.15)

# Set the minimum standard for c++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)    

# set the name of the project
# must immediately follow cmake_minimum_required command
project(
    app1
    VERSION 0.0.1
    LANGUAGES CXX)


include(FetchContent)
FetchContent_Declare(
  NumericConcepts
  GIT_REPOSITORY https://github.com/da380/NumericConcepts.git
  GIT_TAG main
)
FetchContent_MakeAvailable(NumericConcepts)


# add an executable target
add_executable(${PROJECT_NAME})

# add the source files to build
target_sources(${PROJECT_NAME} PRIVATE main.cpp)

# link the executable target against the imported target
# must prefix the target with the namespace set during install
target_link_libraries(${PROJECT_NAME} PUBLIC NumericConcepts)