#pragma once

#include <string_view>

#include "json/value.h"
#include "parse/token.h"
#include "utils/queue.h"

// TODO(are these actually necessary?)

namespace json {

std::optional<utils::Queue<Token>> tokenize(std::string_view json);

Value parse(const std::string_view json);

Value parse(utils::Queue<Token>& tokens);

std::string to_string(const Value& value);

}  // namespace json
