#ifndef VPN_USR
#define VPN_USR

namespace vpn {

enum UserConnectionState {
    Disconnected,
    Panding, // received msg 1
    Establisment, // waiting for msg3
    Connected
};


class Usr {

public:
    Usr() : state(Disconnected) {}
    virtual ~Usr() {}

    void update_state(UserConnectionState new_state);

private:
    UserConnectionState state;
    
};

} // namespace vpn


#endif // VPN_USR
