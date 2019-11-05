#ifndef VPN_LOG_LOGGER
#define VPN_LOG_LOGGER

#include <string>

namespace vpn {

enum LoggerMessageType {
    INFO_T,
    ERROR_T
};


class Logger {
// A singleton for logging errors and info messages.
// 
// Note: when DEBUG is defined all messages will
// be printed. Otherwise you need to use special
// method 'reportLog' to print it or use getters
// to receive last info/error message.
//
// Usage:
// 1. #include "Logger.h"
// 2.
// 3. Logger::getInstance().error("An error occurred");
//
private:
// Can't create an instance. Use getInstance()
    Logger() {}

public:
    Logger(Logger const& logger) = delete;
    virtual ~Logger();
    // true if logger is active (in debug mode)
    // false otherwise.
    // Debug mode when DEBUG flag is defined
    operator bool() const;
    // Return last reported error
    std::string const& getLastError() const;
    // Return last reported info
    std::string const& getLastInfo() const;
    // Return reference to info/error string depending
    // on passed message type:
    // logger[LoggerMessageType::ERROR] -> last_error_log
    // logger[LoggerMessageType::INFO]  -> las_info_log
    // Do not print a message
    std::string & operator[](LoggerMessageType type);

    // Report and print an error
    void error(std::string const& error);
    // Report and print an info
    void info(std::string const& info);

    // Print error/info
    void reportLog(LoggerMessageType type) const;

    // Return an instance of logger
    static Logger & getInstance();

private:
    // last info
    std::string last_info_log;
    // last error
    std::string last_error_log;

    // logger instance (singleton)
    static Logger logger;

};

} // namespace vpn

#endif // VPN_LOG_LOGGER
