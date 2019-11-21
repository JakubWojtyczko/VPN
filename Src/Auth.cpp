#include "Auth.h"
#include "Logger.h"
#include "Socket.h"


#include <cstring>
#include <string>


namespace vpn
{
    

void Auth::reset_seq_num() {
    this -> sequence = 0;
}


std::uint32_t Auth::next_seq_num() {
    this -> sequence += 1;
    return this -> sequence;
}


AuthenticationHeader Auth::prepare_header() {
    AuthenticationHeader ah;
    std::memset(&ah, 0, sizeof(ah));
    // next is Encapsuliting Security Payload
    ah.next_header = IPPROTO_ESP;
    // It is measured in 32 bit units, with 2 subtracted for 
    // consistency with how header lengths are normally calculated in IPv6.
    ah.payload_length = sizeof(ah) / sizeof(std::uint32_t) - 2;
    ah.spi = htonl(this -> spi);
    // incremented last sequence number
    ah.seq_num = htonl(next_seq_num());
    // calculate authentication data
    calcualte_ah_icv(ah);
    return ah;
}


bool Auth::calcualte_ah_icv(AuthenticationHeader & ah) const {
    // TODO
    ah.auth_data[1] = 1;
    return 1;
}


 bool Auth::vevify_header(AuthenticationHeader const& ah) const {
  
    if (ah.next_header != IPPROTO_ESP) {
        Logger::getInstance().error("AH: Invalid next header");
        return false;
    }

    if (ah.payload_length != sizeof(ah) / sizeof(std::uint32_t) - 2) {
        Logger::getInstance().error("AH: Invalid length");
        return false;
    }

    if (ah.spi != htonl(this -> oth_spi)) {
        Logger::getInstance().error("AH: Invalid spi: " + std::to_string(ntohl(ah.spi)) + 
            " but " +  std::to_string(this -> oth_spi) + " was expected");
        return false;
    }

    if (htonl(ah.seq_num) != sequence) {
        Logger::getInstance().error("AH: Invalid seq num: " + std::to_string(ntohl(ah.seq_num)) + 
            " but " +  std::to_string(sequence) + " was expected");
        return false;
    }

    if (validate_ah_icv(ah) == false) {
        Logger::getInstance().error("AH: ICV validation failed");
        return false;
    }
    return true;
}


bool Auth::validate_ah_icv(AuthenticationHeader const& ah) const {
    // TODO
    return 1;
}


} // namespace vpn
