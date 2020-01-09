#include "Usr.h"
#include "ServerIsakmp.h"
#include "Threads.h"
#include "Config.h"
#include "Utils.h"
#include "Tun.h"

#include <vector>
#include <iostream>
#include <csignal>
#include <sstream>

bool halt = false;

int main(int argc, char * argv[]) {
    // Read config from file
    // Note - method below can exit the program
    vpn::Config::read_config_file();
    std::string ip = vpn::Config::get_instance()["server_ip"];
    vpn::Usr self(ip);
    // generate SPI for server
    self.get_isakmp().get_next_spi(true);
    std::vector<vpn::Usr> clients;
    vpn::ServerIsakmp isakmp(self, clients);

    // run ISAKMP in the background
    std::thread isakmp_thread = isakmp.start();
    vpn::user_message("Server is running [IP=" + ip + "]\n");

    // create and allocate the tun interface
    vpn::Tun tun;
    if (!tun.alloc(vpn::Config::get_instance()["tun_name"])) {
        halt = true;
    }
    // Change route settings
    if (tun.set_up() == false) {
        halt = true;
    }

    // Handle Ctrl+C event
    std::signal(SIGINT, 
        [] (int s) -> void {
            vpn::user_message("\n** Halted by user **"); 
            halt = true;}
    );

    while (halt == false);

    isakmp.disconnect_all_users();
    isakmp.shut_down();
    isakmp.close_server();
    isakmp_thread.join();

    // delete the tun interface
    tun.delete_if();

    vpn::user_message("Server shut down successfully");
    return 0;
}
