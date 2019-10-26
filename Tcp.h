#ifndef VPN_IP_TCP
#define VPN_IP_TCP

#include <cstdint>
#include <vector>
#include <utilities>

#include "TcpHeader.h"

namespace vpn {

const double TcpAckExpentanceTiimer {0.1};  // 100 ms
const int TcpMaxRetransmissionNumber {5};

const std::uint16_t TCP_URG {1 << 5};
const std::uint16_t TCP_ACK {1 << 4};
const std::uint16_t TCP_PSH {1 << 3};
const std::uint16_t TCP_RST {1 << 2};
const std::uint16_t TCP_SYN {1 << 1};
const std::uint16_t TCP_FIN {1 << 0};

const bool CRC_FAIL {false};
const bool CRC_PASS {true};


class Tcp {
    
public:
    Tcp(std::uint16_t d_port) : port(d_port) {}
    virtual ~Tcp() {}
    // check if the port is valid
    bool check_port(TcpHeader const& tcp_h) const;
    // generate check sum
    // Important! In the header must be complement (~)
    std::uint8_t calcualte_checksum(std::vector<std::uint8_t> const& data) const;
    // check control sum
    bool crc(TcpHeader const& tcp_h, std::vector<std::uint8_t> const& data) const;
    // generate new acknowledgment number
    // "tcp_h" - last received TCP header
    // always incremented previous sequence number
    std::uint32_t ack_number(TcpHeader const& tcp_h) const;

    // check if flags are set
    // data is urgent
    bool urg_bit(TcpHeader const& tcp_h) const;
    // indicates if ack number is set
    bool ack_bit(TcpHeader const& tcp_h) const;
    // push the entire buffer immediately to receiver
    bool psh_bit(TcpHeader const& tcp_h) const;
    // reset TCP connection
    bool rst_bit(TcpHeader const& tcp_h) const;
    // synchronize sequence number
    bool syn_bit(TcpHeader const& tcp_h) const;
    // terminate TCP connection
    bool fin_bit(TcpHeader const& tcp_h) const;

    // help in knowing from where the data begins
    unsigned header_len(TcpHeader const& tcp_h) const;
    
    // read TCP data from 'data_begin'
    // return amount of read data
    TcpHeader recieve_tcp(const std::uint32_t * bigin, const std::uint32_t * end, int id);
    // check whether destination port is proper and crc
    bool check_new_packet(TcpHeader const& head, std::vector<std::uint8_t> const& data);
private:
    // our port
    std::uint16_t port;

    // tcp[user_id][tcp_number] with data
    std::vector<std::pair<TcpHeader, double>> tcp_h_received;
    // sent TCP with data
    std::vector<std::pair<TcpHeader, double>> tcp_h_sent;

    std::vector<std::uint8_t> send_data_buffer;
    std::vector<std::uint8_t> rec_data_buffer;

};

} // namespace vpn



#endif  // VPN_IP_TCP
