#include "Socket.h"

#ifdef _WIN32 // Windows
  #ifndef _WIN32_WINNT
      #define _WIN32_WINNT 0x0501 // Windows XP
  #endif // _WIN32_WINNT
  #include <winsock2.h>
  #include <Ws2tcpip.h>
#elif defined(__linux__) || defined(__unix__) || defined(_POSIX_VERSION)
  #include <sys/socket.h>
  #include <sys/time.h>
  #include <arpa/inet.h>
  #include <netdb.h>
  #include <unistd.h>
  #include <errno.h>
#else // not Windows and not POSIX-like
  #error "Socket.cpp is not available on your OS. Windows and POSIX-like only!"
#endif // _WIN32

#include <string.h>

#include "Logger.h"

namespace vpn {


int Socket::_sock_init() const {
#ifdef _WIN32
    WSADATA wsa_data;
    return WSAStartup(MAKEWORD(1,1), &wsa_data);
#else
    return 0;
#endif // _WIN32
}


int Socket::_sock_quit() const {
#ifdef _WIN32
    return WSACleanup();
#else
    return 0;
#endif // _WIN32
}


int Socket::last_error() const {
#ifdef _WIN32
    return WSAGetLastError();
#else
    return errno;
#endif // _WIN32
}


std::string Socket::last_error_str() const {
#ifdef _WIN32
    wchar_t *s = NULL;
    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM 
                   | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, WSAGetLastError(),
                     MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&s, 0, NULL);
    std::wstring w_str(s);
    std::string ret(w_str.begin(), w_str.end());
    LocalFree(s);
    return ret;
#else
    return strerror(errno);
#endif // _WIN32
}

bool Socket::create_socket() {
    if(this -> _sock_init() != 0) {
        Logger::getInstance().error("Cannot init socet: " + last_error_str());
        return 0;
    }

    int new_socket;
    switch(this -> transport) {
        case TCP:
            new_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        case UDP:
            new_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }
    if (check_result(new_socket)) {
        this -> fd = new_socket;
        return true;
    } else {
        Logger::getInstance().error("Cannot create socet: " + last_error_str());
        return false;
    } 
}

bool Socket::bind_socket() const{
    sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = inet_addr(ip_address.c_str());
    local.sin_port = htons(port);
    if (!check_result(bind(fd, (sockaddr *)&local, sizeof(local)))) {
        Logger::getInstance().error("Cannot bind socket: " + last_error_str());
        return false;
    }
    return true;
}

bool Socket::set_timeot(int sec, int usec) const {
    timeval tv;
    tv.tv_sec = sec;
    tv.tv_usec = usec;
    if (check_result(setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char * )&tv, sizeof(tv)))) {
        Logger::getInstance().error("Socket:set timeout failed: " + last_error_str());
        return false;
    }
    return true;
}

bool Socket::send_msg(std::vector<char> const& v, int flags) const {
    int res = send(this -> fd, v.data(), v.size(), flags);
    if (check_result(res) == false) {
        Logger::getInstance().error("Socket::send_msg: " + last_error_str());
        return false;
    }
    return true;
}

bool Socket::send_to(const void * buff, size_t len, int flags, const char * addr, int port) const {
    sockaddr_in destination;
    memset(&destination, 0 , sizeof(destination));
    destination.sin_family = AF_INET;
    destination.sin_addr.s_addr = inet_addr(addr);
    destination.sin_port = htons(port);

    if (!check_result(sendto(fd, (char *)buff, len, flags, (sockaddr *)&destination, sizeof(destination)))) {
        Logger::getInstance().error("Socket: sendto - cannot send: " + last_error_str());
        return false;
    }
    return true;
}

bool Socket::recv_msg(std::vector<char> & v, int flags) const {
    char buff[1000];
    int res = recv(this -> fd, buff, sizeof(buff), flags);
    if (check_result(res) == false) {
        Logger::getInstance().error("Socket::send_msg: " + last_error_str());
        return false;
    }
    v.clear();
    v.insert(v.begin(), buff, buff + res);
    return true;
}


bool Socket::recv_from(const void * buff, size_t len, int flags, const char * addr, int port) const {
    sockaddr_in destination;
    memset(&destination, 0 , sizeof(destination));
    destination.sin_family = AF_INET;
    destination.sin_addr.s_addr = inet_addr(addr);
    destination.sin_port = htons(port);
    int address_len = sizeof(destination);
    if (!check_result(recvfrom(fd, (char *)buff, len, flags, (sockaddr *)&destination, &address_len))) {
        Logger::getInstance().error("Socket: recvfrom failed: " + last_error_str());
        return false;
    }
    return true;
}

bool Socket::check_result(int result) const {
#ifdef _WIN32
    return result != SOCKET_ERROR;
#else
    return result >= 0;
#endif // _WIN32  
}

void Socket::close_socket() {
#ifdef _WIN32
    closesocket(this -> fd);
    this -> fd = INVALID_SOCKET;
#else
    close(this -> fd);
    this -> fd = -1;
#endif // _WIN32
    this -> _sock_quit();
}


} // namespace vpn
