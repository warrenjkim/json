cmake_minimum_required(VERSION 3.10)
project(JsonParser VERSION 1.0.1 DESCRIPTION "JSON Parser Library")

# Basic settings
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Prevent in-source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Please create a build directory.")
endif()

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Build type set to default (Debug)")
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Build options
option(BUILD_RELEASE "Build production-ready release" OFF)
if(BUILD_RELEASE)
  set(CMAKE_BUILD_TYPE Release)
  add_compile_definitions(NDEBUG)
endif()

# Compiler flags
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -fsanitize=address -fno-omit-frame-pointer")

# Collect all source files
file(GLOB_RECURSE JSON_SOURCES
  "src/json/*.cc"
  "src/nodes/*.cc"
  "src/parse/*.cc"
  "src/utils/*.cc"
  "src/visitors/*.cc"
)

# Create the implementation library
add_library(json_impl STATIC ${JSON_SOURCES})

# Create the public interface library that users will link against
add_library(json INTERFACE)

# Set include directories for implementation
target_include_directories(json_impl PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Create an interface library for test includes
add_library(json_test_includes INTERFACE)
target_include_directories(json_test_includes INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Set include directories for users
target_include_directories(json INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# Link implementation to interface
target_link_libraries(json INTERFACE json_impl)

# Installation
install(TARGETS json json_impl
  EXPORT JsonConfig
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include/
  DESTINATION include
  FILES_MATCHING PATTERN "*.h" PATTERN "*.inl"
)

# Install config
install(EXPORT JsonConfig
  NAMESPACE Warren::
  DESTINATION lib/cmake/Warren
)

# Testing
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  option(BUILD_TESTING "Build tests" OFF)
else()
  # Force OFF when used as a subproject
  set(BUILD_TESTING OFF CACHE BOOL "Build tests" FORCE)
endif()
if(BUILD_TESTING)
  # Output binaries to a subdirectory "bin"
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_BUILD_TYPE Coverage)

  # Enable CMake testing
  include(CTest)
  enable_testing()

  # Enable GoogleTest
  include(GoogleTest)
  if(NOT TARGET gtest AND NOT TARGET gtest_main)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/googletest googletest)
    set(GTEST_OUTPUT "all")
  endif()

  # Check for -fno-elide-constructors support
  include(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG("-fno-elide-constructors" COMPILER_SUPPORTS_NO_ELIDE)

  # Set optimization flags for coverage
  set(CMAKE_CXX_FLAGS_COVERAGE "-O0 -fsanitize=address")
  if(COMPILER_SUPPORTS_NO_ELIDE)
    set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_COVERAGE} -fno-elide-constructors")
  endif()

  # Helper function for test targets to inherit coverage settings
  function(add_coverage_options target)
    if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
      target_compile_options(${target} PRIVATE -g --coverage -O0)
      target_link_options(${target} PRIVATE -g --coverage)
      if(COMPILER_SUPPORTS_NO_ELIDE)
        target_compile_options(${target} PRIVATE -fno-elide-constructors)
      endif()
    endif()
  endfunction()

  # Add test directory
  add_subdirectory(tests)

  # Coverage reporting
  set(CMAKE_CTEST_ARGUMENTS "${CMAKE_CTEST_ARGUMENTS};--output-on-failure;--force-new-ctest-process;--verbose")
  include(cmake/CodeCoverageReportConfig.cmake)
  generate_coverage_report(
    TARGETS
      json_impl
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
      stringify_visitor_test
      utils_test
      value_test
  )
endif()
