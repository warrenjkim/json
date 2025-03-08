add_library(json STATIC value.cc)

target_link_libraries(json PRIVATE
  tokenizer
  parser
  visitors
  json_utils
)

add_coverage_options(json)
