#ifndef VPN_AUTH
#define VPN_AUTH

#include <cinttypes>

#include "Authentication.h"


namespace vpn
{

class Auth {

public:

    Auth() : spi(0), sequence(0), oth_spi(0) {}
    virtual ~Auth();

    void reset_seq_num();
    
    AuthenticationHeader prepare_header();
    bool vevify_header(AuthenticationHeader const& ah) const;

protected:

    std::uint32_t next_seq_num();
    bool calcualte_ah_icv(AuthenticationHeader & ah) const;
    bool validate_ah_icv(AuthenticationHeader const& ah) const;

private:

    std::uint32_t spi;
    std::uint32_t sequence;
    std::uint32_t oth_spi;
};


} // namespace vpn


#endif // VPN_AUTH
