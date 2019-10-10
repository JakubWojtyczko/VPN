#include "../../Log/Logger.h"
#include <iostream>

using vpn::Logger;
using vpn::LoggerMessageType;

int main() {
    Logger & logger = Logger::getInstance();

    if (logger)
        std::cout << "Logger in active\n";
    else
        std::cout << "Logger is inactive\n";

    if (logger)
        logger[LoggerMessageType::INFO] = "No matter";
    std::cout << logger.getLastInfo();
}
