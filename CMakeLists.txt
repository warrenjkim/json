cmake_minimum_required(VERSION 3.10)

project(JsonParser VERSION 1.0 DESCRIPTION "JSON Parser")

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

if (CMAKE_BUILD_TYPE STREQUAL "")
  message(STATUS "Build type set to default")
  set(CMAKE_BUILD_TYPE Debug)
endif()

# output binaries to a subdirectory "bin"
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable CMake testing
include(CTest)
enable_testing()

# enable GoogleTest
include(GoogleTest)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/googletest googletest)
set(GTEST_OUTPUT "all")

# Enable Boost
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.50 REQUIRED COMPONENTS
  log
  log_setup
)
message(STATUS "Boost version: ${Boost_VERSION}")
message(STATUS "Boost include: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost lib: ${Boost_LIBRARY_DIRS}")

# Check for -fno-elide-constructors support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-fno-elide-constructors" COMPILER_SUPPORTS_NO_ELIDE)

# Set optimization flags based on build type
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_COVERAGE "-O0 -fno-elide-constructors")

# ===== DIRECTORIES =====

# Add directory containing header files
include_directories(include)

# ===== LIBRARIES =====

# function to create a library
function(create_lib src_name src_dir)
  add_library(${src_name} STATIC ${src_dir}/${src_name}.cc)
  target_include_directories(${src_name} PUBLIC ${src_dir})
  target_link_libraries(${src_name} PRIVATE Boost::log Boost::log_setup)

  # Add coverage flags if building for coverage
  if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
    target_compile_options(${src_name} PRIVATE -g --coverage -O0)
    target_link_options(${src_name} PRIVATE -g --coverage)
    if(COMPILER_SUPPORTS_NO_ELIDE)
      target_compile_options(${src_name} PRIVATE -fno-elide-constructors)
    endif()
  endif()
endfunction()

# parse libraries
create_lib(tokenizer "src/parse")
create_lib(parser "src/parse")

# util libraries
create_lib(logger "src/utils")

# node libraries
add_library(types STATIC
  src/types/array.cc
  src/types/boolean.cc
  src/types/type.cc
  src/types/null.cc
  src/types/number.cc
  src/types/object.cc
  src/types/string.cc
)
target_include_directories(types PUBLIC src/types)

# visitor libraries
add_library(visitors STATIC
  src/visitors/cmp_visitor.cc
)
target_include_directories(visitors PUBLIC src/node/visitors)

#json libraries
create_lib(json_utils "src/utils")

# link the logger library to the necessary targets
target_link_libraries(tokenizer PRIVATE logger)
target_link_libraries(parser PRIVATE logger tokenizer types)
target_link_libraries(visitors PRIVATE logger types)

# ===== EXECUTABLES =====

# function to create an executable test
function(create_test_executable test_name file_name test_dir)
  add_executable(${test_name} ${test_dir}/${file_name}.cc)
  target_link_libraries(${test_name} PRIVATE gtest_main Boost::log Boost::log_setup ${ARGN})

  if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
    target_compile_options(${test_name} PRIVATE -g --coverage -O0)
    target_link_options(${test_name} PRIVATE -g --coverage)
    if(COMPILER_SUPPORTS_NO_ELIDE)
      target_compile_options(${test_name} PRIVATE -fno-elide-constructors)
    endif()
  endif()

  gtest_discover_tests(${test_name}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${test_dir}
    PROPERTIES ENVIRONMENT "GTEST_COLOR=1"
  )
endfunction()

# Create test executables
create_test_executable(tokenizer_test "tokenizer_test" "tests/parse" tokenizer)
create_test_executable(parser_test "parser_test" "tests/parse" parser)
create_test_executable(trie_test "trie_test" "tests/utils" logger)
create_test_executable(stack_test "stack_test" "tests/utils" logger)
create_test_executable(queue_test "queue_test" "tests/utils" logger)
create_test_executable(pair_test "pair_test" "tests/utils" logger)
create_test_executable(rbt_test "rbt_test" "tests/utils" logger)
create_test_executable(cmp_visitor_test "cmp_visitor_test" "tests/visitors" visitors)

# Link the logger library to the test executable
target_link_libraries(tokenizer_test PRIVATE logger)
target_link_libraries(parser_test PRIVATE tokenizer logger types visitors)
target_link_libraries(trie_test PRIVATE logger)
target_link_libraries(stack_test PRIVATE logger)
target_link_libraries(queue_test PRIVATE logger)
target_link_libraries(pair_test PRIVATE logger)
target_link_libraries(rbt_test PRIVATE logger)
target_link_libraries(cmp_visitor_test PRIVATE logger types visitors)

# ===== CODE COVERAGE =====

include(cmake/CodeCoverageReportConfig.cmake)
generate_coverage_report(
  TARGETS
  tokenizer
  parser
  visitors

  TESTS
  tokenizer_test
  parser_test
  trie_test
  stack_test
  queue_test
  pair_test
  rbt_test
  cmp_visitor_test
)
