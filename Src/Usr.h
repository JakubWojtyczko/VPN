#ifndef VPN_USR
#define VPN_USR

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
    Usr() : state(UserConnectionState::DISCONNECTED) {}
    virtual ~Usr() {}

    void update_state(UserConnectionState new_state);
    inline std::string const& get_ip() const { return ip; } 
private:
    UserConnectionState state;
    std::string ip;
};

} // namespace vpn


#endif // VPN_USR
