add_library(types STATIC
  array.cc
  boolean.cc
  type.cc
  null.cc
  number.cc
  object.cc
  string.cc
)

target_link_libraries(types PRIVATE
  Boost::log
  Boost::log_setup
)

add_coverage_options(types)
