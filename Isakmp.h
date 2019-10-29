#ifndef VPN_ISAKMP
#define VPN_ISAKMP

#include <cinttypes>

struct IsakmpHeader {

    std::uint32_t init_spi;
    std::uint32_t resp_spi;

    std::uint8_t next_payload;
    std::uint8_t version;
    std::uint8_t exchange_type;
    std::uint8_t flags;
    
    std::uint32_t message_id;
    
    /* payload here */
};


#endif // VPN_ISAKMP