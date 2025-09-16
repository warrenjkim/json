#pragma once

#include "warren/json/internal/ast/node.h"
#include "warren/json/value.h"

namespace json {
namespace convert {

// TODO(consider using a ref instead of a ptr here)
Value lower(ast::Node* root);

}  // namespace convert
}  // namespace json
