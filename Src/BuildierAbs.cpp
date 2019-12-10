#include "BuilderAbs.h"
#include "Config.h"
#include "Defines.h"
#include "Logger.h"
#include "Utils.h"


namespace vpn {
    

bool BuilderAbs::load_dns_from_conf() {
    bool missing_dns = true;
    std::vector <std::string> dns_c = Config::get_instance().starts_with(VPN_CONFIG_DNS_PREFIX);
    for (auto const& sin_dns : dns_c) {
        DNS <IPVersion::V4> new_dns(sin_dns);
        if (new_dns.validate()) {
            this -> dns.push_back(new_dns);
            missing_dns = false;
        } else {
            Logger::getInstance().error("BuldierAbs: invalid DNS in config " + new_dns.get_address());
        }
    }
    Logger::getInstance().info("BuldierAbs: DNS from conf: " + str(dns.size()));
    return missing_dns;
}


bool BuilderAbs::prepare_tap() {
    if (tap.tap_interface_exists() == false) {
        return false;
    }
    if (tap.store_route_table() == false) {
        return false;
    }
    if (tap.set_up_route_table(this -> route) == false) {
        return false;
    }
    return true;
}


} // namespace vpn
