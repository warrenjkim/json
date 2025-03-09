add_library(visitors STATIC
  cmp_visitor.cc
  get_visitor.cc
  set_visitor.cc
  array_visitor.cc
  boolean_visitor.cc
  number_visitor.cc
  null_visitor.cc
  object_visitor.cc
  string_visitor.cc
  stringify_visitor.cc
  container_type_visitor.cc
)

target_link_libraries(visitors PRIVATE
  nodes
  json_internal_includes
)

add_coverage_options(visitors)
