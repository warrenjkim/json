cmake_minimum_required(VERSION 3.10)
project(WarrenJson VERSION 1.0.0 DESCRIPTION "JSON Library")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed.")
endif()

option(BUILD_TESTING "Build tests" OFF)
option(BUILD_COVERAGE "Build with coverage flags" OFF)

# AddressSanitizer for Debug builds
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "Debug build: enabling AddressSanitizer")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
  set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(src)

if(BUILD_TESTING)
  message(STATUS "Test build: enabling tests")
  enable_testing()
  add_subdirectory(tests)
endif()

if(BUILD_COVERAGE)
  message(STATUS "Coverage build: enabling coverage report")
  add_compile_options(--coverage -O0 -g)
  add_link_options(--coverage)
  include(CodeCoverageReportConfig)
  generate_coverage_report(
    TARGETS warren_json
    TESTS
      map_test
      pair_test
      queue_test
      rbt_test
      utils_test
      value_test
      lexer_test
      parser_test
      tokenizer_test
      cmp_visitor_test
      get_visitor_test
      set_visitor_test
      stringify_visitor_test
  )
endif()
