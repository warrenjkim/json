#pragma once

#include "warren/json/internal/ast/node.h"
#include "warren/json/value.h"

namespace json {
namespace convert {

Value lower(const ast::Node& root);

}  // namespace convert
}  // namespace json
