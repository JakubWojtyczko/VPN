#if 0
#include "L2Worker.h"


namespace vpn {
    

bool L2Worker::run(std::uint32_t core_id) {
    this -> core_id = core_id;
    const std::uint16_t BUFFER_SIZE = 64;
    pcpp::MBufRawPacket * buffer[BUFFER_SIZE];

    while (this -> is_active) {

        // read available incoming packets from Rx interface 
        uint16_t num_of_packets = 0;//rx_device -> receivePackets(buffer, BUFFER_SIZE, 0);
        if (num_of_packets > 0) {
            this -> insert_into_queue(buffer, num_of_packets);
        }
        // send available outgoing packtes to Tx interface
        num_of_packets = get_from_queue(buffer, BUFFER_SIZE);
        if (num_of_packets > 0) {
            //tx_device -> sendPackets(buffer, num_of_packets, 0);
        }
    }
    return true;
}

void L2Worker::insert_into_queue(pcpp::MBufRawPacket ** packets, std::uint16_t n) {
    
}


int L2Worker::get_from_queue(pcpp::MBufRawPacket ** packets, std::uint16_t buff_size) {
    return 0;
}


void L2Worker::stop() { 
    this -> is_active = false; 
}

std::uint32_t L2Worker::getCoreId() { 
    return this -> core_id; 
}

} // namespace vpn
#endif // 0