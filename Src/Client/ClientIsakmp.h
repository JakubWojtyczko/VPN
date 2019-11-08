#ifndef VPN_CLIENT_ISAKMP
#define VPN_CLIENT_ISAKMP

#include "../Usr.h"
#include "../Socket.h"
#include "../Isakmp.h"


#include <string>


namespace vpn {



class ClientIsakmp {
// This class is responible for client establishment
// and key management. After that it's working in the
// thread and waiting for server commands.
public:
    ClientIsakmp(Usr & user) : 
        user(user), 
        cli_sock("127.0.0.1", 500, SockTransport::UDP),
        isakmp(0)  {}
    virtual ~ClientIsakmp() {}

    bool connect_to_server();

protected:

    bool prepare_connection_for_isakmp();

private:
    Usr & user;
    Socket cli_sock;
    Isakmp isakmp;
};

} // namespace vpn



#endif // VPN_CLIENT_ISAKMP
