#include <iostream>

#include "warren/json/parse.h"
#include "warren/json/value.h"

using ::warren::json::operator""_json;

namespace json = ::warren::json;

int main() {
  json::Value value = R"json(
  {
    "user": {
      "first_name": "Warren",
      "last_name": "Kim",
      "age": "22"
    },
    "features": [
      "string",
      true,
      4.56
    ]
  }
  )json"_json;

  std::string first = value["user"]["first_name"];
  std::string last = value["user"]["last_name"];
  std::string age = value["user"]["age"];
  std::cout << "Hello, " << first << " " << last << " (age as string = " << age
            << ")\n";

  value["user"]["age"] = std::stoi(age);

  value["user"]["full_name"] = first + " " + last;

  std::cout << "Features:\n";
  const json::array_t& feats = value["features"];
  std::string f0 = feats[0];
  bool f1 = feats[1];
  double f2 = feats[2];

  value["features"].push_back("delta");

  std::cout << "Hello, " << first << " " << last << "\n";
  std::cout << "Age (normalized): " << int32_t(value["user"]["age"]) << "\n";
  std::cout << "Full name: " << std::string(value["user"]["full_name"]) << "\n";

  std::cout << "Features (" << static_cast<int>(feats.size()) << "): " << f0
            << ", " << f1 << ", " << f2 << ", " << std::string(feats[3])
            << "\n";

  return 0;
}
