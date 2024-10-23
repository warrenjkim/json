#pragma once

#include <string>
#include <string_view>

#include "json/json.h"
#include "nodes/node.h"
#include "parse/token.h"
#include "utils/queue.h"

namespace json {

std::optional<utils::Queue<Token>> tokenize(std::string_view json);

Node* parse(const std::string_view json);

Node* parse(utils::Queue<Token>& tokens);

Json jsonify(const std::string& json);

std::string stringify(const Json& json);

}  // namespace json
