#include "ClientIsakmp.h"
#include "Config.h"
#include "Logger.h"
#include "IsakmpHeader.h"
#include "Buffer.h"
#include "Utils.h"
#include "SocketInc.h"

#include <cstdlib>

namespace vpn {


bool ClientIsakmp::connect_to_server() {
    user_message("Connecting to the server (" + Config::get_instance()["server_ip"] + ")...");
    // create socket, bind address and port
    Logger::getInstance().info("ClientIsakmp - prepare connection");
    if (!prepare_connection_for_isakmp()) {
        Logger::getInstance().error("ClientIsakmp: cannot prepare connetion");
        return false;
    }
    cli_sock.set_timeout(5, 0); // 5 sec

    Logger::getInstance().info("ClientIsakmp - handshake begin");
    if (!handshake()) {
        Logger::getInstance().error("Handshake failed  " + cli_sock.last_error_str());
        cli_sock.close_socket();
        return false;
    }
    user_message("Connected successfully.");
    return true;
}


bool ClientIsakmp::handshake() {
    // get server ip
    std::string server_ip = Config::get_instance()["server_ip"];
    // get supported ISAKMP port
    int port = std::atoi(Config::get_instance()["isakmp_port"].c_str());

    // Send message 1 (initial) to the server
    Logger::getInstance().info("ClientIsakmp: sending msg1");
    Message1_2 msg1 = user.get_isakmp().prepare_message_1();
    if (!cli_sock.send_to(&msg1, sizeof(msg1), 0, server_ip.c_str(), port)) {
        Logger::getInstance().error("Client Isakmp: cannot send msg1: " + cli_sock.last_error_str());
        return false;
    }
    // set our new state - we are waiting fot connction
    user.update_state(UserConnectionState::PANDING);

    // Receive message 2 (response) from the server
    Logger::getInstance().info("ClientIsakmp: waiting for msg2");
    user_message("Waiting for server response...");
    Message1_2 msg2;
    std::string p_addr;
    if (cli_sock.recv_from(&msg2, sizeof(msg2), 0, p_addr, port) < 0) {
        user_message("Response timed out. Server unavailable");
        return false;
    }

    Logger::getInstance().info("ClientIsakmp: Msg2 received. Veryfing...");
    if (user.get_isakmp().verify_message_2(msg2) != SUCCESS) {
        Logger::getInstance().error("ClientIsakmp: Msg2 verification failed");
        return false;
    }
    
    // update state - establishment
    user.update_state(UserConnectionState::ESTABLISHMENT);

    // send message 3
    Logger::getInstance().info("ClientIsakmp: sending msg3");

    Buffer<std::uint8_t> msg3 = user.get_isakmp().prepare_message_3();
    if (!cli_sock.send_to(msg3.data(), msg3.size(), 0, server_ip.c_str(), port)) {
        Logger::getInstance().error("Client Isakmp: cannot send msg3: " + cli_sock.last_error_str());
        return false;
    }

    // update state - connected (able to send a data now)
    user.update_state(UserConnectionState::ESTABLISHED);

    return true;
}

std::thread ClientIsakmp::start() {
    return std::thread([this] {listen();});
}

void ClientIsakmp::listen() {
    std::string p_addr;
    int port, len;
    unsigned char buffer[1501];
    while (is_active) {
        len = cli_sock.recv_from(buffer, 1500, MSG_DONTWAIT, p_addr, port);
        if (len < 0) {    
            // zero data received 
            continue;
        } else {
            if (buffer[16] == 12) {
                // delete request
                cli_sock.close_socket();
                user_message("** Halted by server **");
                exit(0);
            }
        }
    }
}

void ClientIsakmp::close() {
    this -> disconnect();
    this -> is_active = false;
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


bool ClientIsakmp::disconnect() {
    // send delete request
    IsakmpDeleteReq d_req = user.get_isakmp().prepare_delete_req();
    Buffer<std::uint8_t> req(&d_req, sizeof(d_req));
    int port = std::atoi(Config::get_instance()["isakmp_port"].c_str());
    std::string server_ip = Config::get_instance()["server_ip"];
    if (!cli_sock.send_to(req.data(), req.size(), 0, server_ip.c_str(), port)) {
        Logger::getInstance().error("Client Isakmp: cannot send delete req: " + cli_sock.last_error_str());
        return false;
    }
    return true;
}


} // namespace vpn
