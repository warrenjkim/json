add_library(visitors STATIC
  cmp_visitor.cc
  get_visitor.cc
)

target_link_libraries(visitors PRIVATE
  logger
  nodes
  Boost::log
  Boost::log_setup
)

add_coverage_options(visitors)

