add_library(visitors STATIC
  cmp_visitor.cc
  get_visitor.cc
  array_visitor.cc
  boolean_visitor.cc
  number_visitor.cc
  null_visitor.cc
  object_visitor.cc
  string_visitor.cc
)

target_link_libraries(visitors PRIVATE
  logger
  nodes
  Boost::log
  Boost::log_setup
)

add_coverage_options(visitors)

