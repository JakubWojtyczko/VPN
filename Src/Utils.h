#ifndef VPN_UTILS
#define VPN_UTILS

#include <string>

namespace vpn {



template <class T>
std::string str(T const& n) {
    return std::to_string(n);
}



}

#endif // VPN_UTILS
