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
#if defined(DEBUG) || defined(W_DEBUG)
    this -> last_error_log = error;
    static_cast<const Logger *>(this) -> reportLog(LoggerMessageType::ERROR_T);
#endif // DEBUG || W_DEBUG
}

void Logger::info(std::string const& info) {
#ifdef W_DEBUG
    this -> last_info_log = info;
    static_cast<const Logger *>(this) -> reportLog(LoggerMessageType::INFO_T);
#endif // W_DEBUG
}

std::string & Logger::operator[](LoggerMessageType type) {
    if (type == LoggerMessageType::INFO_T) {
        return this -> last_info_log;
    } else {
        return this -> last_error_log;
    }
}

void Logger::reportLog(LoggerMessageType type) const {
    switch (type) {
        case ERROR_T:
        std::cout << "ERROR: " << this -> last_error_log << '\n';
        break;
        case INFO_T:
        std::cout << "INFO: " << this -> last_info_log << '\n';
        break;
    }
}

Logger & Logger::getInstance() {
    return logger;
}

} // namespace vpn
