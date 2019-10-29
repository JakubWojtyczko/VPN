#ifndef VPN_ESP_HEADER
#define VPN_ESP_HEADER

#include <vector>
#include <cinttypes>

namespace vpn {

class EspHeader {
    std::uint_fast32_t spi; // Secutiry Parameter Index
    std::uint_fast32_t sqn; // Sequence Number
};

} // namespace vpn

#endif // VPN_ESP_HEADER