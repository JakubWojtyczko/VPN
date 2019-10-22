#ifndef VPN_IP_TCP_HEADER
#define VPN_IP_TCP_HEADER

#include <cstdint>

namespace vpn {


struct TcpHeader {
    // octet 1
    std::uint16_t source_port;
    std::uint16_t destin_port;
    // octet 2
    std::uint32_t seq_num;
    // octet 3
    std::uint32_t ack_num;
    //octet 4
    // header len : 4, reserved (unused): 6,
    // URG : 1, ACK : 1, PSH: 1, RST: 1, SYN: 1, FIN: 1
    std::uint16_t hl_flags;
    std::uint16_t win_size;
    // octet 5
    std::uint16_t checksum;
    std::uint16_t urgent_p;
    // octet 6 
    std::uint32_t options;
    /* data here*/
};

} // namespace vpn



#endif  // VPN_IP_TCP_HEADER
