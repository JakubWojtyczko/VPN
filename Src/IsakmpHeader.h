#ifndef VPN_ISAKMP_HEADER
#define VPN_ISAKMP_HEADER

#include <cinttypes>


namespace vpn {

#pragma pack(push, 1)
struct IsakmpHeader {

    std::uint64_t init_spi;
    std::uint64_t resp_spi;

    std::uint8_t next_payload;
    std::uint8_t version;
    std::uint8_t exchange_type;
    std::uint8_t flags;
    
    std::uint32_t message_id;
    std::uint32_t length;
};
#pragma pack(pop)


#pragma pack(push, 1)
struct Message1_2 {

    // Header
    IsakmpHeader head;

    // payload 1: Security association
    #pragma pack(push, 2)
    struct SecurityAssociation {
        std::uint8_t next_payload;
        std::uint8_t reserved;
        std::uint16_t length;
        std::uint32_t domian_of_interpretation;
        std::uint32_t situation;
        struct Proposal {
            std::uint8_t next_payload;
            std::uint8_t reserved;
            std::uint16_t length;
            std::uint8_t prop_num;
            std::uint8_t proto_id;
            std::uint8_t spi_size;
            std::uint8_t transforms;

            struct Transform {
                std::uint8_t next;
                std::uint8_t reserved;
                std::uint16_t length;
                std::uint8_t num;
                std::uint8_t id;
                std::uint16_t reserved_2; 
                // Encryption algorithm
                struct EA {
                    std::uint16_t format;
                    std::uint16_t value;
                } enc_alg;

                // Key Length
                struct KL {
                    std::uint16_t format;
                    std::uint16_t value;
                } key_len;

                // Hash algorithm
                struct HA {
                    std::uint16_t format;
                    std::uint16_t value;
                } hash_alg;

                // Group description
                struct GD {
                    std::uint16_t format;
                    std::uint16_t value;
                } gr_desc;

                // Authentication Mode
                struct AM {
                    std::uint16_t format;
                    std::uint16_t value;
                } auth_mod;

                // Life type
                struct LT {
                    std::uint16_t format;
                    std::uint16_t value;
                } lif_ty;

                // Life duration
                struct LD {
                    std::uint16_t format;
                    std::uint16_t length;
                    std::uint32_t value;
                } lif_du;
            } tran;
        } proposal;
    } sec_assoc;
    #pragma pack(pop)

    // payload 2: Key Exchange
    struct KeyExchange {
        std::uint8_t next;
        std::uint8_t reserved;
        std::uint16_t length;
        std::uint8_t data[128];
    } key;

    // payload 3: identification
    struct Identification {
        std::uint8_t next;
        std::uint8_t reserved;
        std::uint16_t length;
        std::uint8_t id_type;
        std::uint8_t protocol;
        std::uint16_t port;
        std::uint32_t data; // ip
    } id;
};
#pragma pack(pop)


#pragma pack(push, 1)
struct IsakmpHash {
    std::uint8_t next;
    std::uint8_t reserved;
    std::uint16_t length;
};
#pragma pack(pop)


#pragma pack(push, 1)
struct IsakmpDeleteReq {
    IsakmpHeader head;

    std::uint8_t next;
    std::uint8_t reserved;
    std::uint16_t len;

    std::uint32_t doi; // domian of interpretation

    std::uint8_t proto_id;
    std::uint8_t spi_size;
    std::uint16_t of_spi; // number of spis to delete

    std::uint64_t spi;
};
#pragma pack(pop)


} // namespace vpn

#endif // VPN_ISAKMP_HEADER
