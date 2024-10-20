add_library(logger STATIC logger.cc)
add_library(json_utils STATIC json_utils.cc)

target_link_libraries(logger PRIVATE
  Boost::log
  Boost::log_setup
)

add_coverage_options(logger)
add_coverage_options(json_utils)
