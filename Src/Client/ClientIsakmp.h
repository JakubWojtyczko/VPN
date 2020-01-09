#ifndef VPN_CLIENT_ISAKMP
#define VPN_CLIENT_ISAKMP

#include "Usr.h"
#include "Config.h"
#include "Socket.h"
#include "Isakmp.h"
#include "Threads.h"

#include <string>


namespace vpn {



class ClientIsakmp {
// This class is responible for client establishment
// and key management. After that it's working in the
// thread and waiting for server commands.
public:
    ClientIsakmp(Usr & user) : 
        user(user), 
        cli_sock("0.0.0.0", Isakmp::PORT, SockTransport::UDP),
        is_active(true)
        {}
    virtual ~ClientIsakmp() {}
    
    std::thread start();
    void close();
    bool connect_to_server();
    bool disconnect();
protected:

    bool prepare_connection_for_isakmp();
    bool handshake();
    void listen();

private:
    Usr & user;
    Socket cli_sock;
    bool is_active;
};

} // namespace vpn



#endif // VPN_CLIENT_ISAKMP
