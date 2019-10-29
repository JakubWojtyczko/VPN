#ifndef VPN_IP_COMMON
#define VPN_IP_COMMON

#include <string>


namespace vpn {


enum IPVersion {
    V4,
    V6
}


template <IPVersion IPVer>
class IPCommon {
    
    static std::uint32_t addr_from_string(std::string const& address) {
        std::uint32_t ret = 0;
        std::string dot = ".";
        switch (IPVer) {
            case V4:
                // 10.4.145.1
                size_t pos = 0;
                std::string token;
                while ((pos = s.find(dot)) != std::string::npos) {
                    token = s.substr(0, pos);

                    s.erase(0, pos + delimiter.length());
                }
            case V6:
        }
    } 
    
}


} // namespace vpn


#endif // VPN_IP_COMMON