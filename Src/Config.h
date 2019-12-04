#ifndef VPN_CONFIG
#define VPN_CONFIG

#include <map>
#include <string>
#include <vector>

namespace vpn {

class Config {

    Config() {}
    virtual ~Config() {}

public:
    static void read_config_file();
    static Config & get_instance();

    std::string operator[](std::string const& key) const;
    std::vector <std::string> starts_with(std::string const& prefix) const;
private:
    static Config instance;
    std::map <std::string, std::string> data;
};


} // namespace vpn



#endif // VPN_CONFIG
