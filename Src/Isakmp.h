#ifndef VPN_ISAKMP
#define VPN_ISAKMP


#include <string>
#include <vector>
#include <cstdlib>

namespace vpn {

struct IsakmpHeader;
struct IsakmpDeleteReq;
struct Message1_2;
template <class T> class Buffer;


enum IsakmpStatus {
    SUCCESS,
    INVALID_PAYLOAD_TYPE,
    DOI_NOT_SUPPORTED,
    SITUATION_NOT_SUPPOETED,
    INVALID_MAJ_VER = 5,
    INVALID_MIN_VER,
    INVALID_EXCHANGE_TYPE,
    INVALID_FLAGS,
    INVALID_MESSAGE_ID,
    INVALID_PROTOCOL_ID,
    INVALID_SPI,
    INVALID_TRANSFORM_ID,
    ATTRIBUTES_NOT_SUPPORTED,
    CONNECTED = 16384
};

const std::uint32_t CertificateMaxDuration {0x4F1A00}; // 24H (in seconds)

// Security association:
// source                      destination
//   --------------- msg 1 --------->
//      source public key, negotiation
//   <-------------- msg 2 ----------
//      destin public key, negotiation
//   --------------- msg 3 ---------> (encrypted content)
//      first data, controll
class Isakmp {

public:
    // for responder ip - its own ip, for initiator ip=0
    Isakmp(std::string const& ip_address, std::uint64_t spi=0) : ip(ip_address), source_spi(spi), mess_id(0) {}
    
    void prepare_security_context(Message1_2 & msg) const;

    IsakmpHeader prepare_header_for_message1() const;
    Message1_2 prepare_message_1() const;
    IsakmpStatus verify_message_1(Message1_2 const& msg1);

    IsakmpHeader prepare_header_for_message2();
    Message1_2 prepare_message_2();
    IsakmpStatus verify_message_2(Message1_2 const& msg2);

    IsakmpHeader prepare_header_for_message3() const;
    Buffer<std::uint8_t> prepare_message_3() const;
    bool verify_message_3(IsakmpHeader const& head, std::vector<std::uint8_t> const& content) const;

    // delete
    IsakmpHeader prepare_header_for_delete_req() const;
    IsakmpDeleteReq prepare_delete_req() const;
    IsakmpStatus verify_delete_request();

    std::uint64_t get_spi(bool dest=false) const;
    std::uint64_t get_next_spi(bool set=false);
    void set_spi(std::uint64_t spi);
    const static int PORT;

protected:
    void prepare_key(std::uint8_t key[128]) const;
    void receive_piblic_key(std::uint8_t const data[128]);
private:
    std::uint8_t our_public_key_hex[128];
    std::uint8_t rec_public_key_hex[128];

    std::string our_private_key_hex;
    // our secret value for decoding/encoding messages over IPSec
    std::string secret_hex;
    // Responder IPv4 address
    std::string ip;

    std::uint64_t source_spi;
    std::uint64_t dest_spi;
    std::uint32_t mess_id;

    std::uint32_t certificate_expiration;

    static std::uint64_t next_spi;
};


} // namespace vpn


#endif // VPN_ISAKMP