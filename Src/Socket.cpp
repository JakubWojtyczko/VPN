#include "Socket.h"

#ifdef _WIN32 // Windows
  #ifndef _WIN32_WINNT
      #define _WIN32_WINNT 0x0501 // Windows XP
  #endif // _WIN32_WINNT
  #include <winsock2.h>
  #include <Ws2tcpip.h>
#elif defined(__linux__) || defined(__unix__) || defined(_POSIX_VERSION)
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <netdb.h>
  #include <unistd.h>
  #include <errno.h>
#else // not Windows and not POSIX-like
  #error "Socket.cpp is not available on your OS. Windows and POSIX-like only!"
#endif // _WIN32

#include <string.h>

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
