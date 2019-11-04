#ifndef VPN_ESP_AUTH_DATA
#define VPN_ESP_AUTH_DATA

#include <vector>
#include <cinttypes>

namespace vpn {

class EspAuthData {
    std::vector <std::uint8_t> data;
};

} // namespace vpn

#endif // VPN_ESP_AUTH_DATA