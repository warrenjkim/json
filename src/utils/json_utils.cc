#include "utils/json_utils.h"

#include <optional>
#include <string_view>

#include "parse/parser.h"
#include "parse/token.h"
#include "parse/tokenizer.h"
#include "types/type.h"
#include "utils/queue.h"

namespace json {

std::optional<utils::Queue<Token>> tokenize(std::string_view json) {
  return Tokenizer::tokenize(json);
}

Type* parse(const std::string_view json) { return Parser::parse(json); }

Type* parse(utils::Queue<Token>& tokens) { return Parser::parse(tokens); }

}  // namespace json
