#ifndef VPN_IP_IPV4
#define VPN_IP_IPV4

#include <cstdint>

namespace vpn {


struct IPv4Header {
    // octet 1
    std::uint8_t ver_hl; // version : 4 header length : 4
    std::uint8_t tos;    // type of service : 8
    std::uint16_t tl;    // total length
    // octet 2
    std::uint16_t id;    // identification
    std::uint16_t dmf;   // O : 1, DF : 1, MF : 1, fragment offset : 13
    // octet 3
    std::uint8_t ttl;    // time to live : 8
    std::uint8_t proto;  // protocol : 8
    std::uint16_t hs;    // header checksum : 16
    // octet 4
    std::uint32_t saddr; // source address : 32
    // octet 5
    std::uint32_t daddr; // destination address : 32
    // octet 6
	union options {      // options : 8 | 16 | 32
		std::uint8_t o8;
		std::uint16_t o16;
		std::uint32_t o32;
	};
    /* data here */
};

} // namespace vpn



#endif  // VPN_IP_IPV4
