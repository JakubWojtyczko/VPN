#ifndef VPN_CAPTURE
#define VPN_CAPTURE

#include "Buffer.h"

#include <pcap.h>

#include <cinttypes>
#include <string>

namespace vpn {


class Capture {

public:
    Capture(const char * dev="") : dev(dev), handle(nullptr) {}
    virtual ~Capture();

    bool set_up();

    Buffer<std::uint8_t> receive();

protected:

    char * find_default_device() const;
    bool obtain_addr_and_mask();
    pcap_t * open_live(const char * device) const;
    void close(pcap_t * handle) const;

    bool check_data_link(pcap_t * handle, int expected) const;

    bool set_filter(const char * filter);

    

private:
    
    std::string dev;     // device name
    pcap_t * handle;     // pcap device
    bpf_u_int32 net;     // address int network notation
    bpf_u_int32 mask;    // mask in ntwork notation
    bpf_program fp;      // filter
    pcap_pkthdr header;  // pcap header
};


} // namespace vpn


#endif // VPN_CAPTURE
