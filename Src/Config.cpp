#include "Config.h"
#include "Defines.h"
#include "Logger.h"
#include "Utils.h"

#include <fstream>
#include <sstream>

namespace vpn {
    
Config Config::instance;

void Config::read_config_file() {

    std::ifstream file;
    file.open(VPN_CONFIG_FILE_PATH);
    if (!file) {
        exit_with_error("FATAL ERROR: Config file is missing!");
    }
    try {
        std::string line;
        while (std::getline(file, line)) {
            if (line.size() == 0 || line[0] == '#') {
                // skip line starting with '#'
                continue;
            }
            std::istringstream iss(line);
            std::string key, value;
            iss >> key >> value;
            instance.data[key] = value;
        }
    } catch (...) {
        exit_with_error("FATAL ERROR: Config file is invalid!");
    }
    Logger::getInstance().info("Config: Read " + str(instance.data.size()) + " enteries:");
    for (auto const& pair: instance.data) {
        Logger::getInstance().info(pair.first + ": " + pair.second);
    }
}


Config & Config::get_instance() {
    return instance;
}


std::string Config::operator[](std::string const& key) const {
    for (auto const& pair : data) {
        if (key == pair.first) {
            return pair.second;
        }
    }
    Logger::getInstance().error("Config::operator[] - key \'" + key +  "\' not present");
    return "";
}


std::vector <std::string> Config::starts_with(std::string const& prefix) const {
    std::vector <std::string> keys;
    for (auto const& pair : data) {
        if (pair.first.find(prefix) == 0) {
            keys.push_back(pair.second);
        }
    }
    return keys;
}

} // namespace vpn
