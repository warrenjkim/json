#include "warren/internal/parse/parser.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

#include "warren/internal/dsa/numeric.h"
#include "warren/internal/nodes/node.h"
#include "warren/internal/nodes/object.h"
#include "warren/internal/parse/lexer.h"
#include "warren/internal/parse/token.h"
#include "warren/json/exception.h"

namespace {

uint32_t to_code_point(const char c1, const char c2, const char c3,
                       const char c4) {
  auto to_uint32 = [](const char c) -> uint32_t {
    if (c >= '0' && c <= '9') {
      return uint32_t(c - '0');
    } else if (c >= 'A' && c <= 'F') {
      return uint32_t(c - 'A' + 10);
    } else if (c >= 'a' && c <= 'f') {
      return uint32_t(c - 'a' + 10);
    }

    return 0;
  };

  uint32_t code_point = 0;
  code_point = (code_point << 4) | to_uint32(c1);
  code_point = (code_point << 4) | to_uint32(c2);
  code_point = (code_point << 4) | to_uint32(c3);
  code_point = (code_point << 4) | to_uint32(c4);

  return code_point;
}

// https://www.ietf.org/rfc/rfc3629.txt
void emit_utf8(uint32_t code_point, std::string& res) {
  if (code_point > 0x10FFFF) {
    throw json::ParseException("Code point out of Unicode range (> 0x10FFFF)");
  } else if (0xD800 <= code_point && code_point <= 0xDFFF) {
    throw json::ParseException(
        "Code point in surrogate range [0xD800, 0xDFFF]");
  }

  if (code_point < 0x80) {
    res += (char)(code_point);
  } else if (code_point < 0x800) {
    res += (char)(0xC0 | (code_point >> 6));
    res += (char)(0x80 | (code_point & 0x3F));
  } else if (code_point < 0x10000) {
    res += (char)(0xE0 | (code_point >> 12));
    res += (char)(0x80 | ((code_point >> 6) & 0x3F));
    res += (char)(0x80 | (code_point & 0x3F));
  } else {
    res += (char)(0xF0 | (code_point >> 18));
    res += (char)(0x80 | ((code_point >> 12) & 0x3F));
    res += (char)(0x80 | ((code_point >> 6) & 0x3F));
    res += (char)(0x80 | (code_point & 0x3F));
  }
}

void resolve_unicode_sequences(const std::string& s, std::string& res) {
  res.reserve(s.length());
  size_t i = 0;
  size_t j = 0;
  while (j < s.length()) {
    if (s[j] == '\\' && s[j + 1] == 'u') {
      res.append(s, i, j - i);
      j += 6;

      uint32_t code_point =
          to_code_point(s[j - 4], s[j - 3], s[j - 2], s[j - 1]);

      // Section 3.8 Surrogates
      // https://www.unicode.org/versions/Unicode15.0.0/ch03.pdf
      // 0xD800 <= high-surrogate code point <= 0xDBFF
      uint16_t high_surrogate_cp = code_point;
      if (0xD800 <= high_surrogate_cp && high_surrogate_cp <= 0xDBFF) {
        j += 6;
        if (j + 1 >= s.length() || s[j] != '\\' || s[j + 1] != 'u') {
          throw json::ParseException(
              "Expected low surrogate after high surrogate: " +
              s.substr(j - 6, 6));
        }

        // 0xDC00 <= low-surrogate code point <= 0xDFFF
        uint16_t low_surrogate_cp =
            to_code_point(s[j - 4], s[j - 3], s[j - 2], s[j - 1]);
        if (!(0xDC00 <= low_surrogate_cp && low_surrogate_cp <= 0xDFFF)) {
          throw json::ParseException(
              "Invalid low surrogate (" + s.substr(j - 6, 6) +
              ") after high surrogate: " + s.substr(j - 12, 6));
        }

        code_point = 0x10000 + ((high_surrogate_cp - 0xD800) << 10) +
                     (low_surrogate_cp - 0xDC00);
      }

      emit_utf8(code_point, res);
      i = j;
    }

    j++;
  }

  if (i < s.length()) {
    res.append(s, i, s.length() - i);
  }
}

}  // namespace

namespace json {

namespace syntax {

Parser::Parser(Lexer&& lexer) : lexer_(std::move(lexer)), root_(nullptr) {}

nodes::Node* Parser::parse() {
  ++lexer_;
  nodes::Node* json = parse_value();
  if (lexer_->type != TokenType::END_OF_JSON) {
    throw ParseException("Unexpected token: " + lexer_->value);
  }

  return json;
}

nodes::Node* Parser::parse_value() {
  switch (lexer_->type) {
    case TokenType::BOOLEAN:
      return parse_boolean();
    case TokenType::JSON_NULL:
      return parse_null();
    case TokenType::STRING:
      return parse_string();
    case TokenType::NUMBER:
      return parse_number();
    case TokenType::ARRAY_START:
      return parse_array();
    case TokenType::OBJECT_START:
      return parse_object();
    default:
      throw ParseException("Unexpected token: " + lexer_->value);
  }
}

nodes::Null* Parser::parse_null() {
  if (lexer_->type != TokenType::JSON_NULL) {
    throw ParseException("Unexpected token: " + lexer_->value);
  }

  ++lexer_;

  return new nodes::Null();
}

nodes::Boolean* Parser::parse_boolean() {
  if (lexer_->type != TokenType::BOOLEAN) {
    throw ParseException("Unexpected token: " + lexer_->value);
  }

  bool value = lexer_->value == "true";
  ++lexer_;

  return new nodes::Boolean(value);
}

nodes::String* Parser::parse_string() {
  if (lexer_->type != TokenType::STRING) {
    throw ParseException("Unexpected token: " + lexer_->value);
  }

  std::string value = "";
  resolve_unicode_sequences(lexer_->value, value);
  ++lexer_;

  return new nodes::String(value);
}

nodes::Number* Parser::parse_number() {
  if (lexer_->type != TokenType::NUMBER) {
    throw ParseException("Unexpected token: " + lexer_->value);
  }

  std::string_view number = lexer_->value;
  // integer
  size_t i = 0;
  if (number[i] == '-') {
    i++;
  }

  while (i < number.length() && isdigit(number[i])) {
    i++;
  }

  // fraction
  size_t j = i;
  if (j < number.length() && number[j] == '.') {
    j++;
    while (j < number.length() && isdigit(number[j])) {
      j++;
    }
  }

  // exponent
  size_t k = j;
  if (k < number.length() && tolower(number[k]) == 'e') {
    k++;
    if (number[k] == '+' || number[k] == '-') {
      k++;
    }

    while (k < number.length() && isdigit(number[k])) {
      k++;
    }
  }

  std::string_view integer = number.substr(0, i);

  std::string_view fraction = "";
  if (j > i + 1) {
    fraction = number.substr(i + 1, j - i - 1);
  }

  std::string_view exponent = "";
  if (k > j + 1) {
    exponent = number.substr(j + 1, k - j - 1);
  }

  nodes::Number* value =
      new nodes::Number(dsa::to_numeric(integer, fraction, exponent));
  ++lexer_;

  return value;
}

nodes::Array* Parser::parse_array() {
  if (lexer_->type != TokenType::ARRAY_START) {
    throw ParseException("Unexpected token: " + lexer_->value);
  }

  ++lexer_;

  nodes::Array* value = new nodes::Array();
  if (lexer_->type == TokenType::ARRAY_END) {
    ++lexer_;
    return value;
  }

  while (true) {
    value->push_back(parse_value());
    if (lexer_->type == TokenType::ARRAY_END) {
      break;
    }

    if (lexer_->type != TokenType::COMMA) {
      throw ParseException("Unexpected token: " + lexer_->value);
    }

    ++lexer_;
  }

  if (lexer_->type != TokenType::ARRAY_END) {
    throw ParseException("Unexpected token: " + lexer_->value);
  }

  ++lexer_;

  return value;
}

nodes::Object* Parser::parse_object() {
  if (lexer_->type != TokenType::OBJECT_START) {
    throw ParseException("Unexpected token: " + lexer_->value);
  }

  ++lexer_;

  nodes::Object* value = new nodes::Object();
  if (lexer_->type == TokenType::OBJECT_END) {
    ++lexer_;
    return value;
  }

  while (true) {
    nodes::String* key = parse_string();
    if (lexer_->type != TokenType::COLON) {
      throw ParseException("Unexpected token: " + lexer_->value);
    }

    ++lexer_;
    value->insert(key->get(), parse_value());
    delete key;
    if (lexer_->type == TokenType::OBJECT_END) {
      break;
    }

    if (lexer_->type != TokenType::COMMA) {
      throw ParseException("Unexpected token: " + lexer_->value);
    }

    ++lexer_;
  }

  if (lexer_->type != TokenType::OBJECT_END) {
    throw ParseException("Unexpected token: " + lexer_->value);
  }

  ++lexer_;

  return value;
}

}  // namespace syntax

}  // namespace json
