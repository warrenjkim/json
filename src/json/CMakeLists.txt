add_library(json STATIC value.cc)

target_link_libraries(json PRIVATE
  tokenizer
  parser
  visitors
  json_utils
  json_internal_includes
)

add_coverage_options(json)
