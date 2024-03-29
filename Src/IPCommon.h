#ifndef VPN_IP_COMMON
#define VPN_IP_COMMON

#include <string>
#include <cmath>


#include "IPv4Header.h"


namespace vpn {



enum IPVersion {
    V4,
    V6
};


template <IPVersion ipver> 
bool validate_address(std::string const& address) {
    return true;
}


template <IPVersion ipver>
class Route {
    
public:
    Route(std::string const& addr, int prefix=0, int metric=1, std::string const& gw="") 
        : addr(addr), prefix(prefix), metric(metric), gw(gw) {}
    virtual ~Route() {}

    inline std::string const& get_address() const {
        return addr;
    }

    inline int get_prefix_len() const {
        return prefix;
    }

    inline int get_metric() const {
        return metric;
    }

    inline std::string const& get_default_gateway() {
        return gw;
    }

    inline bool validate() const {
        return validate_address<ipver>(this -> addr);
    }

private:

    std::string addr;
    int prefix;
    int metric;
    std::string gw;
};


template <IPVersion ipver>
class DNS {

public:

    DNS(std::string const& addr) : addr(addr) {}
    virtual ~DNS() {}
    inline std::string const& get_address() const {
        return addr;
    }
    inline bool validate() const {
        return validate_address<ipver>(this -> addr);
    }

public:
    std::string addr;
};


template <IPVersion IPVer>
class IPCommon {
    
    static std::uint32_t addr_from_string(std::string const& address) {
       /* std::uint32_t ret = 0;
        std::string dot = ".";
        switch (IPVer) {
            case V4:
                // 10.4.145.1
                size_t pos = 0;
                std::string token;
                while ((pos = s.find(dot)) != std::string::npos) {
                    token = s.substr(0, pos);

                    s.erase(0, pos + delimiter.length());
                }
            case V6:
        }*/
        return 0;
    }

    static std::uint16_t header_checksum(const std::uint16_t * head_begin, const std::uint16_t * head_end) {
        unsigned long sum = 0;
        for ( ; head_begin != head_end; ++head_begin) {
            sum += *head_begin;
        
            while (sum > std::pow(2, 16) - 1) { // 511 = 2^9 - 1
                //sum = (sum & (std::pow(2, 16) - 1)) + (sum >> 16);
            }
        }
        return std::uint16_t(sum);
    }
    

};






} // namespace vpn


#endif // VPN_IP_COMMON
