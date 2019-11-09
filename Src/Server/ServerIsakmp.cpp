#include "ServerIsakmp.h"
#include "../Logger.h"

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
    while (this -> is_active) {

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
