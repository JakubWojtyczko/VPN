#ifndef VPN_ISAKMP_HEADER
#define VPN_ISAKMP_HEADER

#include <cinttypes>

#define pack __attribute__((__packed__))

struct pack IsakmpHeader {

    std::uint64_t init_spi;
    std::uint64_t resp_spi;

    std::uint8_t next_payload;
    std::uint8_t version;
    std::uint8_t exchange_type;
    std::uint8_t flags;
    
    std::uint32_t message_id;
    std::uint32_t length;
};

struct pack Message1_2 {

    // Header
    IsakmpHeader head;

    // payload 1: Security association
    struct pack SecurityAssociation {
        std::uint8_t next_payload;
        std::uint8_t reserved;
        std::uint16_t length;
        std::uint16_t domian_of_interpretation;
        std::uint32_t situation;
        struct pack Proposal {
            std::uint8_t next_payload;
            std::uint8_t reserved;
            std::uint16_t length;
            std::uint8_t prop_num;
            std::uint8_t proto_id;
            std::uint8_t spi_size;
            std::uint8_t transforms;

            struct pack Transform {
                std::uint8_t next;
                std::uint8_t reserved;
                std::uint16_t length;
                std::uint8_t num;
                std::uint8_t id;
                std::uint16_t reserved_2; 
                // Encryption algorithm
                struct pack EA {
                    std::uint16_t format;
                    std::uint16_t value;
                } enc_alg;

                // Key Length
                struct pack KL {
                    std::uint16_t format;
                    std::uint16_t value;
                } key_len;

                // Hash algorithm
                struct pack HA {
                    std::uint16_t format;
                    std::uint16_t value;
                } hash_alg;

                // Group description
                struct pack GD {
                    std::uint16_t format;
                    std::uint16_t value;
                } gr_desc;

                // Authentication Mode
                struct pack AM {
                    std::uint16_t format;
                    std::uint16_t value;
                } auth_mod;

                // Life type
                struct pack LT {
                    std::uint16_t format;
                    std::uint16_t value;
                } lif_ty;

                // Life duration
                struct pack LD {
                    std::uint16_t format;
                    std::uint16_t length;
                    std::uint32_t value;
                } lif_du;
            } tran;
        } proposal;
    } sec_assoc;

    // payload 2: Key Exchange
    struct pack KeyExchange {
        std::uint8_t next;
        std::uint8_t reserved;
        std::uint16_t length;
        std::uint8_t data[128];
    } key;

    // payload 3: identification
    struct pack Identification {
        std::uint8_t next;
        std::uint8_t reserved;
        std::uint16_t length;
        std::uint8_t id_type;
        std::uint8_t protocol;
        std::uint16_t port;
        std::uint32_t data; // ip
    } id;
};

#undef pack

#endif // VPN_ISAKMP_HEADER