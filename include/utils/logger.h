#pragma once

#include <boost/log/trivial.hpp>

namespace json {

namespace utils {

void init_logging(boost::log::trivial::severity_level min_severity);

}  // namespace utils

}  // namespace json
