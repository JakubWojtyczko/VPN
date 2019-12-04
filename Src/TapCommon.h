#ifndef VPN_TAP_COMMON
#define VPN_TAP_COMMON

#ifdef _WIN32 // Windows
  #ifndef _WIN32_WINNT
      #define _WIN32_WINNT 0x0501 // Windows XP
  #endif // _WIN32_WINNT
  #include "TapWin.h"
#elif defined(__linux__) || defined(__unix__) || defined(_POSIX_VERSION)
  #include "TapLin.h"
#else // not Windows and not POSIX-like
  #error "TapCommon.h is not available on your OS. Windows and POSIX-like only!"
#endif // _WIN32

#endif // VPN_TAP_COMMON
