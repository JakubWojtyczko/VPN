#ifndef VPN_ESP_TRAILER
#define VPN_ESP_TRAILER

#include <vector>
#include <cinttypes>

namespace vpn {

class EspTrailer {
    std::vector <std::uint8_t> padding;
    std::uint8_t pad_length;
    std::uint8_t next_hreder;
};

} // namespace vpn

#endif // VPN_ESP_TRAILER