#include "Logger.h"

#include <iostream>

namespace vpn {

Logger Logger::logger;

Logger::~Logger() {
	
}

Logger::operator bool() const {
#ifdef DEBUG
    return true;
#else
    return false;
#endif // DEBUG
}

std::string const& Logger::getLastError() const {
    return this -> last_error_log;
}

std::string const& Logger::getLastInfo() const  {
    return this -> last_info_log;
}

void Logger::error(std::string const& error) {
    this -> last_error_log = error;
    static_cast<const Logger *>(this) -> reportLog(LoggerMessageType::ERROR);
}

void Logger::info(std::string const& info) {
    this -> last_info_log = info;
    static_cast<const Logger *>(this) -> reportLog(LoggerMessageType::INFO);
}

std::string & Logger::operator[](LoggerMessageType type) {
	if (type == LoggerMessageType::INFO) {
		return this -> last_info_log;
	} else {
		return this -> last_error_log;
	}
}

void Logger::reportLog(LoggerMessageType type) const {
    switch (type) {
        case ERROR:
		std::cout << "ERROR: " << this -> last_error_log << '\n';
        break;
		case INFO:
		std::cout << "INFO: " << this -> last_info_log << '\n';
        break;
    }
}

Logger & Logger::getInstance() {
	return logger;
}

} // namespace vpn