#include "DH.h"

#include <cstdlib>


namespace vpn {

void DH::prepare_common_fields_for_message_1_2(Message1_2 & msg, std::uint32_t const& ip_addr) const {
    // SecurityAssociation
    msg.sec_assoc.next_payload = 4; // Key Exchange 
    msg.sec_assoc.reserved = 0;
    msg.sec_assoc.length = sizeof(msg.sec_assoc);
    msg.sec_assoc.domian_of_interpretation = 1; // IPSEC
    msg.sec_assoc.situation = 1; // Identify only
        // Proposal
        msg.sec_assoc.proposal.next_payload = 0; // None
        msg.sec_assoc.proposal.reserved = 0;
        msg.sec_assoc.proposal.length = sizeof(msg.sec_assoc.proposal);
        msg.sec_assoc.proposal.prop_num = 1; // one propsal
        msg.sec_assoc.proposal.proto_id = 1; // ISAKMP
        msg.sec_assoc.proposal.spi_size = 0;
        msg.sec_assoc.proposal.transforms = 1; // one transform
            // Transform
            msg.sec_assoc.proposal.tran.next = 0; // None
            msg.sec_assoc.proposal.tran.reserved = 0;
            msg.sec_assoc.proposal.tran.length = sizeof(msg.sec_assoc.proposal.tran);
            msg.sec_assoc.proposal.tran.num = 1;
            msg.sec_assoc.proposal.tran.id = 1; // KEY_IKE
            msg.sec_assoc.proposal.tran.reserved_2 = 0; 
                // Encryption algorithm
                msg.sec_assoc.proposal.tran.enc_alg.format = 0x800E; // (TV - type/value)
                msg.sec_assoc.proposal.tran.enc_alg.value = 7;  // TODO: AeS-CBC
                // Key Length
                msg.sec_assoc.proposal.tran.key_len.format = 0x800E; // TV
                msg.sec_assoc.proposal.tran.key_len.value = 128; // always the same length
                // Hash algorithm
                msg.sec_assoc.proposal.tran.hash_alg.format = 0x800E; // TV 
                msg.sec_assoc.proposal.tran.hash_alg.value = 2; // SHA algorithm
                // Group description
                msg.sec_assoc.proposal.tran.gr_desc.format = 0x800E; // TV
                msg.sec_assoc.proposal.tran.gr_desc.value = 2; // Alternate 1024bit MODP group
                // Authentication Mode
                msg.sec_assoc.proposal.tran.auth_mod.format = 0x800E; 
                msg.sec_assoc.proposal.tran.auth_mod.value = 1; // Pre-shared key
                // Life type
                msg.sec_assoc.proposal.tran.lif_ty.format = 0x800E; // TV 
                msg.sec_assoc.proposal.tran.lif_ty.value = 1; // seconds
                // Life duration
                msg.sec_assoc.proposal.tran.lif_du.format = 0x000C; // type/length/value
                msg.sec_assoc.proposal.tran.lif_du.length = sizeof(msg.sec_assoc.proposal.tran.lif_du.value);
                msg.sec_assoc.proposal.tran.lif_du.value = 0x4F1A00; // 24 h

    // Key Exchange
    msg.key.next = 5; // Identification
    msg.key.reserved = 0;
    msg.key.length = sizeof(msg.key);
    prepare_key(msg.key.data, our_public_key_hex);

    // Identification
    msg.id.next = 0; // None
    msg.id.reserved = 9;
    msg.id.length = sizeof(msg.id);
    msg.id.id_type = 1; // IPV4_ADDR;
    msg.id.protocol = 17; // UDP
    msg.id.port = 0; // Unused
    msg.id.data = ip_addr; // ip
}


IsakmpHeader DH::prepare_header_for_message1() const {
    IsakmpHeader head;
    // initiator spi is a random number
    head.init_spi = std::rand();
    // responder spi must be 0 for msg1
    head.resp_spi = 0u;
    // first payload - security association (1)
    head.next_payload = 1;
    // verision 1.0 (0001 0000)
    head.exchange_type = 1 << 4;
    // exchange type - Aggresive mode (4)
    head.exchange_type = 4;
    // no flags for msg1
    head.flags = 0;
    // Message ID = 0
    head.message_id = 0;
    head.length = sizeof(Message1_2);
    return head;
}


Message1_2 DH::prepare_message_1(std::uint32_t const& ip_addr) const {
    Message1_2 msg;
    msg.head = prepare_header_for_message1();
    prepare_common_fields_for_message_1_2(msg, ip_addr);
}


IsakmpHeader DH::prepare_header_for_message2(IsakmpHeader const& head1) const {
    IsakmpHeader head;
    // initiator spi is copied from received header
    head.init_spi = head1.init_spi;
    // responder spi is a random number in msg2
    head.resp_spi = (std::uint32_t) rand();
    // first payload - security association (1)
    head.next_payload = 1;
    // verision 1.0 (0001 0000)
    head.exchange_type = 1 << 4;
    // exchange type - Aggresive mode (4)
    head.exchange_type = 4;
    // no flags for msg1
    head.flags = 0;
    // Message ID = 0
    head.message_id = 0;
    head.length = sizeof(Message1_2);
    return head;
}

Message1_2 DH::prepare_message_2(IsakmpHeader const& header1, std::uint32_t const& ip_addr) const {
    Message1_2 msg;
    msg.head = prepare_header_for_message2(header1);
    prepare_common_fields_for_message_1_2(msg, ip_addr);
}


void DH::prepare_key(std::uint8_t key[128], std::string const& key_hex) const {

}

} // namespace vpn
