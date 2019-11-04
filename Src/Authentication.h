#ifndef VPN_AUTENTICATION
#define VPN_AUTENTICATION

#include <cstdint>
#include <vector>

namespace vpn {

struct AuthenticationHeader {
    // octet 1
    std::uint8_t next_header;
    std::uint8_t payload_length;
    std::uint16_t reserved;
    // octet 2
    std::uint32_t spi; // security parameter index
    // octet 3
    std::uint32_t seq_num;
    // authentication data
    std::vector <std::uint32_t> auth_data;
};


} // namespace vpn


#endif // VPN_AUTENTICATION
