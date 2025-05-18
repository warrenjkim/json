enable_testing()

# Output binaries into bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
set(INSTALL_GTEST OFF CACHE BOOL "Disable installation of googletest" FORCE)
FetchContent_MakeAvailable(googletest)

# Load GTest helpers
include(GoogleTest)

# Helper function to add unit tests
function(add_unit_test test_name relative_path)
  add_executable(${test_name} ${CMAKE_CURRENT_SOURCE_DIR}/${relative_path})
  target_link_libraries(${test_name} PRIVATE warren_json GTest::gtest_main)

  # Apply coverage flags if needed
  if(BUILD_COVERAGE)
    target_compile_options(${test_name} PRIVATE --coverage)
    target_link_options(${test_name} PRIVATE --coverage)
  endif()

  gtest_discover_tests(${test_name})
endfunction()

add_unit_test(map_test dsa/map_test.cc)
add_unit_test(pair_test dsa/pair_test.cc)
add_unit_test(queue_test dsa/queue_test.cc)
add_unit_test(numeric_test dsa/numeric_test.cc)
add_unit_test(rbt_test dsa/rbt_test.cc)
add_unit_test(utils_test json/utils_test.cc)
add_unit_test(value_test json/value_test.cc)
add_unit_test(lexer_test parse/lexer_test.cc)
add_unit_test(parser_test parse/parser_test.cc)
add_unit_test(tokenizer_test parse/tokenizer_test.cc)
add_unit_test(cmp_visitor_test visitors/cmp_visitor_test.cc)
add_unit_test(get_visitor_test visitors/get_visitor_test.cc)
add_unit_test(set_visitor_test visitors/set_visitor_test.cc)
add_unit_test(stringify_visitor_test visitors/stringify_visitor_test.cc)

