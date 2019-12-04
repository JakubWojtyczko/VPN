#ifdef _WIN32
// Windows only !

#ifndef VPN_TAP_WIN
#define VPN_TAP_WIN

#ifdef VPN_TAP_LIN
  #warning "Tap for Linux has been detected!"
#endif // VPN_TAP_LIN


#include "TapAbs.h"

// declare struct from <iphlpapi.h>
#include <windows.h>
#include <iphlpapi.h>

namespace vpn {
    

class Tap : public TapAbs {

public:

    Tap() : ip_forward_table(nullptr) {}
    virtual ~Tap();

    bool store_route_table() override;
    bool clear_route_table() const override;
    bool restore_route_table() override;


    // Add required by VPN rows. 
    virtual bool set_up_route_table(std::vector<Route<IPVersion::V4>> const& rt) {}
    // Enable a TAP interface. 
    virtual bool enable_tap_interface() {}
    // Disable a TAP interface.
    virtual bool disable_tap_interface() {}
    // True if tap interface exists, false otherwise.
    virtual bool tap_interface_exists() const {}

protected:
    // Note: Allocated memory inside !!
    MIB_IPFORWARDTABLE * get_route_table() const;

private:
    MIB_IPFORWARDTABLE * ip_forward_table;
};

} // namespace vpn



#endif // VPN_TAP_WIN
#endif // _WIN32
