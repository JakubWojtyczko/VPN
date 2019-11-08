#include "Usr.h"

namespace vpn
{

void Usr::update_state(UserConnectionState new_state) {
    this -> state = new_state;
}

    
} // namespace vpn

