#ifndef VPN_L2_WORKER
#define VPN_L2_WORKER

#include <cinttypes>

#include <pcpp/DpdkDevice.h>
#include <pcpp/DpdkDeviceList.h>

#include "Queue.h"


namespace vpn {
    
class L2Worker : public pcpp::DpdkWorkerThread {

public:
    L2Worker(pcpp::DpdkDevice * tx_device, pcpp::DpdkDevice * rx_device, Queue & tx, Queue & rx) :
        tx_device(tx_device), rx_device(rx_device), is_active(true), tx(tx), rx(rx) {}

    virtual ~L2Worker() {}

    bool run(std::uint32_t core_id);

    void stop() final;

    std::uint32_t getCoreId();
    
private:
    pcpp::DpdkDevice * tx_device;
    pcpp::DpdkDevice * rx_device;
    bool is_active;
    Queue & tx;
    Queue & rx;

    std::uint32_t core_id;

protected:
    
    void insert_into_queue(pcpp::MBufRawPacket ** packets, std::uint16_t n);
    int get_from_queue(pcpp::MBufRawPacket ** packets, std::uint16_t buff_size);    
};


} // namespace vpn



#endif // VPN_L2_WORKER
