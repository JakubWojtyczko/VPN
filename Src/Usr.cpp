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


} // namespace vpn

