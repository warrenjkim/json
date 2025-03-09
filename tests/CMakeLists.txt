file(GLOB_RECURSE TEST_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/json/*.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/parse/*.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/dsa/*.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/visitors/*.cc"
)

foreach(test_source ${TEST_SOURCES})
  get_filename_component(test_name ${test_source} NAME_WE)

  add_executable(${test_name} ${test_source})

  target_link_libraries(${test_name} PRIVATE
    json_impl
    json_test_includes
    gtest
    gtest_main
  )

  add_coverage_options(${test_name})

  add_test(NAME ${test_name} COMMAND ${test_name})

  gtest_discover_tests(${test_name})
endforeach()
