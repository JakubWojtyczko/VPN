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
    }

    static std::uint16_t header_checksum(const std::uint16_t * head_begin, const std::uint16_t * head_end) {/*
        unsigned long sum = 0;
        for ( ; head_begin != head_end; ++head_begin) {
            sum += *head_begin;
        
            while (sum > std::pow(2, 16) - 1) { // 511 = 2^9 - 1
                sum = (sum & (std::pow(2, 16) - 1)) + (sum >> 16);
            }
        }
        return std::uint16_t(sum);
    */}
    
};


} // namespace vpn


#endif // VPN_IP_COMMON