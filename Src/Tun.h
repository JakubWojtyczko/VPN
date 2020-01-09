#ifndef VPN_TUN
#define VPN_TUN

#include <string>

namespace vpn {


class Tun {

public:

    bool alloc(std::string const& dev);

    bool set_up();

    bool delete_if();

private:
    int fd;
    std::string name;
    std::string submask;
    std::string ip;
};

} // namespace vpn

#endif // VPN_TUN