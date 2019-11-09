#include "ClientIsakmp.h"
#include "../Logger.h"
#include "../IsakmpHeader.h"

namespace vpn {

const char * SERVER_IP = "127.0.12.1"; // TODO can't be hardcoded

bool ClientIsakmp::connect_to_server() {
    // create socket, bind address and port
    Logger::getInstance().info("ClientIsakmp - prepare connection");
    if (!prepare_connection_for_isakmp()) {
        Logger::getInstance().error("ClientIsakmp: cannot prepare connetion");
        return false;
    }
    // set timout 100 [ms] 
    cli_sock.set_timeot(0, 10000); // 0 sec, 10000 usec

    Logger::getInstance().info("ClientIsakmp - handshake begin");
    if (!handshake()) {
        Logger::getInstance().error("Handshake failed  " + cli_sock.last_error_str());
        cli_sock.close_socket();
        return false;
    }
    Logger::getInstance().info("ClientIsakmp - handshake end");
    return true;
}


bool ClientIsakmp::handshake() {
    
    // Send message 1 (initial) to the server
    Logger::getInstance().info("ClientIsakmp: sending msg1");

    Message1_2 msg1 = isakmp.prepare_message_1();
    if (!cli_sock.send_to(&msg1, sizeof(msg1), 0, SERVER_IP, 500)) {
        Logger::getInstance().error("Client Isakmp: cannot send msg1: " + cli_sock.last_error_str());
        return false;
    }
    // set our new state - we are waiting fot connction
    user.update_state(UserConnectionState::PANDING);

    // Receive message 2 (response) from the server
    Logger::getInstance().info("ClientIsakmp: waiting for msg2");

    Message1_2 msg2;
    std::string p_addr;
    int p_port = 500;
    if (!cli_sock.recv_from(&msg2, sizeof(msg2), 0, p_addr, p_port)) {
        Logger::getInstance().error("Timeout while waiting for msg2: " + cli_sock.last_error_str());
        return false;
    }

    Logger::getInstance().info("ClientIsakmp: Msg2 received. Veryfing...");
    if (isakmp.verify_message_2(msg2) != SUCCESS) {
        Logger::getInstance().error("ClientIsakmp: Msg2 verification failed");
        return false;
    }
    
    // update state - establishment
    user.update_state(UserConnectionState::ESTABLISHMENT);

    // send message 3
    Logger::getInstance().info("ClientIsakmp: sending msg3");

    std::vector<std::uint8_t> msg3 = isakmp.prepare_message_3();
    if (!cli_sock.send_to(msg3.data(), msg3.size(), 0, SERVER_IP, 500)) {
        Logger::getInstance().error("Client Isakmp: cannot send msg3: " + cli_sock.last_error_str());
        return false;
    }

    // update state - connected (able to send a data now)
    user.update_state(UserConnectionState::ESTABLISHED);

    return true;
}


bool ClientIsakmp::prepare_connection_for_isakmp() {
    Logger::getInstance().info("ClientIsakmp: creating socket");
    if (!cli_sock.create_socket()) {
        return false;
    }
    Logger::getInstance().info("ClientIsakmp: binding socket");
    if (!cli_sock.bind_socket()) {
        cli_sock.close_socket();
        return false;
    }
    return true;
}


} // namespace vpn
