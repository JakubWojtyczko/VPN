#ifndef VPN_ESP_AUTH_DATA
#define VPN_ESP_AUTH_DATA

#include <vector>
#include <cinttypes>

#ifndef ESP_AUTH_LEN
#define ESP_AUTH_LEN 32
#endif // ESP_AUTH_LEN

namespace vpn {


#pragma pack(push, 1)
struct EspAuthData {
    std::uint8_t data[ESP_AUTH_LEN];
};
#pragma pack(pop)


} // namespace vpn

#endif // VPN_ESP_AUTH_DATA
