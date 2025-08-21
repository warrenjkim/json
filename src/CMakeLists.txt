file(GLOB_RECURSE JSON_SOURCES
  "json/*.cc"
  "nodes/*.cc"
  "parse/*.cc"
  "dsa/*.cc"
)

add_library(warren_json ${JSON_SOURCES})

target_include_directories(warren_json
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include>
)

# Apply coverage flags if needed
if(BUILD_COVERAGE)
  target_compile_options(warren_json PRIVATE --coverage)
  target_link_options(warren_json PRIVATE --coverage)
endif()

install(TARGETS warren_json
  EXPORT WarrenJsonTargets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY ../include/ DESTINATION include)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/WarrenJsonConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMinorVersion
)
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/WarrenJsonConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/WarrenJsonConfig.cmake"
  INSTALL_DESTINATION lib/cmake/Warren
)
install(EXPORT WarrenJsonTargets
  FILE WarrenJsonTargets.cmake
  NAMESPACE Warren::
  DESTINATION lib/cmake/Warren
)
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/WarrenJsonConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/WarrenJsonConfigVersion.cmake"
  DESTINATION lib/cmake/Warren
)
