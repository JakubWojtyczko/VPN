#include "Usr.h"
#include "ServerIsakmp.h"
#include "Threads.h"
#include "Config.h"
#include "Utils.h"

#include <vector>
#include <iostream>
#include <csignal>

bool halt = false;

int main(int argc, char * argv[]) {
    // Read config from file
    // Note - method below can exit the program
    vpn::Config::read_config_file();
    vpn::Usr self(vpn::Config::get_instance()["server_ip"]);
    std::vector<vpn::Usr> clients;
    vpn::ServerIsakmp isakmp(self, clients);

    // run ISAKMP in the background
    std::thread isakmp_thread = isakmp.start();

    // Handle Ctrl+C event
    std::signal(SIGINT, 
        [](int s) -> void {vpn::user_message("\nHalted by user"); halt=true;}
    );

    while (halt == false);

    isakmp.shut_down();
    isakmp.close_server();
    isakmp_thread.join();

    return 0;

}
