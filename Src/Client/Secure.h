#ifndef VPN_SECURE
#define VPN_SECURE

#include "Crypto.h"
#include "Config.h"
#include "Defines.h"
#include "Tun.h"
#include "Threads.h"
#include "Socket.h"

#include <string>

namespace vpn {



class Secure {

public:
    Secure (Tun & tun, Crypto & crypto) : 
        tun(tun), 
        crypto(crypto),
        is_active(true),
        sock(VPN_ANY_ADDRESS, stoi(Config::get_instance()["sec_port"]), SockTransport::UDP) {}


    virtual ~Secure();
    bool set_up();

    std::thread start_rx();
    std::thread start_tx();

    void rx_loop();
    void tx_loop();

    void stop();

private:
    Tun & tun; // reference to TUN interdace
    Crypto & crypto;
    bool is_active;
    Socket sock;
};

} // namespace vpn

#endif // VPN_SECURE