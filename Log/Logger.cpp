#include "Logger.h"

namespace vpn {

Logger:::Logger() {}

Logger::~Logger() {}

Logger::bool() const {
#ifdef DEBUG
    const bool debug = ture;
#else
    const bool debug = false;
#endif // DEBUG
}

std::string const& Logger::getLastError() const {
    return this.last_error_log;
}

std::string const& Logger::getLastInfo() const  {
    return this.last_info_log;
}

void Logger::error(std::string const& error) {
    this.last_error_log = error;
    this.reportLog();
}

void Logger::info(std::string const& info) {
    this.last_info_log = info;
    this.reportLog();
}

void reportLog(LoggerMessageType type) const {
    switch (type) {
        case ERROR:
        break;
        case INFO:
        break;
    }
}

Logger & Logger::getInstance() {

}

} // namespace vpn