#include "warren/json/utils.h"

#include <optional>
#include <string_view>

#include "warren/internal/dsa/queue.h"
#include "warren/internal/nodes/array.h"
#include "warren/internal/nodes/boolean.h"
#include "warren/internal/nodes/node.h"
#include "warren/internal/nodes/null.h"
#include "warren/internal/nodes/number.h"
#include "warren/internal/nodes/object.h"
#include "warren/internal/nodes/string.h"
#include "warren/internal/parse/parser.h"
#include "warren/internal/parse/token.h"
#include "warren/internal/parse/tokenizer.h"
#include "warren/json/value.h"

namespace json {

std::optional<dsa::Queue<Token>> tokenize(std::string_view json) {
  return Tokenizer::tokenize(json);
}

Value parse(const std::string_view json) { return Value(Parser::parse(json)); }

Value parse(dsa::Queue<Token>& tokens) {
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
