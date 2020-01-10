#include "Capture.h"
#include "Logger.h"
#include "Utils.h"

#include <string>

#include <pcap.h>

namespace vpn {

Capture::~Capture() {
    if (handle != nullptr) {
        this -> close(this -> handle);
    }
}

bool Capture::set_up() {
    if (this -> dev == "") {
        char * found;
        if((found = this -> find_default_device()) == nullptr) {
            return false;
        }
        this -> dev = std::string(found);
    }
    // determinate address and address mask
    if (this -> obtain_addr_and_mask() == false) {
        return false;
    }
    // try to open a live device
    if ((handle = open_live(dev.c_str())) == nullptr) {
        return false;
    }
    // only ethernet frames are supported
    //if (check_data_link(handle, DLT_EN10MB) == false) {
    //    return false;
    //}
    // only IPv4 packets are considered
    if (set_filter("ip") == false) {
        return false;
    }
    Logger::getInstance().info(dev + " has been set up");
    return true;
}



Buffer<std::uint8_t> Capture::receive() {
    Buffer<std::uint8_t> buffer;
    const std::uint8_t * packet = pcap_next(handle, &header);
    if (packet == nullptr) {
        Logger::getInstance().error("error while reading packet");
        // return an empty buffer
        return buffer;
    }
    buffer.insert(packet, header.len);
    return buffer;
}


void Capture::send(Buffer<std::uint8_t> const& buffer) {
    if (pcap_inject(handle, buffer.data(), buffer.size()) < 0) {
        Logger::getInstance().error("Capture send failed");
    }
}


char * Capture::find_default_device() const {
    char error_buffer[PCAP_ERRBUF_SIZE];
    char * device;
#ifdef _WIN32
    pcap_if_t * devs = nullptr;
    if (pcap_findalldevs(&devs, error_buffer) == 0) {
        // find first connected wireless type interface
        for (pcap_if_t * i = devs; i; i = i -> next) {
            // check connection
            if (i -> flags & PCAP_IF_CONNECTION_STATUS_CONNECTED ) {
                // check type
                if (i -> flags & PCAP_IF_WIRELESS) {
                    device = new char [strlen(i -> name) + 1];
                    sprintf(device, "%s", i -> name);
                    break;
                }
            }
        }
        pcap_freealldevs(devs);
    }
#else // linux
    device = pcap_lookupdev(error_buffer);
#endif // _WIN32
    if (device == nullptr) {
        Logger::getInstance().error(std::string("cannot find default device: ") 
            + std::string(error_buffer));
        return nullptr;
    }
    Logger::getInstance().info(std::string("found default device: ") + device);
    return device;
}


bool Capture::obtain_addr_and_mask() {
    if (dev == "") {
        Logger::getInstance().info("obtain_addr_and_mask: an empty device");
        return false;
    }
    char error_buffer[PCAP_ERRBUF_SIZE];
    if (pcap_lookupnet(dev.c_str(), &net, &mask, error_buffer) == -1) {
        Logger::getInstance().error(std::string("obtain_addr_and_mask: ") 
            + std::string(error_buffer));
        return false;
    }
    return true;
}


pcap_t * Capture::open_live(const char * device) const {
    char error_buffer[PCAP_ERRBUF_SIZE];
    pcap_t * handle = pcap_open_live(device, BUFSIZ, 1, 1000, error_buffer);
    if (handle == nullptr) {
        Logger::getInstance().error(std::string("cannot open device: ") + device + ": " 
            + error_buffer);
        return nullptr;
    }
    Logger::getInstance().info("Opened " + std::string(device));
    return handle;
}

void Capture::close(pcap_t * handle) const {
    pcap_close(handle);
}


bool Capture::check_data_link(pcap_t * handle, int expected) const {
    if (pcap_datalink(handle) != expected) {
        Logger::getInstance().error(std::string("data link != ") + str(expected));
        return false;
    }
    return true;
}


bool Capture::set_filter(const char * filter) {
    if (pcap_compile(handle, &fp, filter, 0, net) == -1) {
        Logger::getInstance().error(std::string("Cannot compile filter"));
        return false;
    }
    if (pcap_setfilter(handle, &fp) == -1) {
        Logger::getInstance().error(std::string("Cannot set filter"));
        return false;
    }
    return true;
}


} // namespace vpn
