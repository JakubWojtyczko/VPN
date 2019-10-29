#ifndef VPN_IP_IPV6
#define VPN_IP_IPV6

#include <cstdint>

namespace vpn {

typedef unsigned __int128 uint128_t;

struct IPv6Header {
    // octet 1
    std::uint32_t ctf;   // version : 4, traffic class : 8, flow_label : 20
                         // version = 0110 for IPv6
    // octet 2
    std::uint16_t pl;    // payload length : 16
    std::uint8_t nhdr;   // next header : 6
    std::uint8_t hl;     // hop limit
    // octet 3
    uint128_t saddr;     // source address : 32
    // octet 4
    uint128_t daddr;     // destination address : 32
    // octet 6
    std::uint32_t options;
    /* data here */
};

} // namespace vpn



#endif  // VPN_IP_IPV6
