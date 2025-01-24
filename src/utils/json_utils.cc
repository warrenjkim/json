#include "utils/json_utils.h"

#include <optional>
#include <string_view>

#include "json/value.h"
#include "parse/parser.h"
#include "parse/token.h"
#include "parse/tokenizer.h"
#include "utils/queue.h"

namespace json {

std::optional<utils::Queue<Token>> tokenize(std::string_view json) {
  return Tokenizer::tokenize(json);
}

Value parse(const std::string_view json) { return Value(Parser::parse(json)); }

Value parse(utils::Queue<Token>& tokens) {
  return Value(Parser::parse(tokens));
}

Value to_string(const Value& value) { return value.to_string(); }

}  // namespace json
