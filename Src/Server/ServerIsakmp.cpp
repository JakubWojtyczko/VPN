#include "ServerIsakmp.h"
#include "../Logger.h"
#include "../SocketInc.h"

#include <string>
#include <cstring>

namespace vpn
{


std::thread ServerIsakmp::start() {
    // create socket, bind address and port
    Logger::getInstance().info("ClientIsakmp - prepare connection");
    if (!prepare_connection_for_isakmp()) {
        Logger::getInstance().error("ClientIsakmp: cannot prepare connetion");
        
    }
    return std::thread([this] {listen_and_handle();});

}

bool ServerIsakmp::prepare_connection_for_isakmp() {
    Logger::getInstance().info("ServerIsakmp: creating socket");
    if (!server_sock.create_socket()) {
        return false;
    }
    Logger::getInstance().info("ServerIsakmp: binding socket");
    if (!server_sock.bind_socket()) {
        server_sock.close_socket();
        return false;
    }
    return true;
}

bool ServerIsakmp::listen_and_handle() {
    std::string p_addr;
    int p_port;
    const int MAX_UDP_LEN = 2250;
    char buffer[MAX_UDP_LEN];
    while (this -> is_active) {
        int rec_len = server_sock.recv_from(buffer, MAX_UDP_LEN, 0, p_addr, p_port);
        if(rec_len > 0) {
            Logger::getInstance().info("ServerIsakmp: reveived " + std::to_string(rec_len) + " bytes");
            // now get it back and print it
            Logger::getInstance().info("froom " + p_addr);
      } else {
          Logger::getInstance().info("ServerIsakmp: listen - wrong :/");
      }
    }
    return true;
}

void ServerIsakmp::shut_down() {
    this -> is_active = false;
}


void ServerIsakmp::close_server() {
    Logger::getInstance().info("ServerIsakmp: closing connetion...");
    server_sock.close_socket();
    Logger::getInstance().info("ServerIsakmp: connetion closed");
}


} // namespace vpn
