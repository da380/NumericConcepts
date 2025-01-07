cmake_minimum_required(VERSION 3.15)

project(
    NC
    VERSION 0.0.1
    LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)    

add_library(${PROJECT_NAME} INTERFACE)

option(BUILD_EXAMPLES "Build the example programs" ON)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

include(GNUInstallDirs)

target_include_directories(
    ${PROJECT_NAME}
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-config)


install(
    EXPORT ${PROJECT_NAME}-config 
    NAMESPACE ${PROJECT_NAME}:: 
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/${PROJECT_NAME}/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})


