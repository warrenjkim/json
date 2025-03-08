#pragma once

#include <string_view>

#include "json/value.h"
#include "nodes/node.h"
#include "parse/token.h"
#include "utils/queue.h"

namespace json {

std::optional<utils::Queue<Token>> tokenize(std::string_view json);

Value parse(const std::string_view json);

Value parse(utils::Queue<Token>& tokens);

std::string to_string(const Value& value);

nodes::Array* Array();

nodes::Boolean* Boolean();

nodes::Null* Null();

nodes::Number* Number();

nodes::Object* Object();

nodes::String* String();

}  // namespace json
