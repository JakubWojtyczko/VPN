#ifndef VPN_DH
#define VPN_DH

#include "Isakmp.h"

#include <string>
#include <vector>

namespace vpn {
// Security association:
// source                      destination
//   ---------------msg1--------->
//      source public key, negotiation
//   <--------------msg2----------
//      destin public key, negotiation
//   ---------------msg3---------> (encrypted content)
//      first data, control
class DH {

public:

    void prepare_common_fields_for_message_1_2(Message1_2 & msg, std::uint32_t const& ip_addr) const;

    IsakmpHeader prepare_header_for_message1() const;
    Message1_2 prepare_message_1(std::uint32_t const& ip_addr) const;
    bool verify_message_1(Message1_2 const& msg1) const;

    IsakmpHeader prepare_header_for_message2(IsakmpHeader const& header1) const;
    Message1_2 prepare_message_2(IsakmpHeader const& header1, std::uint32_t const& ip_addr) const;
    bool verify_message_2(Message1_2 const& msg2) const;

    IsakmpHeader prepare_header_for_message3(IsakmpHeader const& header2) const;
    std::vector<std::uint8_t> prepare_content_message_3() const;
    bool verify_message_3(IsakmpHeader const& head, std::vector<std::uint8_t> const& content) const;

protected:
    void prepare_key(std::uint8_t key[128], std::string const& key_hex) const;

private:
    std::string our_public_key_hex;
    std::string rec_public_key_hex;

    std::string our_private_key_hex;
    std::string secret_hex;

};

} // namespace vpn
 

#endif // VPN_DH