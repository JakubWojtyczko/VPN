#ifndef VPN_TAP_ABS
#define VPN_TAP_ABS

namespace vpn {
    
class TapAbs {

public: 

    virtual ~TapAbs() {}

    virtual bool store_route_table() = 0;
    virtual bool clear_route_table() const = 0;
    virtual bool restore_route_table() = 0;
    virtual bool set_up_route_table() = 0;
    
    virtual bool enable_tap_interface() = 0;
    virtual bool disable_tap_interface() = 0;
    virtual bool tap_interface_exists() const=0;

private:



};


} // namespace vpn


#endif // VPN_TAP_ABS
