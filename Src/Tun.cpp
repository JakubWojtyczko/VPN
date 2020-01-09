#include "Tun.h"
#include "Config.h"
#include "Logger.h"
#include "SocketInc.h"
#include "Utils.h"

#include <net/if.h>
#include <fcntl.h>
#include <linux/if_tun.h>
#include <sys/ioctl.h>

#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>

namespace vpn {

bool Tun::alloc(std::string const& dev) {
    // https://backreference.org/2010/03/26/tuntap-interface-tutorial/
    struct ifreq ifr;
    int  err;
    const char *clonedev = "/dev/net/tun";

    // open the clone device 
    if( (fd = open(clonedev, O_RDWR)) < 0 ) {
        Logger::getInstance().error(std::string("Cant open ") + clonedev);
        return false;
    }
    // preparation of the struct ifr, of type "struct ifreq"
    memset(&ifr, 0, sizeof(ifr));
    // IFF_TUN or IFF_TAP, plus maybe IFF_NO_PI
    ifr.ifr_flags = IFF_TUN;
    
    if (!dev.empty()) {
        // if a device name was specified, put it in the structure; otherwise,
        // the kernel will try to allocate the "next" device of the
        // specified type 
        strncpy(ifr.ifr_name, dev.c_str(), IFNAMSIZ);
    }
  
    // try to create the device
    if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ) {
        Logger::getInstance().error("Can\'t create tun device.");
        close(fd);
        return false;
    }
    this -> name = dev;
    return true;
}


bool Tun::set_up() {
    this -> ip = Config::get_instance()["tun_ip"];
    this -> submask = Config::get_instance()["tun_sub_mask"];
    // call system command to set new ip
    std::string command = "ip addr add " + ip + submask + " dev " + name;
    if (std::system(command.c_str()) == -1) {
        Logger::getInstance().error("Tun::set_up cannot set ip");
        return false;
    }
    // set it up
    command = "ip link set " + this -> name + " up";
    if (std::system(command.c_str()) == -1) {
        Logger::getInstance().error("Tun::set_up cannot set tun up");
        return false;
    }
    // change default gateway
    command = "ip route add default via " + this -> ip + " dev " + this -> name;
    if (std::system(command.c_str()) == -1) {
        Logger::getInstance().error("Tun::set_up set default gateway");
        return false;
    }

    return true;
}


bool Tun::delete_if() {
    user_message("Reseting networking settings...");
    // disable interface
    if(ioctl(this -> fd, TUNSETPERSIST, 0) < 0){
        Logger::getInstance().error("Error while deleting tun iff");
        return false;
    }
    // restart networking 
    system("service networking restart");
    return true;
}

} // namespace vpn
