#ifndef VPN_SSL
#define VPN_SSL

#include <string>
#include <openssl/dh.h>

namespace vpn  {


class Ssl {

public:
    Ssl(size_t key_len) :priv_key(nullptr), pub_key(nullptr), key_len(key_len), 
        secret(nullptr),  secret_size(0), other_public(nullptr) {}
    virtual ~Ssl();

    bool init();

    bool receive_public_key(std::string const& key);
    std::string get_pub_key_hex();
    
protected:
    bool compute_secret();

private:

    DH * priv_key;
    BIGNUM * pub_key;
    const size_t key_len;
    unsigned char * secret;
    int secret_size;

    // received public key via Key Exchange
    BIGNUM * other_public;
};


} // namespace vpn


#endif 
