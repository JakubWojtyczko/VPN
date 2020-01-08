#ifndef VPN_SSL
#define VPN_SSL

#include <string>
#include <openssl/dh.h>

#include "Buffer.h"

namespace vpn  {


class Ssl {

public:
    Ssl() : priv_key(nullptr), pub_key(nullptr), 
        secret(nullptr),  secret_size(0), other_public(nullptr) {}
    virtual ~Ssl();

    // Important! Must be called after object construction.
    bool init();

    bool receive_public_key(std::string const& key);
    Buffer <std::uint8_t> get_pub_key_hex();

    Buffer <std::uint8_t> encode_text(Buffer <std::uint8_t> const& buffer) const;
    Buffer <std::uint8_t> decode_text(Buffer <std::uint8_t> const& buffer) const;

    Buffer <std::uint8_t> calculate_hash(Buffer <std::uint8_t> const& buffer) const;

protected:
    bool compute_secret();

private:

    DH * priv_key;
    BIGNUM * pub_key;
    static const size_t key_len = 1024;
    unsigned char * secret;
    int secret_size;

    // received public key via Key Exchange
    BIGNUM * other_public;
};


} // namespace vpn


#endif 
