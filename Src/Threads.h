#ifndef VPN_THREADS
#define VPN_THREADS

#ifdef _WIN32 // Windows
  #ifndef _WIN32_WINNT
      #define _WIN32_WINNT 0x0501 // Windows XP
  #endif // _WIN32_WINNT
  // Windows has no std::thread and similar classes.
  // The following library was download from here:
  // https://github.com/meganz/mingw-std-threads
  // Please read README.md and LICENCE file.
  #include "mingw_std_threads/mingw.thread.h"
#elif defined(__linux__) || defined(__unix__) || defined(_POSIX_VERSION)
  #include <thread>
#else // not Windows and not POSIX-like
  #error "Socket.cpp is not available on your OS. Windows and POSIX-like only!"
#endif // _WIN32

#endif // VPN_THREADS
