#ifndef VPN_SERVER_ISAKMP
#define VPN_SERVER_ISAKMP

#include "Usr.h"
#include "Socket.h"
#include "Threads.h"
#include "Isakmp.h"
#include "Config.h"


#include <vector>

namespace vpn
{

class ServerIsakmp {

public:
    ServerIsakmp(Usr & self, std::vector<vpn::Usr> & cli) :
        self(self),
        clients(cli),
        server_sock(Config::get_instance()["server_ip"], Isakmp::PORT, SockTransport::UDP),
        is_active(true)
        {}
    std::thread start();
    void shut_down();
    void close_server();


protected:

    bool prepare_connection_for_isakmp();
    bool listen_and_handle();
    void handle(std::string const& ip, char * buf, int len);

    void handle_new(std::string const& ip, char * buf, int len);
    void handle_exist(std::string const& ip, char * buf, int len);

    int is_client_added(std::string const& ip) const;

private:
    Usr & self;
    std::vector<vpn::Usr> & clients;
    Socket server_sock;
    bool is_active;
};


} // namespace vpn


#endif // VPN_SERVER_ISAKMP