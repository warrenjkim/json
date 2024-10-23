#pragma once

#include <string_view>

#include "nodes/node.h"
#include "parse/token.h"
#include "utils/queue.h"

namespace json {

std::optional<utils::Queue<Token>> tokenize(std::string_view json);

Node* parse(const std::string_view json);

Node* parse(utils::Queue<Token>& tokens);

}  // namespace json
