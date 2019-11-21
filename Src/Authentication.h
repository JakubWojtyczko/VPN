#ifndef VPN_AUTENTICATION
#define VPN_AUTENTICATION

#include <cstdint>
#include <vector>

namespace vpn {


#pragma pack(push, 1)
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
    std::uint8_t auth_data[12];
};
#pragma pack(pop)


} // namespace vpn


#endif // VPN_AUTENTICATION
