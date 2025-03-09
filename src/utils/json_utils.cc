#include <optional>
#include <string_view>

#include "json/utils.h"
#include "json/value.h"
#include "nodes/array.h"
#include "nodes/boolean.h"
#include "nodes/node.h"
#include "nodes/null.h"
#include "nodes/number.h"
#include "nodes/object.h"
#include "nodes/string.h"
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

std::string to_string(const Value& value) { return value.to_string(); }

nodes::Array* Array() { return new nodes::Array(); }

nodes::Boolean* Boolean() { return new nodes::Boolean(false); }

nodes::Null* Null() { return new nodes::Null(); }

nodes::Number* Number() { return new nodes::Number(0); }

nodes::Object* Object() { return new nodes::Object(); }

nodes::String* String() { return new nodes::String(""); }

}  // namespace json
