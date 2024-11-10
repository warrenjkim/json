function(create_test test_name)
  add_executable(${test_name} ${test_name}.cc)
  target_include_directories(${test_name} PRIVATE ${CMAKE_SOURCE_DIR}/include)
  target_link_libraries(${test_name} PRIVATE
    gtest_main
    Boost::log
    Boost::log_setup
    ${ARGN}
  )

  if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
    target_compile_options(${test_name} PRIVATE -g --coverage -O0)
    target_link_options(${test_name} PRIVATE -g --coverage)
    if(COMPILER_SUPPORTS_NO_ELIDE)
      target_compile_options(${test_name} PRIVATE -fno-elide-constructors)
    endif()
  endif()

  gtest_discover_tests(${test_name}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTIES ENVIRONMENT "GTEST_COLOR=1"
  )
endfunction()

add_subdirectory(parse)
add_subdirectory(utils)
add_subdirectory(visitors)
add_subdirectory(json)
