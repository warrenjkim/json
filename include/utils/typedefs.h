#pragma once

namespace json {

enum class TokenType {
  OBJECT_START,
  OBJECT_END,
  ARRAY_START,
  ARRAY_END,
  QUOTE,
  COMMA,
  COLON,
  STRING,
  NUMBER,
  BOOLEAN,
  JSON_NULL,
  SOLIDUS,
  UNKNOWN,
  END_OF_JSON
};

}  // namespace json
