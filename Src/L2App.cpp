#if 0
#include "L2App.h"
#include "Logger.h"
#include "L2Worker.h"


#include <pcpp/SystemUtils.h>
#include <pcpp/DpdkDeviceList.h>
#include <pcpp/TablePrinter.h>

namespace vpn {


bool L2App::setup() {
    this -> init();
    if (this -> find_dpdk_devices() == false) {
        return false;
    }
    if (this -> open_dpdk_devices() == false) {
        return false;
    }
    this -> create_workers();
    this -> create_core_mask();
    return true;
}


bool L2App::start_capture_asyns() {
    if (!pcpp::DpdkDeviceList::getInstance().startDpdkWorkerThreads(workers_core_mask, workers)) {
        Logger::getInstance().error("L2App: cant start workers");
        return false;
    }
    return true;
}


void L2App::init() const {
    pcpp::CoreMask core_mask = pcpp::getCoreMaskForAllMachineCores();
    pcpp::DpdkDeviceList::initDpdk(core_mask, MBUF_POOL_SIZE);
}


bool L2App::find_dpdk_devices() {
    tap = pcpp::DpdkDeviceList::getInstance().getDeviceByPort(DEVICE_ID_1);
    if (tap == nullptr)
    {
        Logger::getInstance().error("L2App: Cannot find tap device");
        return false;
    }

    phy = pcpp::DpdkDeviceList::getInstance().getDeviceByPort(DEVICE_ID_2);
    if (phy == nullptr)
    {
        Logger::getInstance().error("L2App: Cannot find physical device");
        return false;
    }
    return true;
}


bool L2App::open_dpdk_devices() {
    if (!tap -> openMultiQueues(1, 1))
    {
        Logger::getInstance().error("L2App: Couldn't open tap " + std::string(tap -> getPMDName().c_str()));
        return false;
    }

    if (!phy -> openMultiQueues(1, 1))
    {
        Logger::getInstance().error("L2App: Couldn't open phy " + std::string(phy -> getPMDName().c_str()));
        return false;
    }
    return true;
}


void L2App::create_workers() {
    // tap device -> phy device
    workers.push_back(new L2Worker(tap, phy, usr.get_to_phy_queue(), usr.get_from_tap_queue()));
    // phy device -> tap device
    workers.push_back(new L2Worker(phy, tap, usr.get_to_tap_queue(), usr.get_from_phy_queue()));
}


void L2App::create_core_mask() {
    // assign core 1 and core 2 to workers
    for (unsigned i = 0; i< workers.size(); ++i) {
        workers_core_mask |= (1 << (i+1));
    }
}


} // namespace vpn
#endif // 0
