#include "Tcp.h"
#include <cstring>
#include <ctime>

#include "Logger.h"


namespace vpn {

using std::to_string;

bool Tcp::check_port(TcpHeader const& tcp_h) const {
    return this -> port == tcp_h.destin_port;
}

std::uint8_t Tcp::calcualte_checksum(std::vector<std::uint8_t> const& data) const {
    unsigned long sum = 0;
    for (std::uint8_t const& segment : data) {
        sum += segment;
    }
    while (sum > 511) { // 511 = 2^9 - 1
        sum = (sum & 511) + (sum >> 8);
    }
    return std::uint8_t(sum);
}

Crc Tcp::crc(TcpHeader const& tcp_h, std::vector<std::uint8_t> const& data) const {
    std::uint8_t seg_sum = this -> calcualte_checksum(data);
    std::uint8_t sum = ~(seg_sum + std::uint8_t(tcp_h.checksum));
    if (sum == 0) {
        return Crc::PASS;
    } else {
        return Crc::FAIL;
    }
}

std::uint32_t Tcp::ack_number(TcpHeader const& tcp_h) const {
    std::uint32_t new_ack_num = tcp_h.seq_num + 1;
}

bool Tcp::urg_bit(TcpHeader const& tcp_h) const {
    return tcp_h.hl_flags & TCP_URG;
}

bool Tcp::ack_bit(TcpHeader const& tcp_h) const {
    return tcp_h.hl_flags & TCP_ACK;
}

bool Tcp::psh_bit(TcpHeader const& tcp_h) const {
    return tcp_h.hl_flags & TCP_PSH;
}

bool Tcp::rst_bit(TcpHeader const& tcp_h) const {
    return tcp_h.hl_flags & TCP_RST;
}

bool Tcp::syn_bit(TcpHeader const& tcp_h) const {
    return tcp_h.hl_flags & TCP_SYN;
}

bool Tcp::fin_bit(TcpHeader const& tcp_h) const {
    return tcp_h.hl_flags & TCP_FIN;
}

unsigned Tcp::header_len(TcpHeader const& tcp_h) const {
    // tcp_h.hl_flags = 0110 1010 1101 1101
    // header length    ^^^^
    std::uint16_t offset = 3 * 4;
    std::uint16_t header_len_mask = 15 << offset; // b'1111' = 2^4 - 1
    return (tcp_h.hl_flags & header_len_mask) >> offset;
} 

int Tcp::recieve_tcp(const std::uint32_t * begin, const std::uint32_t * end) {
    TcpHeader header;
    std::memset(&header, 0, sizeof(header));
    header.source_port = std::uint16_t(*begin >> 8);
    header.destin_port = std::uint16_t(*begin);
    begin++;
    header.seq_num = *begin;
    begin++;
    header.ack_num = *begin;
    begin++;
    header.hl_flags = std::uint16_t(*begin >> 8);
    header.win_size = std::uint16_t(*begin);
    begin++;
    header.checksum = *begin;
    begin++;
    header.urgent_p = *begin;
    begin++;
    std::vector<std::uint8_t> data;
    if (header_len(header) == 20) {
        // there are no options applied
        for ( ; begin != end; begin++) {
            data.push_back(std::uint8_t(*begin >> 24));
            data.push_back(std::uint8_t(*begin >> 16));
            data.push_back(std::uint8_t(*begin >> 8));
            data.push_back(std::uint8_t(*begin));
        }
    } else {
        // TODO: read options
    }
    if(check_new_packet(header, data) == false) {
        // release that packet
        return 0;
    }
    return 1;
}

bool Tcp::check_new_packet(TcpHeader const& head, std::vector<std::uint8_t> const& data) {
    // check port 
    if (head.destin_port != this -> port) {
        Logger::getInstance().info("TCP: Wrong port. " + to_string(head.destin_port) + " received "
            + " but " + to_string(port) + " was expected");
        return false;
    }
    // check control sum
    if (crc(head, data) == Crc::FAIL) {
        Logger::getInstance().info("TCP: Incorrect Checksum: " + to_string(head.checksum));
        return false;
    }
    return true;
}

bool Tcp::required_resopnse(TcpHeader const& head) const {
    if (ack_bit(head) && !syn_bit(head)) {
        return false;
    }
    return true;
}

bool Tcp::prepare_header(std::uint32_t len, std::uint32_t last_seq_num,
    std::uint32_t last_ack_num) {
        
}

} // namespace vpn