#ifndef VPN_CRYPTO
#define VPN_CRYPTO

#include <cinttypes>

#include "Buffer.h"
#include "EspHeader.h"
#include "EspTrailer.h"
#include "EspAuthData.h"
#include "Socket.h"


namespace vpn {


class Crypto {

public:

    Crypto() : spi(0), sequence(0), oth_spi(0) {}
    virtual ~Crypto() {}

    Buffer <std::uint8_t> set_up_tunnel(Buffer <std::uint8_t> const& data);
    Buffer <std::uint8_t> tear_down_tunnel(Buffer <std::uint8_t> tunnel);

    void reset_seq_num();

protected:

    std::uint32_t next_seq_num();
    bool verify_header(EspHeader const& head) const;
    EspHeader prepare_header() const;
    EspTrailer prepare_trailer(int data_len, std::uint8_t next=IPPROTO_IPV4) const;
    EspAuthData prepare_auth() const;
    bool verify_auth(EspAuthData const& auth) const;

    Buffer <std::uint8_t> decode(Buffer <std::uint8_t> const& data);
    Buffer <std::uint8_t> encode(Buffer <std::uint8_t> const& data);

private:
    std::uint32_t spi;
    std::uint32_t sequence;
    std::uint32_t oth_spi;
};



} // namespace vpn

#endif // VPN_CRYPTO