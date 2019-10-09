#ifndef VPN_LOG_LOGGER
#define VPN_LOG_LOGGER

namespace vpn {

enum LoggerMessageType {
    INFO,
    ERRPR
}

class Logger {

private:
    Logger();
    ~Logger();
public:

    // true if logger is active (in debug mode)
    // false otherwise.
    bool() const;

    std::string const& getLastError() const;
    std::string const& getLastInfo() const;

    void error(std::string const& error);
    void info(std::string const& info);

    void reportLog(LoggerMessageType type) const;

    static Logger & getInstance();

private:
    std::string last_info_log;
    std::string last_error_log;

    static Logger logger;

};

} // namespace vpn

#endif // VPN_LOG_LOGGER
