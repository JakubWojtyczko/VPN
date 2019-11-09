#ifndef VPN_SERVER_ISAKMP
#define VPN_SERVER_ISAKMP

#include "../Usr.h"
#include "../Socket.h"
#include "../Threads.h"


#include <vector>

namespace vpn
{

#ifndef SERVER_IP
#define SERVER_IP "127.0.12.1" // TODO can't be hardcoded !!!
#endif


class ServerIsakmp {

public:
    ServerIsakmp(Usr & self, std::vector<vpn::Usr> & cli) :
        self(self),
        clients(cli),
        server_sock(SERVER_IP, 500, SockTransport::UDP),
        is_active(true)
        {}
    std::thread start();
    void shut_down();
    void close_server();
protected:

    bool prepare_connection_for_isakmp();
    bool listen_and_handle();

private:
    Usr & self;
    std::vector<vpn::Usr> & clients;
    vpn::Socket server_sock;
    bool is_active;
};


} // namespace vpn


#endif // VPN_SERVER_ISAKMP