#include "ServerIsakmp.h"
#include "Logger.h"
#include "SocketInc.h"
#include "Isakmp.h"
#include "IsakmpHeader.h"
#include "Utils.h"

#include <string>
#include <cstring>

namespace vpn
{


std::thread ServerIsakmp::start() {
    // create socket, bind address and port
    Logger::getInstance().info("ClientIsakmp - prepare connection");
    if (!prepare_connection_for_isakmp()) {
        Logger::getInstance().error("ClientIsakmp: cannot prepare connetion");
        exit(1);
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
        int rec_len = server_sock.recv_from(buffer, MAX_UDP_LEN, MSG_DONTWAIT, p_addr, p_port);
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
          // zero bytes received
          continue;
      }
    }
    return true;
}


void ServerIsakmp::handle(std::string const& ip, char * buf, int len) {
    Logger::getInstance().info("ServerIsakmp::handle");
    if (is_client_added(ip) == -1) {
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
    clients.push_back(Usr(ip, self.get_isakmp().get_spi()));
    
    Message1_2 msg1;
    std::memcpy(&msg1, buf, len);
    if (clients.back().get_isakmp().verify_message_1(msg1) != SUCCESS) {
        Logger::getInstance().error("ServerIsakmp: msg1 verification failed");
        clients.erase(clients.end() - 1);
        return; // TODO respnse with failed code !
    }
    // prepare SSL parameters for new client
    if (clients.back().prepare_ssl() == false) {
        Logger::getInstance().error("IsakmpServer: Error with SSL parameters.");
        return;
    }
    // send message 2
    Message1_2 msg2 = clients.back().get_isakmp().prepare_message_2();
    const int msg2_size = sizeof(msg2);
    unsigned char buffer[msg2_size];
    memcpy(buffer, &msg2, msg2_size);
    server_sock.send_to(buffer, msg2_size, 0, ip.c_str(), Isakmp::PORT);

    user_message("New client: " + ip);
}

void ServerIsakmp::handle_exist(std::string const& ip, char * buf, int len) {
    if (is_client_added(ip) == false) {
        int payload = (int)(buf[16]);
        if (payload == 12) {
            // delete request
            if (len != sizeof(IsakmpDeleteReq)) {
                Logger::getInstance().error("ServerIsakmp delete req too short");
                return;
            }
            IsakmpDeleteReq req;
            std::memcpy(&req, buf, len);
            int cli_idx = find_by_spi(req.head.init_spi);
            if (cli_idx == -1) {
                Logger::getInstance().error("ServerIsakmp:handle_exist - Unknown spi: " + str(req.head.init_spi));
                return;
            }
            user_message(std::string("Deleted user with ip ") + clients[cli_idx].get_ip());
            clients.erase(clients.begin() + cli_idx);
        }
    } 
}

int ServerIsakmp::find_by_spi(std::uint64_t spi) const {
    for (int i=0; i<(int)clients.size(); ++i) {
        if (clients[i].get_isakmp().get_spi(true) == spi) {
            return i;
        }
    }
    return -1;
}

void ServerIsakmp::shut_down() {
    this -> is_active = false;
}


void ServerIsakmp::close_server() {
    user_message("Shutting down...");
    Logger::getInstance().info("ServerIsakmp: closing connetion...");
    server_sock.close_socket();
    Logger::getInstance().info("ServerIsakmp: connetion closed");
    this -> shut_down();
}


int ServerIsakmp::is_client_added(std::string const& ip) const {
    for (unsigned i=0; i < clients.size(); ++i) {
        if (clients[i].get_ip() == ip) {
            return i;
        }
    }
    return -1;
}

void ServerIsakmp::disconnect_all_users() {
    user_message("Disconnecting all users...");
    for (auto & client : clients) {
        IsakmpDeleteReq req = client.get_isakmp().prepare_delete_req();
        int ret = server_sock.send_to(&req, sizeof(req), 0, client.get_ip().c_str(), Isakmp::PORT); 
        if (ret < 0) {
            Logger::getInstance().error("Cannot send delete request");
        }
    }
    clients.clear();
}

} // namespace vpn
