#ifndef VPN_USR
#define VPN_USR

#include "Isakmp.h"

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
    Usr(std::string const& ip) : 
        state(UserConnectionState::DISCONNECTED),
        ip(ip),
        isakmp(ip) {}
    virtual ~Usr() {}

    void update_state(UserConnectionState new_state);
    inline std::string const& get_ip() const { return ip; } 
    inline Isakmp & get_isakmp() { return isakmp; }


private:
    UserConnectionState state;
    std::string ip;
    Isakmp isakmp;
};

} // namespace vpn


#endif // VPN_USR
