#pragma once

#include <string_view>

#include "parse/token.h"
#include "types/type.h"
#include "utils/queue.h"

namespace json {

std::optional<utils::Queue<Token>> tokenize(std::string_view json);

Type* parse(const std::string_view json);

Type* parse(utils::Queue<Token>& tokens);

}  // namespace json
