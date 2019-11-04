#ifndef VPN_TUNNEL_MODE
#define VPN_TUNNEL_MODE

#include "IPv4Header.h"
#include "Authentication.h"
#include "TcpHeader.h"
#include "EspHeader.h"
#include "EspTrailer.h"
#include "EspAuthData.h"


#include <vector>
#include <cinttypes>

namespace vpn
{

class TunnelMode {
public:


private:
    IPv4Header new_header;
    AuthenticationHeader ah_header;
    EspHeader esp_header;
    // encrypted data begin
    IPv4Header ip_header;
    TcpHeader tcp_hreader;
    std::vector <std::uint32_t> tcp_data;
    EspTrailer esp_trailer;
    // encrypted data end
    EspAuthData esp_auth_data;
};

} // namespace vpn


#endif // VPN_TUNNEL_MODE
