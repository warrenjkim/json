add_library(json_utils STATIC json_utils.cc)

target_link_libraries(json_utils PRIVATE
  tokenizer
  parser
  json_internal_includes
)

add_coverage_options(json_utils)
