#ifndef VPN_ESP_HEADER
#define VPN_ESP_HEADER

#include <vector>
#include <cinttypes>

namespace vpn {


#pragma pack(push, 1)
struct EspHeader {
    std::uint32_t spi; // Secutiry Parameter Index
    std::uint32_t sqn; // Sequence Number
};
#pragma pack(pop)


} // namespace vpn

#endif // VPN_ESP_HEADER
