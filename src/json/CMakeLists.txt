add_library(json STATIC value.cc)

target_link_libraries(json PRIVATE
  Boost::log
  Boost::log_setup
  logger
  nodes
  tokenizer
  parser
  visitors
  json_utils
)

add_coverage_options(json)
