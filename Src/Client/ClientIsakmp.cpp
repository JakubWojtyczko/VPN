#include "ClientIsakmp.h"
#include "../Logger.h"
#include "../IsakmpHeader.h"

namespace vpn {

const char * SERVER_IP = "127.0.12.1"; // TODO can't be hardcoded


bool ClientIsakmp::connect_to_server() {
    if (!prepare_connection_for_isakmp()) {
        Logger::getInstance().error("ClientIsakmp: cannot prepare connetion");
        return false;
    }

    // set timout 100 [ms] 
    cli_sock.set_timeot(0, 10000); // 0 sec, 10000 usec

    // Sending message 1 (initial) to the server
    Logger::getInstance().info("ClientIsakmp: sending msg1");

    Message1_2 msg1 = isakmp.prepare_message_1();
    if (!cli_sock.send_to(&msg1, sizeof(msg1), 0, SERVER_IP, 500)) {
        Logger::getInstance().error("Client Isakmp: cannot send msg1: " + cli_sock.last_error_str());
        return false;
    }

    // Receive message 2 (response) from the server
    Logger::getInstance().info("ClientIsakmp: waiting for msg2");

    Message1_2 msg2;
    if (!cli_sock.recv_from(&msg2, sizeof(msg2), 0, SERVER_IP, 500)) {
        Logger::getInstance().error("Timeout while waiting for msg2: " + cli_sock.last_error_str());
        return false;
    }

    return true;
}


bool ClientIsakmp::prepare_connection_for_isakmp() {
    Logger::getInstance().info("ClientIsakmp: creating socket");
    if (!cli_sock.create_socket()) {
        return false;
    }
    Logger::getInstance().info("ClientIsakmp: binding socket");
    if (!cli_sock.bind_socket()) {
        return false;
    }
    return true;
}


} // namespace vpn
