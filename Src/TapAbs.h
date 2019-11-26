#ifndef VPN_TAP_ABS
#define VPN_TAP_ABS

// Important: Tap class is defferent for each os.
// Each class must:
//  1. be called "Tap"
//  2. inherit from this class (TapAbs)
//  3. have overwritten all virtual methods
//  4. save original route table 
//  5. enable and disable a TAP interface
//  6. set new routing suitable for VPN
//  7. bing back the old table



namespace vpn {
    
class TapAbs {

public: 

    virtual ~TapAbs() {}

    // Save ip routing table before overwritting. We
    // have to bing back all enteries after vpn destruction
    // and remove our ones.
    virtual bool store_route_table() = 0;
    // Remove routing set by us.
    virtual bool clear_route_table() const = 0;
    // Insert overwritten and deleted rows.
    virtual bool restore_route_table() = 0;
    // Add required by VPN rows. 
    virtual bool set_up_route_table() = 0;
    
    // Enable a TAP interface. 
    virtual bool enable_tap_interface() = 0;
    // Disable a TAP interface.
    virtual bool disable_tap_interface() = 0;
    // True if tap interface exists, false otherwise.
    virtual bool tap_interface_exists() const=0;

private:

};


} // namespace vpn


#endif // VPN_TAP_ABS
