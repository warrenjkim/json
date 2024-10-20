add_library(tokenizer STATIC tokenizer.cc)
add_library(parser STATIC parser.cc)

target_link_libraries(tokenizer PRIVATE
  logger
  Boost::log
  Boost::log_setup
)
target_link_libraries(parser PRIVATE
  logger
  tokenizer
  types
  Boost::log
  Boost::log_setup
)

add_coverage_options(tokenizer)
add_coverage_options(parser)
