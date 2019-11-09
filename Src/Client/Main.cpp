#include "../Usr.h"
#include "ClientIsakmp.h"



int main() {
    
    vpn::Usr self("127.0.0.1");
    vpn::ClientIsakmp isakmp(self);

    isakmp.connect_to_server();


    return 0;
}
