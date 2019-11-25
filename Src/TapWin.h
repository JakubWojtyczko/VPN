#ifdef _WIN32
// Windows only !

#ifndef VPN_TAP_WIN
#define VPN_TAP_WIN

#include "TapAbs.h"

// declare struct from <iphlpapi.h>
struct _MIB_IPFORWARDTABLE;

namespace vpn {
    

class Tap : public TapAbs {

public:

    Tap() : ip_forward_table(nullptr) {}
    virtual ~Tap();

    bool store_route_table() override;
    bool clear_route_table() const override;
    bool restore_route_table() override;

protected:
    // Note: Allocated memory inside !!
    _MIB_IPFORWARDTABLE * get_route_table() const;

private:
    _MIB_IPFORWARDTABLE * ip_forward_table;
};

} // namespace vpn



#endif // VPN_TAP_WIN
#endif // _WIN32
