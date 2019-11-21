#ifndef VPN_ESP_AUTH_DATA
#define VPN_ESP_AUTH_DATA

#include <vector>
#include <cinttypes>

namespace vpn {


#pragma pack(push, 1)
struct EspAuthData {
    std::uint8_t data[24];
};
#pragma pack(pop)


} // namespace vpn

#endif // VPN_ESP_AUTH_DATA
