# Globbed with CONFIGURE_DEPENDS so that a newly added example is picked up
# without anyone having to remember to list it here.
file(GLOB _example_sources CONFIGURE_DEPENDS
     ${CMAKE_CURRENT_SOURCE_DIR}/[0-9][0-9]-*.cpp)
list(SORT _example_sources)

if(NOT _example_sources)
    message(FATAL_ERROR "No examples found in ${CMAKE_CURRENT_SOURCE_DIR}.")
endif()

# The targets carry the project prefix, so that they cannot collide with a
# consumer's own targets in a superbuild; the programs keep their plain names.
foreach(_source ${_example_sources})
    get_filename_component(_name ${_source} NAME_WE)
    set(_target numericconcepts_${_name})
    add_executable(${_target} ${_source})
    set_target_properties(${_target} PROPERTIES OUTPUT_NAME ${_name})
    target_link_libraries(${_target} PRIVATE NumericConcepts::NumericConcepts)

    # Run the examples alongside the tests. They assert as much as they
    # illustrate, so an example that stops working is a failure worth seeing.
    if(NUMERICCONCEPTS_BUILD_TESTS)
        add_test(NAME example.${_name} COMMAND ${_target})
    endif()
endforeach()
