include_directories(${CMAKE_SOURCE_DIR}/include)

# Common function for coverage options
function(add_coverage_options target)
  if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
    target_compile_options(${target} PRIVATE -g --coverage -O0)
    target_link_options(${target} PRIVATE -g --coverage)
    if(COMPILER_SUPPORTS_NO_ELIDE)
      target_compile_options(${target} PRIVATE -fno-elide-constructors)
    endif()
  endif()
endfunction()

add_subdirectory(parse)
add_subdirectory(nodes)
add_subdirectory(utils)
add_subdirectory(visitors)
