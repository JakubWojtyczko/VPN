#include "ServerIsakmp.h"
#include "../Logger.h"
#include "../SocketInc.h"
#include "../Isakmp.h"

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
            Logger::getInstance().info("ServerIsakmp: reveived " + std::to_string(rec_len)
                + " bytes from " + p_addr + ":" + std::to_string(p_port));

            if (p_port != Isakmp::PORT) {
                // wrong source port. refuse it
                Logger::getInstance().error("ServerIsakmp: message from " + p_addr +
                    + ":" + std::to_string(p_port) + " was reveived but not expected");
                continue;
            }
            handle(p_addr, buffer, rec_len);
      } else {
          Logger::getInstance().info("ServerIsakmp: listen - zero data reveived");
      }
    }
    return true;
}


void ServerIsakmp::handle(std::string const& ip, char * buf, int len) {
    Logger::getInstance().info("ServerIsakmp::handle");
    if (is_client_added(ip) == -1) {
        Logger::getInstance().info("New client: " + ip);
        handle_new(ip, buf, len);
    } else {
        handle_exist(ip, buf, len);
    }
}

void ServerIsakmp::handle_new(std::string const& ip, char * buf, int len) {
    Logger::getInstance().info("ServerIsakmp::handle_new");
    if(sizeof(Message1_2) != len) {
        Logger::getInstance().error("ServerIsakmp: msg1 wrong size: " + std::to_string(len)
            + " but " + std::to_string(sizeof(Message1_2)) + " was expected");
            return;
    }
    clients.push_back(Usr(ip));
    Message1_2 msg1;
    std::memcpy(&msg1, buf, len);
    if (clients.back().get_isakmp().verify_message_1(msg1) != SUCCESS) {
        Logger::getInstance().error("ServerIsakmp: msg1 verification failed");
        return; // TODO respnse with failed code !
    }
    // send message 2
    Message1_2 msg2 = clients.back().get_isakmp().prepare_message_2();
    const int msg2_size = sizeof(msg2);
    unsigned char buffer[msg2_size];
    memcpy(buffer, &msg2, msg2_size);
    server_sock.send_to(buffer, msg2_size, 0, ip.c_str(), Isakmp::PORT);
}

void ServerIsakmp::handle_exist(std::string const& ip, char * buf, int len) {
    if (is_client_added(ip) == false) {
       
    }
}


void ServerIsakmp::shut_down() {
    this -> is_active = false;
}


void ServerIsakmp::close_server() {
    Logger::getInstance().info("ServerIsakmp: closing connetion...");
    server_sock.close_socket();
    Logger::getInstance().info("ServerIsakmp: connetion closed");
}


int ServerIsakmp::is_client_added(std::string const& ip) const {
    for (unsigned i=0; i < clients.size(); ++i) {
        if (clients[i].get_ip() == ip) {
            return i;
        }
    }
    return -1;
}


} // namespace vpn
