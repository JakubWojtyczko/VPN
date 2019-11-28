#include "Usr.h"
#include "ServerIsakmp.h"
#include "Threads.h"
#include "Config.h"

#include <vector>
#include <iostream>

int main(int argc, char * argv[]) {
    // Read config from file
    // Note - method below can exit the program
    vpn::Config::read_config_file();
    vpn::Usr self("127.0.12.1");
    std::vector<vpn::Usr> clients;
    vpn::ServerIsakmp isakmp(self, clients);

    // run ISAKMP in the background
    std::thread isakmp_thread = isakmp.start();

    std::string s;
    std::cin >> s;

    isakmp.shut_down();
    isakmp.close_server();
    isakmp_thread.join();

    return 0;

}