cmake_minimum_required(VERSION 3.10)
project(JsonParser VERSION 1.0.1 DESCRIPTION "JSON Parser")

# cmake flags
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CTEST_ARGUMENTS "${CMAKE_CTEST_ARGUMENTS};--output-on-failure;--force-new-ctest-process;--verbose")

# C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# no cmake in the main project directory
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "dumbass. lol")
endif()

# build type
if (CMAKE_BUILD_TYPE STREQUAL "")
  message(STATUS "Build type set to default")
  set(CMAKE_BUILD_TYPE Debug)
endif()

option(BUILD_RELEASE "Build production-ready release" OFF)
if(BUILD_RELEASE)
 set(CMAKE_BUILD_TYPE Release)
 add_compile_definitions(NDEBUG)
endif()

# implementation
add_subdirectory(src)

# library interface (only json.h)
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# Link implementation to interface
target_link_libraries(${PROJECT_NAME} INTERFACE json)

set_target_properties(json PROPERTIES INTERFACE_LINK_LIBRARIES "")

# installation rules
install(TARGETS ${PROJECT_NAME} json EXPORT ${PROJECT_NAME}Config)
install(FILES include/json.h DESTINATION include)
install(EXPORT ${PROJECT_NAME}Config DESTINATION lib/cmake/${PROJECT_NAME})

# testing
option(BUILD_TESTING "Build tests" OFF)
if(BUILD_TESTING)
  # output binaries to a subdirectory "bin"
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_BUILD_TYPE Coverage)

  # enable CMake testing
  include(CTest)
  enable_testing()

  # enable GoogleTest
  include(GoogleTest)
  if(NOT TARGET gtest AND NOT TARGET gtest_main)
    # only include GoogleTest if targets don't already exist
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/googletest googletest)
    set(GTEST_OUTPUT "all")
  endif()

  # check for -fno-elide-constructors support
  include(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG("-fno-elide-constructors" COMPILER_SUPPORTS_NO_ELIDE)

  # set optimization flags based on build type
  set(CMAKE_CXX_FLAGS_COVERAGE "-O0 -fno-elide-constructors -fsanitize=address")

  # add test directory
  add_subdirectory(tests)

  # coverage
  include(cmake/CodeCoverageReportConfig.cmake)
  generate_coverage_report(
    TARGETS
      tokenizer
      parser
      visitors
      json
    TESTS
      tokenizer_test
      parser_test
      queue_test
      pair_test
      map_test
      rbt_test
      cmp_visitor_test
      get_visitor_test
      set_visitor_test
      array_visitor_test
      boolean_visitor_test
      null_visitor_test
      number_visitor_test
      object_visitor_test
      string_visitor_test
      value_test
      json_utils_test
      stringify_visitor_test
  )
endif()

# set optimization flags based on build type
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -fsanitize=address -fno-omit-frame-pointer")
