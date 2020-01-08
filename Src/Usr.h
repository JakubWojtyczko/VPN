#ifndef VPN_USR
#define VPN_USR

#include "Isakmp.h"
#include "Queue.h"
#include "SSL.h"

#include <string>


namespace vpn {

enum UserConnectionState {
    DISCONNECTED,
    PANDING, // received msg 1
    ESTABLISHMENT, // waiting for msg3
    ESTABLISHED
};


class Usr {

public:
    Usr(std::string const& ip, std::uint64_t spi=0) : 
        state(UserConnectionState::DISCONNECTED),
        ip(ip),
        isakmp(ip, spi)
        {}
    virtual ~Usr() {}

    void update_state(UserConnectionState new_state);
    inline std::string const& get_ip() const { return ip; } 
    Isakmp & get_isakmp();

    Queue & get_from_phy_queue();
    Queue & get_to_phy_queue();
    Queue & get_from_tap_queue();
    Queue & get_to_tap_queue();

    bool prepare_ssl();

private:
    UserConnectionState state;
    std::string ip;
    Isakmp isakmp;
    Queue from_phy, to_tap, from_tap, to_phy;
    Ssl ssl;
};

} // namespace vpn


#endif // VPN_USR
