#include "Usr.h"
#include "ClientIsakmp.h"
#include "Config.h"
#include "Utils.h"

#include <csignal>
#include <iostream>


bool halt = false;

int main() {
    // Read config from file
    // Note - method below can exit the program
    vpn::Config::read_config_file();
    vpn::Usr self(vpn::Config::get_instance()["client_ip"]);

    // prepare SSL parameters to exchange
    if (self.prepare_ssl() == false) {
        vpn::exit_with_error("Cannot prepare SSL parameters");
    }

    vpn::ClientIsakmp isakmp(self);

    if (isakmp.connect_to_server() == false) {
        vpn::exit_with_error("Cannot connect to the server");
    }

    std::thread isakmp_thread = isakmp.start();

    // Handle Ctrl+C event
    std::signal(SIGINT, 
        [] (int s) -> void {
            vpn::user_message("\n** Halted by user **"); 
            halt=true;
        }
    );

    while(halt == false);

    isakmp.close();
    isakmp_thread.join();
    vpn::user_message("Disconnected successfully");
    return 0;
}
