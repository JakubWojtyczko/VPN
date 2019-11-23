#ifndef VPN_TUNNEL
#define VPN_TUNNEL

#include <cinttypes>
#include <queue>

namespace vpn {

template <class T> class Buffer;
class Auth;
class Crypto;


class Tunnel {
public:

    Tunnel() {}
    virtual ~Tunnel() {}

protected:

    Buffer<std::uint8_t> set_up_tunnel(Buffer<std::uint8_t> const& data);
    Buffer<std::uint8_t> tear_down_tunnel(Buffer<std::uint8_t> tunnel);

private:
    
    Auth auth;
    Crypto crypto;

};

} // namespace vpn


#endif // VPN_TUNNEL
