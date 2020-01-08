#include "Isakmp.h"
#include "Usr.h"


namespace vpn
{

void Usr::update_state(UserConnectionState new_state) {
    this -> state = new_state;
}

Isakmp & Usr::get_isakmp() { 
    return this -> isakmp; 
}


Queue & Usr::get_from_phy_queue() {
    return this -> from_phy;
}


Queue & Usr::get_to_phy_queue() {
    return this -> to_phy;
}


Queue & Usr::get_from_tap_queue() {
    return this -> from_tap;
}


Queue & Usr::get_to_tap_queue() {
    return this -> to_tap;
}

bool Usr::prepare_ssl() {
    return this -> ssl.init();
}

} // namespace vpn

