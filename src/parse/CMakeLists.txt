add_library(tokenizer STATIC tokenizer.cc)
add_library(parser STATIC parser.cc)

target_link_libraries(parser PRIVATE
  tokenizer
  nodes
)

add_coverage_options(tokenizer)
add_coverage_options(parser)
