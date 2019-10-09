#ifndef VPN_IP_IPV4
#define VPN_IP_IPV4

#include <cstdint>

namespace vpn
{

struct IPv4Header {
    std::uint8_t addr;
    std::uint8_t control;
    std::uint16_t protocol_id;
    std::uint8_t code;
    std::uint16_t length;
    
    std::uint32_t source_address;
    std::uint32_t destination_address;
};

} // namespace vpn



#endif  // VPN_IP_IPV4