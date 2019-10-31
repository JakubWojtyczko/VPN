#ifndef VPN_ISAKMP
#define VPN_ISAKMP

#include "IsakmpHeader.h"

#include <string>
#include <vector>
#include <cstdlib>

namespace vpn {
// Security association:
// source                      destination
//   ---------------msg1--------->
//      source public key, negotiation
//   <--------------msg2----------
//      destin public key, negotiation
//   ---------------msg3---------> (encrypted content)
//      first data, controll
class Isakmp {

public:
    // for responder ip - its own ip, for initiator ip=0
    Isakmp(std::uint32_t const& ip_address) : ip(ip_address), source_spi(std::rand() / 2 + 100) {}
    
    void prepare_security_context(Message1_2 & msg) const;

    IsakmpHeader prepare_header_for_message1() const;
    Message1_2 prepare_message_1() const;
    bool verify_message_1(Message1_2 const& msg1);

    IsakmpHeader prepare_header_for_message2() const;
    Message1_2 prepare_message_2() const;
    bool verify_message_2(Message1_2 const& msg2) const;

    IsakmpHeader prepare_header_for_message3() const;
    std::vector<std::uint8_t> prepare_content_message_3() const;
    bool verify_message_3(IsakmpHeader const& head, std::vector<std::uint8_t> const& content) const;

protected:
    void prepare_key(std::uint8_t key[128], std::string const& key_hex) const;

private:
    std::string our_public_key_hex;
    std::string rec_public_key_hex;

    std::string our_private_key_hex;
    // our secret value for decoding/encoding messages over IPSec
    std::string secret_hex;
    // Responder IPv4 address
    std::uint32_t ip;

    std::uint32_t source_spi;
    std::uint32_t dest_spi;

};

} // namespace vpn
 

#endif // VPN_ISAKMP