#include <boost/log/trivial.hpp>
#include <iostream>

#include "json/value.h"
#include "utils/json_utils.h"
#include "utils/logger.h"

int main() {
  json::utils::init_logging(boost::log::trivial::info);
  std::string test_json = R"(
            {
                "name": "test",
                "numbers": [1, 2, 3],
                "nested": {
                    "key": "value"
                }
            }
        )";

  json::Value json = json::parse(test_json);
  std::cout << json.to_string() << std::endl;
  json.put("string", "hello world");
  json.put("exponent", 1e2);
  json.put("null", nullptr);
  json.put("boolean", true);
  json::Value arr;
  arr.add("one");
  arr.add(2);
  json["testing"] = "asdf";
  json["s"] = arr;
  std::cout << "after put " << std::endl;
  std::cout << json.to_string() << std::endl;
  std::cout << json["testing"] << std::endl;

  json::Value x = std::move(json);
  std::cout << "moved" << std::endl;
  std::cout << x.to_string() << std::endl;
  std::cout << x["testing"] << std::endl;

  std::cout << "original" << std::endl;
  std::cout << json << std::endl;
  std::cout << json["testing"] << std::endl;

  return 0;
}
