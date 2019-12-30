#ifndef VPN_TUN
#define VPN_TUN

namespace vpn {


class Tun {

public:

    bool alloc(int flags);

private:
    int fd;
};

} // namespace vpn

#endif // VPN_TUN