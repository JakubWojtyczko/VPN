#ifndef VPN_CONFIG
#define VPN_CONFIG

#include <map>
#include <string>

namespace vpn {

class Config {

    Config() {}
    virtual ~Config() {}

public:
    static void read_config_file();
    static Config & get_instance();

    std::string const& operator[](std::string const& key) const;
private:
    static Config instance;
    std::map <std::string, std::string> data;
};


} // namespace vpn



#endif // VPN_CONFIG
