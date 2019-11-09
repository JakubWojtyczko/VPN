#ifndef VPN_SOCKET
#define VPN_SOCKET

#include <string>
#include <vector>


namespace vpn {

enum SockTransport{
    UDP,
    TCP
};

class Socket {

public:
    Socket(std::string const& ip, int port, SockTransport t) :
        fd(-1), ip_address(ip), port(port), transport(t) {}
    virtual ~Socket() {}
    
    int last_error() const;
    std::string last_error_str() const;

    bool create_socket();
    bool bind_socket() const;
    bool set_timeot(int sec, int usec) const;

    bool send_msg(std::vector<char> const& v, int flags=0) const;
    bool send_to(const void * buff, size_t len, int flags, const char * addr, int port) const;

    bool recv_msg(std::vector<char> & v, int flags=0) const;
    bool recv_from(const void * buff, size_t len, int flags, const char * addr, int port) const;

    bool check_result(int res) const;
    
    void close_socket();

    static unsigned int htnl(unsigned int val);
    static unsigned short int htns(unsigned short int val);

protected:
    // WinSocket requires that
    int _sock_init() const;
    // WinSocket requires that
    int _sock_quit() const;

private:
    // Socket file descriptor.
    // POSIX systems use int to describe sockets and
    // negative number to indicate invalid one,
    // while Windows (WinSocket) uses unsigned int
    // for that and special INVALID_SOCKET for 
    // invalids.
    int fd;
    std::string ip_address;
    int port;
    SockTransport transport;
};

} // namespace vpn



#endif // VPN_SOCKET
