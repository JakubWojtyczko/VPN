#include "Usr.h"
#include "ClientIsakmp.h"
#include "Defines.h"
#include "Config.h"
#include "Utils.h"
#include "Tun.h"


#include <csignal>
#include <iostream>


bool halt = false;

int main() {
    // Read config from file
    // Note - method below can exit the program
    vpn::Config::read_config_file();
    vpn::Usr self(VPN_ANY_ADDRESS);

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

    vpn::Tun tun;
    if (!tun.alloc(vpn::Config::get_instance()["tun_name"])) {
        halt = true;
    }
    if (!tun.set_up()) {
        halt = true;
    }

    while(halt == false);

    isakmp.close();
    isakmp_thread.join();
    tun.delete_if();
    vpn::user_message("Disconnected successfully");
    return 0;
}
