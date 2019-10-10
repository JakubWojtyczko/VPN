#ifndef VPN_IP_IPV4
#define VPN_IP_IPV4

#include <cstdint>

namespace vpn {


struct IPv4Header {
    // octet 1
    std::uint8_t version_ihl;
    std::uint8_t type_of_service;
    std::uint16_t length;
    // octet 2
    std::uint16_t identification;
    std::uint16_t df_dm_fragment_offset;
    // octet 3
    std::uint8_t ttl;
    std::uint8_t protocol;
    std::uint16_t header_checksum;
    // octet 4
    std::uint32_t source_address;
    // octet 5
    std::uint32_t destination_address;
    // octet 6
    std::uint32_t options;
	
    /* data here */
};

} // namespace vpn



#endif  // VPN_IP_IPV4
