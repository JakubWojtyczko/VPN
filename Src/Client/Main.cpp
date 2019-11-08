#include "../Usr.h"
#include "ClientIsakmp.h"



int main() {
    
    vpn::Usr self;
    vpn::ClientIsakmp isakmp(self);

    isakmp.connect_to_server();


    return 0;
}
