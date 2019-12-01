#ifndef VPN_L2_APP
#define VPN_L2_APP


#include <pcpp/DpdkDeviceList.h>

#include <vector>

#define MBUF_POOL_SIZE 16*1024-1
#define DEVICE_ID_1 0
#define DEVICE_ID_2 1

namespace vpn {
    

class L2App {

public:

    L2App() : tap(nullptr), phy(nullptr), workers_core_mask(0) {}
    virtual ~L2App() {}
    bool setup();
    bool start_capture_asyns();

protected:

    void init() const;
    bool find_dpdk_devices();
    bool open_dpdk_devices();
    void create_workers();
    void create_core_mask();

private:

    pcpp::DpdkDevice * tap;
    pcpp::DpdkDevice * phy;
    // workers[0] = tap device -> phy device
    // workers[1] = phy device -> tap device
    std::vector <pcpp::DpdkWorkerThread *> workers;
    int workers_core_mask;
};



} // namespace vpn


#endif // VPN_L2_APP
