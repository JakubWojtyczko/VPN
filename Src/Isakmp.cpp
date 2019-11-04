#include "Isakmp.h"
#include "Logger.h"

#include <cstdlib>

namespace vpn {


void Isakmp::prepare_security_context(Message1_2 & msg) const {
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
    msg.id.reserved = 0;
    msg.id.length = sizeof(msg.id);
    msg.id.id_type = 1; // IPV4_ADDR;
    msg.id.protocol = 17; // UDP
    msg.id.port = 0; // Unused
    msg.id.data = this -> ip; // ip
}


IsakmpHeader Isakmp::prepare_header_for_message1() const {
    IsakmpHeader head;
    // initiator spi is a random number
    head.init_spi = this -> source_spi;
    // responder spi must be 0 for msg1
    head.resp_spi = 0;
    // first payload - security association (1)
    head.next_payload = 1;
    // verision 1.0 (0001 0000)
    head.version = 0x10;
    // exchange type - Aggresive mode (4)
    head.exchange_type = 4;
    // no flags for msg1
    head.flags = 0;
    // Message ID = 0
    head.message_id = 0;
    head.length = sizeof(Message1_2);
    return head;
}


Message1_2 Isakmp::prepare_message_1() const {
    Message1_2 msg;
    msg.head = prepare_header_for_message1();
    prepare_security_context(msg);
    return msg;
}

IsakmpStatus Isakmp::verify_message_1(Message1_2 const& msg) {
    // verify header
    if (msg.head.init_spi) {
        source_spi = msg.head.init_spi;
    } else {
        Logger::getInstance().error("TSAKMP: init spi must be != 0 for message 1");
        return INVALID_SPI;
    }
    if (msg.head.resp_spi) {
        Logger::getInstance().error("TSAKMP: resp spi must be = 0 for message 1");
        return INVALID_SPI;
    }
    if (msg.head.version & 15) {
        Logger::getInstance().error("TSAKMP: invalid min ver in message 1");
        return INVALID_MIN_VER;
    }
    if (msg.head.version != 16) {
        Logger::getInstance().error("TSAKMP: invalid min ver in message 1");
        return INVALID_MAJ_VER;
    }
    if (msg.head.exchange_type != 4) {
        Logger::getInstance().error("TSAKMP: invalid exchange type in message 1");
        return INVALID_EXCHANGE_TYPE;
    }
    // store for future use
    this -> mess_id = msg.head.message_id;

    // verify security association
    if (msg.sec_assoc.domian_of_interpretation != 1) {
        Logger::getInstance().error("TSAKMP: msg1 - DOI not supported. IPSec only");
        return DOI_NOT_SUPPORTED;
    }
    if (msg.sec_assoc.situation != 1) {
        Logger::getInstance().error("TSAKMP: msg1 - situation should be indetify only");
        return SITUATION_NOT_SUPPOETED;
    }
    // verify proposition
    if (msg.sec_assoc.proposal.proto_id != 1) {
        Logger::getInstance().error("TSAKMP: msg1 - protocol ID should be 1 (ISAKMP)");
        return INVALID_PROTOCOL_ID;
    }
    // verify transform
    if (msg.sec_assoc.proposal.tran.id != 1) {
        Logger::getInstance().error("TSAKMP: msg1 - transfrom ID should be 1 (key_ike)");
        return INVALID_TRANSFORM_ID;
    }
    // verify attributes
    if (msg.sec_assoc.proposal.tran.enc_alg.value != 7) {
        Logger::getInstance().error("TSAKMP: msg1 - encod alg not supported");
        return ATTRIBUTES_NOT_SUPPORTED;
    }
    if (msg.sec_assoc.proposal.tran.key_len.value != 128) {
        Logger::getInstance().error("TSAKMP: msg1 - wrong key length");
        return ATTRIBUTES_NOT_SUPPORTED;
    }
    if (msg.sec_assoc.proposal.tran.hash_alg.value != 2) {
        Logger::getInstance().error("TSAKMP: msg1 - hash alg not supported");
        return ATTRIBUTES_NOT_SUPPORTED;
    }
    if (msg.sec_assoc.proposal.tran.gr_desc.value != 2) {
        Logger::getInstance().error("TSAKMP: msg1 - group desc not supported");
        return ATTRIBUTES_NOT_SUPPORTED;
    };
    if (msg.sec_assoc.proposal.tran.auth_mod.value != 1) {
        Logger::getInstance().error("TSAKMP: msg1 - authorisation mode not supported");
        return ATTRIBUTES_NOT_SUPPORTED;
    }
    if (msg.sec_assoc.proposal.tran.lif_du.value <= CertificateMaxDuration) {
        certificate_expiration = msg.sec_assoc.proposal.tran.lif_du.value;
    } else {
        certificate_expiration = CertificateMaxDuration;
    }

    return IsakmpStatus::SUCCESS;
}

IsakmpHeader Isakmp::prepare_header_for_message2() const {
    IsakmpHeader head;
    // initiator spi is copied from received header
    head.init_spi = this -> dest_spi;
    // responder spi is a random number in msg2
    head.resp_spi = this -> source_spi;
    // first payload - security association (1)
    head.next_payload = 1;
    // verision 1.0 (0001 0000)
    head.version = 0x10;
    // exchange type - Aggresive mode (4)
    head.exchange_type = 4;
    // no flags for msg1
    head.flags = 0;
    // Message ID = 0
    head.message_id = 0;
    head.length = sizeof(Message1_2);
    return head;
}

Message1_2 Isakmp::prepare_message_2() const {
    Message1_2 msg;
    msg.head = prepare_header_for_message2();
    prepare_security_context(msg);
    return msg;
}


IsakmpHeader Isakmp::prepare_header_for_message3() const {
    IsakmpHeader head;
    // initiator spi is copied from received header
    head.init_spi = this -> source_spi;
    // responder spi is copied
    head.resp_spi = this -> dest_spi;
    // first payload - security association (1)
    head.next_payload = 8; // hash 
    // verision 1.0 (0001 0000)
    head.version = 0x10;
    // exchange type - Aggresive mode (4)
    head.exchange_type = 4;
    // no flags for msg1
    head.flags = 0;
    // Message ID = 0
    head.message_id = 0;
    // Important: length field is incomplete
    // We have to add the size of hashed data!
    head.length = sizeof(IsakmpHeader);
    return head;
}

IsakmpStatus Isakmp::verify_message_2(Message1_2 const& msg) {
    // verify header
    if (msg.head.resp_spi) {
        dest_spi = msg.head.resp_spi;
    } else {
        Logger::getInstance().error("TSAKMP: resp spi must be != 0 for message 2");
        return INVALID_SPI;
    }
    if (msg.head.init_spi != source_spi) {
        Logger::getInstance().error("TSAKMP: msg2 - invalid init spi");
        return INVALID_SPI;
    }
    if (msg.head.version & 15) {
        Logger::getInstance().error("TSAKMP: invalid min ver in message 2");
        return INVALID_MIN_VER;
    }
    if (msg.head.version != 16) {
        Logger::getInstance().error("TSAKMP: invalid min ver in message 2");
        return INVALID_MAJ_VER;
    }
    if (msg.head.exchange_type != 4) {
        Logger::getInstance().error("TSAKMP: invalid exchange type in message 2");
        return INVALID_EXCHANGE_TYPE;
    }
    if (this -> mess_id != msg.head.message_id) {
        Logger::getInstance().error("TSAKMP: invalid message id in message 2");
        return INVALID_MESSAGE_ID;
    }

    // verify security association
    if (msg.sec_assoc.domian_of_interpretation != 1) {
        Logger::getInstance().error("TSAKMP: msg2 - DOI not supported. IPSec only");
        return DOI_NOT_SUPPORTED;
    }
    if (msg.sec_assoc.situation != 1) {
        Logger::getInstance().error("TSAKMP: msg2 - situation should be indetify only");
        return SITUATION_NOT_SUPPOETED;
    }
    // verify proposition
    if (msg.sec_assoc.proposal.proto_id != 1) {
        Logger::getInstance().error("TSAKMP: msg2 - protocol ID should be 1 (ISAKMP)");
        return INVALID_PROTOCOL_ID;
    }
    // verify transform
    if (msg.sec_assoc.proposal.tran.id != 1) {
        Logger::getInstance().error("TSAKMP: msg2 - transfrom ID should be 1 (key_ike)");
        return INVALID_TRANSFORM_ID;
    }
    // verify attributes
    if (msg.sec_assoc.proposal.tran.enc_alg.value != 7) {
        Logger::getInstance().error("TSAKMP: msg2 - encod alg not supported");
        return ATTRIBUTES_NOT_SUPPORTED;
    }
    if (msg.sec_assoc.proposal.tran.key_len.value != 128) {
        Logger::getInstance().error("TSAKMP: msg2 - wrong key length");
        return ATTRIBUTES_NOT_SUPPORTED;
    }
    if (msg.sec_assoc.proposal.tran.hash_alg.value != 2) {
        Logger::getInstance().error("TSAKMP: msg2 - hash alg not supported");
        return ATTRIBUTES_NOT_SUPPORTED;
    }
    if (msg.sec_assoc.proposal.tran.gr_desc.value != 2) {
        Logger::getInstance().error("TSAKMP: msg2 - group desc not supported");
        return ATTRIBUTES_NOT_SUPPORTED;
    };
    if (msg.sec_assoc.proposal.tran.auth_mod.value != 1) {
        Logger::getInstance().error("TSAKMP: msg2 - authorisation mode not supported");
        return ATTRIBUTES_NOT_SUPPORTED;
    }
    certificate_expiration = msg.sec_assoc.proposal.tran.lif_du.value;
    Logger::getInstance().info("TSAKMP: msg2 - life duration: " + std::to_string(certificate_expiration));

    return IsakmpStatus::SUCCESS;
}

void Isakmp::prepare_key(std::uint8_t key[128], std::string const& key_hex) const {

}

} // namespace vpn
