#ifdef _WINDOWS_H
  // There are some dependency issues here
  #warning "Please include SocketInc.h before <windows.h>"
#endif // _WINDOWS_H

#ifndef VPN_SOCKET_INC
#define VPN_SOCKET_INC

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


#endif // VPN_SOCKET_INC
