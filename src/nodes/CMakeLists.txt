add_library(nodes STATIC
  array.cc
  boolean.cc
  node.cc
  null.cc
  number.cc
  object.cc
  string.cc
)

target_link_libraries(nodes PRIVATE
  Boost::log
  Boost::log_setup
  visitors
)

add_coverage_options(nodes)
