#ifndef VPN_IP_TCP
#define VPN_IP_TCP

#include <cstdint>

namespace vpn {

typedef unsigned __int128 uint128_t;

struct TcpHeader {
    // octet 1
    std::uint16_t source_port;
    std::uint16_t destin_port;
    // octet 2
    std::uint32_t seq_num;
    // octet 3
    std::uint32_t ack_num;
    //octet 4
    std::uint8_t off_rsv;
    std::uint8_t flags; // URG, ACK, PSH, RST, SYN, FIN
    std::uint16_t win_size;
    // octet 5
    std::uint16_t checksum;
    std::uint16_t urgent_p;
    // octet 6 
    std::uint32_t options;
    /* data here*/
};

} // namespace vpn



#endif  // VPN_IP_TCP
