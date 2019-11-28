#include "Usr.h"
#include "ClientIsakmp.h"
#include "Config.h"

#include <iostream>


int main() {
    // Read config from file
    // Note - method below can exit the program
    vpn::Config::read_config_file();
    vpn::Usr self("127.0.0.1");
    vpn::ClientIsakmp isakmp(self);

    if (isakmp.connect_to_server() == false) {
        exit(1);
    }

    std::thread isakmp_thread = isakmp.start();

    std::string str;
    std::cin >> str;
    isakmp.close();
    isakmp_thread.join();

    return 0;
}
