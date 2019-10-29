#include "Utils.h"

#include <cstdint>
#include <iostream>
#include <vector>

namespace vpn {
// 1011 1010 0000 0000 1010
// 1011 0101
// packed = 2
// offset = sizeo

template < class T, class P, class... Args>
T convert(P p, Args... args) {
    for (auto && x : {args...}) {
        
    }
   // int offset = sizeof(t) - packed - sizeof(p);
    T t;
    return t;// |= convert<T>(args..., packed + sizeof(p));
}
} // namespace vpn


int main() {

    std::uint16_t a = 12;
    std::uint16_t b = 13;
    std::cout << vpn::convert<std::uint32_t>(a, b, 0);
    return 0;
}

