#ifndef VPN_SOCKET
#define VPN_SOCKET

#ifdef _WIN32 // Windows
  //#ifndef _WIN32_WINNT
  //    #define _WIN32_WINNT 0x0501 // Windows XP
  //#endif // _WIN32_WINNT
  #include <winsock2.h>
  #include <Ws2tcpip.h>
#else  // Assume POSIX
  
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <netdb.h>
  #include <unistd.h>
#endif // _WIN32

namespace vpn {
    

class Socket {

public:
    // WinSocket requires that
    int sock_init() const;
    // WinSocket requires that
    int sock_quit() const;

private:
    // Socket file descriptor.
    // POSIX systems use int to describe sockets and
    // negative number to indicate invalid one,
    // while Windows (WinSocket) uses unsigned int
    // for that and special INVALID_SOCKET for 
    // invalid sockets.
    int socket_fd;
};

} // namespace vpn


#endif // VPN_SOCKET
