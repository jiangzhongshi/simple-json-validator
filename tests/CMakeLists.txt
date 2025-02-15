################################################################################
# Tests
################################################################################

set(test_sources
	main.cpp
	test_validator.cpp
)
add_executable(unit_tests ${test_sources})

################################################################################
# Required Libraries
################################################################################

include(catch2)
target_link_libraries(unit_tests PUBLIC Catch2::Catch2)

target_link_libraries(unit_tests PUBLIC sjv::sjv)

include(sjv_warnings)
target_link_libraries(unit_tests PRIVATE sjv::warnings)

if(POLYSOLVE_WITH_SANITIZERS)
	add_sanitizers(unit_tests)
endif()

################################################################################
# Register tests
################################################################################

foreach(source IN ITEMS ${test_sources})
    source_group("tests" FILES "${source}")
endforeach()

# Register tests
set(PARSE_CATCH_TESTS_ADD_TO_CONFIGURE_DEPENDS ON)
catch_discover_tests(unit_tests)
