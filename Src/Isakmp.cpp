#include "Buffer.h"
#include "Isakmp.h"
#include "IsakmpHeader.h"
#include "Logger.h"
#include "Socket.h"
#include "SocketInc.h"

#include <cstdlib>
#include <cstring>


namespace vpn {

// following port is reserved for that protocol.
const int Isakmp::PORT = 500;
std::uint64_t Isakmp::next_spi = 0xABC;


void Isakmp::prepare_security_context(Message1_2 & msg) const {
    // SecurityAssociation
    msg.sec_assoc.next_payload = 4; // Key Exchange 
    msg.sec_assoc.reserved = 0;
    msg.sec_assoc.length = Socket::htns(sizeof(msg.sec_assoc));
    msg.sec_assoc.domian_of_interpretation = Socket::htnl(1); // IPSEC
    msg.sec_assoc.situation = Socket::htnl(1); // Identify only
        // Proposal
        msg.sec_assoc.proposal.next_payload = 0; // None
        msg.sec_assoc.proposal.reserved = 0;
        msg.sec_assoc.proposal.length = Socket::htns(sizeof(msg.sec_assoc.proposal));
        msg.sec_assoc.proposal.prop_num = 1; // one propsal
        msg.sec_assoc.proposal.proto_id = 1; // ISAKMP
        msg.sec_assoc.proposal.spi_size = 0;
        msg.sec_assoc.proposal.transforms = 1; // one transform
            // Transform
            msg.sec_assoc.proposal.tran.next = 0; // None
            msg.sec_assoc.proposal.tran.reserved = 0;
            msg.sec_assoc.proposal.tran.length = Socket::htns(sizeof(msg.sec_assoc.proposal.tran));
            msg.sec_assoc.proposal.tran.num = 1;
            msg.sec_assoc.proposal.tran.id = 1; // KEY_IKE
            msg.sec_assoc.proposal.tran.reserved_2 = 0; 
                // Encryption algorithm
                msg.sec_assoc.proposal.tran.enc_alg.format = Socket::htns(0x8001); // (TV - type/value)
                msg.sec_assoc.proposal.tran.enc_alg.value = Socket::htns(7);  // TODO: AeS-CBC
                // Key Length
                msg.sec_assoc.proposal.tran.key_len.format = Socket::htns(0x800E); // TV
                msg.sec_assoc.proposal.tran.key_len.value = Socket::htns(128); // always the same length
                // Hash algorithm
                msg.sec_assoc.proposal.tran.hash_alg.format = Socket::htns(0x8002); // TV 
                msg.sec_assoc.proposal.tran.hash_alg.value = Socket::htns(2); // SHA algorithm
                // Group description
                msg.sec_assoc.proposal.tran.gr_desc.format = Socket::htns(0x8004); // TV
                msg.sec_assoc.proposal.tran.gr_desc.value = Socket::htns(2); // Alternate 1024bit MODP group
                // Authentication Mode
                msg.sec_assoc.proposal.tran.auth_mod.format = Socket::htns(0x8003); 
                msg.sec_assoc.proposal.tran.auth_mod.value = Socket::htns(1); // Pre-shared key
                // Life type
                msg.sec_assoc.proposal.tran.lif_ty.format = Socket::htns(0x800B); // TV 
                msg.sec_assoc.proposal.tran.lif_ty.value = Socket::htns(1); // seconds
                // Life duration
                msg.sec_assoc.proposal.tran.lif_du.format = Socket::htns(12); // type/length/value
                msg.sec_assoc.proposal.tran.lif_du.length = Socket::htns(sizeof(msg.sec_assoc.proposal.tran.lif_du.value));
                msg.sec_assoc.proposal.tran.lif_du.value = Socket::htnl(86400); // 24 h

    // Key Exchange
    msg.key.next = 5; // Identification
    msg.key.reserved = 0;
    msg.key.length = Socket::htns(sizeof(msg.key));
    prepare_key(msg.key.data);

    // Identification
    msg.id.next = 0; // None
    msg.id.reserved = 0;
    msg.id.length = Socket::htns(sizeof(msg.id));
    msg.id.id_type = 1; // IPV4_ADDR;
    msg.id.protocol = 17; // UDP
    msg.id.port = 0; // Unused
    msg.id.data = Socket::htnl(0); // ip  TODO
}


IsakmpHeader Isakmp::prepare_header_for_message1() const {
    IsakmpHeader head;
    // spi is equal 0
    head.init_spi = 0;
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
    head.message_id = htonl(this -> mess_id);
    head.length = Socket::htnl(sizeof(Message1_2));
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
        Logger::getInstance().error("TSAKMP: init spi must be = 0 for message 1");
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
    this -> mess_id = ntohl(msg.head.message_id);

    // verify security association
    if (ntohl(msg.sec_assoc.domian_of_interpretation) != 1) {
        Logger::getInstance().error("TSAKMP: msg1 - DOI not supported. IPSec only");
        return DOI_NOT_SUPPORTED;
    }
    if (ntohl(msg.sec_assoc.situation) != 1) {
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
    if (ntohs(msg.sec_assoc.proposal.tran.enc_alg.value) != 7) {
        Logger::getInstance().error("TSAKMP: msg1 - encod alg not supported");
        return ATTRIBUTES_NOT_SUPPORTED;
    }
    if (ntohs(msg.sec_assoc.proposal.tran.key_len.value) != 128) {
        Logger::getInstance().error("TSAKMP: msg1 - wrong key length");
        return ATTRIBUTES_NOT_SUPPORTED;
    }
    if (ntohs(msg.sec_assoc.proposal.tran.hash_alg.value) != 2) {
        Logger::getInstance().error("TSAKMP: msg1 - hash alg not supported");
        return ATTRIBUTES_NOT_SUPPORTED;
    }
    if (ntohs(msg.sec_assoc.proposal.tran.gr_desc.value) != 2) {
        Logger::getInstance().error("TSAKMP: msg1 - group desc not supported");
        return ATTRIBUTES_NOT_SUPPORTED;
    };
    if (ntohs(msg.sec_assoc.proposal.tran.auth_mod.value) != 1) {
        Logger::getInstance().error("TSAKMP: msg1 - authorisation mode not supported");
        return ATTRIBUTES_NOT_SUPPORTED;
    }
    if (ntohl(msg.sec_assoc.proposal.tran.lif_du.value) <= CertificateMaxDuration) {
        certificate_expiration = msg.sec_assoc.proposal.tran.lif_du.value;
    } else {
        certificate_expiration = CertificateMaxDuration;
    }
    // get public key
    this -> receive_piblic_key(msg.key.data);

    return IsakmpStatus::SUCCESS;
}

IsakmpHeader Isakmp::prepare_header_for_message2() {
    IsakmpHeader head;
    // generate new SPI for new client and store it
    this -> dest_spi = get_next_spi();
    head.init_spi = this -> dest_spi;
    // spi of the server
    head.resp_spi = this -> source_spi;
    // first payload - security association (1)
    head.next_payload = 1;
    // verision 1.0 (0001 0000)
    head.version = 0x10;
    // exchange type - Aggresive mode (4)
    head.exchange_type = 4;
    // no flags for msg1
    head.flags = htonl(0);
    // Message ID = 0
    head.message_id = htonl(this -> mess_id);
    head.length = htonl(sizeof(Message1_2));
    return head;
}

Message1_2 Isakmp::prepare_message_2() {
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
    head.message_id = htonl(mess_id);
    // Important: length field is incomplete
    // We have to add the size of hashed data!
    head.length = sizeof(IsakmpHeader);
    return head;
}

Buffer<std::uint8_t> Isakmp::prepare_message_3() const {
    // prepare message header
    IsakmpHeader head = prepare_header_for_message3();    
    // prepare hash data 
    //TODO TDO
    std::vector<std::uint8_t> encr_data(120);
    for (auto & c : encr_data) {
        c = std::rand();
    }
    // prepare hash payload header
    IsakmpHash hash;
    hash.next = 0; // next payload - None
    hash.reserved = 0;
    hash.length = htons(sizeof(hash) + encr_data.size());
    head.length = htonl(sizeof(head) + sizeof(hash) + encr_data.size());
    // copy head to buffer
    Buffer <std::uint8_t> msg3(&head, sizeof(head));
    // copy hash payload head to buffer
    msg3.insert(&hash, sizeof(hash));
    // copy data to buffer
    msg3.insert(encr_data);
    return msg3;
}

IsakmpStatus Isakmp::verify_message_2(Message1_2 const& msg) {
    // verify header
    if (msg.head.resp_spi) {
        dest_spi = msg.head.resp_spi;
    } else {
        Logger::getInstance().error("TSAKMP: resp spi must be != 0 for message 2");
        return INVALID_SPI;
    }
    if (msg.head.init_spi) {
        source_spi = msg.head.init_spi;
    } else {
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
    if (this -> mess_id != ntohl(msg.head.message_id)) {
        Logger::getInstance().error("TSAKMP: invalid mess id in msg2: " + std::to_string(ntohl(msg.head.message_id))
             + " but " + std::to_string(mess_id) + " was expected");
        return INVALID_MESSAGE_ID;
    }

    // verify security association
    if (ntohl(msg.sec_assoc.domian_of_interpretation) != 1) {
        Logger::getInstance().error("TSAKMP: msg2 - DOI not supported. IPSec only");
        return DOI_NOT_SUPPORTED;
    }
    if (ntohl(msg.sec_assoc.situation) != 1) {
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
    if (ntohs(msg.sec_assoc.proposal.tran.enc_alg.value) != 7) {
        Logger::getInstance().error("TSAKMP: msg2 - encod alg not supported");
        return ATTRIBUTES_NOT_SUPPORTED;
    }
    if (ntohs(msg.sec_assoc.proposal.tran.key_len.value) != 128) {
        Logger::getInstance().error("TSAKMP: msg2 - wrong key length");
        return ATTRIBUTES_NOT_SUPPORTED;
    }
    if (ntohs(msg.sec_assoc.proposal.tran.hash_alg.value) != 2) {
        Logger::getInstance().error("TSAKMP: msg2 - hash alg not supported");
        return ATTRIBUTES_NOT_SUPPORTED;
    }
    if (ntohs(msg.sec_assoc.proposal.tran.gr_desc.value) != 2) {
        Logger::getInstance().error("TSAKMP: msg2 - group desc not supported");
        return ATTRIBUTES_NOT_SUPPORTED;
    };
    if (ntohs(msg.sec_assoc.proposal.tran.auth_mod.value) != 1) {
        Logger::getInstance().error("TSAKMP: msg2 - authorisation mode not supported");
        return ATTRIBUTES_NOT_SUPPORTED;
    }
    certificate_expiration = ntohl(msg.sec_assoc.proposal.tran.lif_du.value);
    Logger::getInstance().info("TSAKMP: msg2 - life duration: " + std::to_string(certificate_expiration));

    // get server public key
    receive_piblic_key(msg.key.data);

    return IsakmpStatus::SUCCESS;
}

IsakmpHeader Isakmp::prepare_header_for_delete_req() const {
    IsakmpHeader head;
    // SPI
    head.init_spi = this -> source_spi;
    head.resp_spi = this -> dest_spi;
    // first payload - delete (12)
    head.next_payload = 12;
    // verision 1.0 (0001 0000)
    head.version = 0x10;
    // exchange type - Aggresive mode (4)
    head.exchange_type = 4;
    // no flags for delete request
    head.flags = 0;
    // Message ID = 0
    head.message_id = Socket::htnl(0);
    head.length = Socket::htnl(sizeof(IsakmpDeleteReq));
    return head;
}


IsakmpDeleteReq Isakmp::prepare_delete_req() const {
    IsakmpDeleteReq req;
    req.head = prepare_header_for_delete_req();
    // no next payload
    req.next = 0;
    req.reserved = 0;
    // len of payload
    req.len = htons(sizeof(req) - sizeof(req.head));
    // DOI = 0 for ISAKMP
    req.doi = 0;
    // ISAKMP
    req.proto_id = 1;
    // SPI size = 8 octets
    req.spi_size = 8;
    // 1 spi only
    req.of_spi = htons(1);
    // our spi to delete
    req.spi = htonl(this -> source_spi);
    return req;
}


IsakmpStatus Isakmp::verify_delete_request() {
    return IsakmpStatus::SUCCESS;
}

std::uint64_t Isakmp::get_spi(bool dest) const {
    return (dest ? dest_spi : source_spi);
}

void Isakmp::receive_piblic_key(std::uint8_t const data[128]) {
    for (unsigned i=0; i<128; ++i) {
        this -> rec_public_key_hex[i] = data[i];
    }
}


void Isakmp::prepare_key(std::uint8_t key[128]) const {
    for (unsigned i=0; i<128; ++i) {
        key[i] = our_public_key_hex[i];
    }
}

std::uint64_t Isakmp::get_next_spi(bool set) {
    if (set) {
        this -> source_spi = Isakmp::next_spi;
    }
    return Isakmp::next_spi++;
}

} // namespace vpn
