#ifndef VPN_UTILS
#define VPN_UTILS

namespace vpn {

// uint8, uint8 -> uint16
// uint16, uint8, uint8 -> uint32

template <class T, class... Args>
T convert(Args... args);

}

#endif // VPN_UTILS