#if defined(__linux__) || defined(__unix__) || defined(_POSIX_VERSION)
// Lunux only !

#ifndef VPN_TAP_LIN
#define VPN_TAP_LIN

#ifdef VPN_TAP_WIN
  #warning "Tap for Windows has been detected!"
#endif // VPN_TAP_WIN

#include "TapAbs.h"

#include <string>
#include <cinttypes>
#include <vector>

namespace vpn {

struct _linux_ip_forward_tab {
    std::string name;
    std::uint32_t dest;
    std::uint32_t gate;
    std::uint16_t flags;
    std::uint16_t refcnt;
    std::uint16_t use;
    std::uint16_t metric;
    std::uint32_t mask;
    std::uint16_t mtu;
    std::uint16_t window;
    std::uint16_t irtt;
};

class Tap : TapAbs {

public:
    Tap() : path("/proc/net/route") {}
    virtual ~Tap();
    bool store_route_table() override;
    bool clear_route_table() const override;
    bool restore_route_table() override;
    bool set_up_route_table() override;
    
    bool enable_tap_interface() override;
    bool disable_tap_interface() override;
    bool tap_interface_exists() const override;

protected:

    std::vector <_linux_ip_forward_tab> get_raoute_table() const;

private:
    std::vector <_linux_ip_forward_tab> route_tab;
    std::string path;

};


} // namespace vpn


#endif // VPN_TAP_LIN
#endif // Lunux 
