#include "utils/logger.h"

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/formatter_parser.hpp>
#include <ctime>
#include <iostream>

namespace json {

namespace utils {

void init_logging(boost::log::trivial::severity_level min_severity) {
  using namespace boost::log;

  add_console_log(std::cout,
                  keywords::filter = trivial::severity >= min_severity,
                  keywords::format = "[%Severity%] <%TimeStamp%>:\t%Message%");

  add_file_log(keywords::target = "logs", keywords::file_name = "server.log",
               keywords::target_file_name = "json_lib_%Y-%m-%d_%H-%M-%S.log",
               keywords::max_size = 100 * 1024 * 1024,
               keywords::rotation_size = 10 * 1024 * 1024,
               keywords::time_based_rotation =
                   sinks::file::rotation_at_time_point(0, 0, 0),
               keywords::format = "<%Severity%> [%TimeStamp%]: %Message%",
               keywords::auto_flush = true,
               keywords::open_mode = std::ios_base::app);

  add_common_attributes();
}

}  // namespace utils

}  // namespace json
