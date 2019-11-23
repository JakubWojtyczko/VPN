#ifndef VPN_UTILS
#define VPN_UTILS

#include <cstdlib>
#include <string>
#include <cstdio>

namespace vpn {



template <class T>
std::string str(T const& n) {
    return std::to_string(n);
}


void exit_with_error(const char * error, int err_no=1);


}

#endif // VPN_UTILS
