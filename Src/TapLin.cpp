#if defined(__linux__) || defined(__unix__) || defined(_POSIX_VERSION)
// Lunux only !

#include "TapLin.h"
#include "Utils.h"
#include "Logger.h"

#include <fstream>
#include <sstream>
#include <string>

namespace vpn {
    

Tap::~Tap() {

}

std::vector <_linux_ip_forward_tab> Tap::get_raoute_table() const {
    std::vector <_linux_ip_forward_tab> tab;
    std::ifstream file;
    file.open(this -> path);
    if(!file) {
        Logger::getInstance().error("Tap: cannot open " + path);
        // return an empty table
        return tab;
    }
    std::string line;
    // read header
    std::getline(file, line);
    // read enteries
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        _linux_ip_forward_tab t;
        iss >> t.name;
        iss >> std::hex >> t.dest;
        iss >> std::hex >> t.gate;
        iss >> std::oct >> t.flags;
        iss >> std::dec >> t.refcnt;
        iss >> std::dec >> t.use;
        iss >> std::dec >> t.metric;
        iss >> std::hex >> t.mask;
        iss >> std::dec >> t.mtu;
        iss >> std::dec >> t.window;
        iss >> std::dec >> t.irtt;
        tab.push_back(t);
    }
    Logger::getInstance().info("Tap: Read " + str(tab.size()) + " entries");
    return tab;
}

bool Tap::store_route_table() {
    this -> route_tab = this -> get_raoute_table();
    return !this -> route_tab.empty();
}

bool Tap::clear_route_table() const {
    return false;
}
bool Tap::restore_route_table() {
    return false;
}
bool Tap::set_up_route_table() {
    return false;
}
    
bool Tap::enable_tap_interface() {
    return false;
}
bool Tap::disable_tap_interface() {
    return false;
}
bool Tap::tap_interface_exists() const {
    // TODO: move path to config!
    std::ifstream f("/dev/tun1234");
    return f.good();
}

} // namespace vpn


#endif // Linux
