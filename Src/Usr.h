#ifndef VPN_USR
#define VPN_USR

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

private:
    UserConnectionState state;
    
};

} // namespace vpn


#endif // VPN_USR
