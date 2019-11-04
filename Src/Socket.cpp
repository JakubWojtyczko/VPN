#include "Socket.h"



namespace vpn {


int Socket::sock_init() const {
#ifdef _WIN32
    WSADATA wsa_data;
    return WSAStartup(MAKEWORD(1,1), &wsa_data);
#else
    return 0;
#endif // _WIN32
}


int Socket::sock_quit() const {
#ifdef _WIN32
    return WSACleanup();
#else
    return 0;
#endif // _WIN32
}

} // namespace vpn
