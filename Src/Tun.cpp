#include "Tun.h"
#include "Config.h"
#include "Logger.h"
#include "SocketInc.h"

#include <net/if.h>
#include <fcntl.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>

#include <string>
#include <cstring>

#include <iostream>

namespace vpn {

bool Tun::alloc(int flags) {
    std::string name = Config::get_instance()["tap_name"];
    struct ifreq ifr;
    std::string device = "/dev/net/" + name;
    std::memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = flags;

    if ((this -> fd = open(device.c_str() , O_RDWR)) < 0) {
        Logger::getInstance().error("Cannot open " + device);
        return false;
    }

    strncpy(ifr.ifr_name, name.c_str(), IFNAMSIZ);
    
    if (ioctl(this -> fd, TUNSETIFF, &ifr) < 0) {
        Logger::getInstance().error("Tun::alloc::ioctl(TUNSETIFF)");
        
        std::cout << strerror(errno);
        close(this -> fd);
        return false;
    }
    return true;
}

} // namespace vpn
