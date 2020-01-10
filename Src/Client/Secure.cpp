#include "Buffer.h"
#include "Config.h"
#include "Logger.h"
#include "Secure.h"
#include "SocketInc.h"


#include <string>
#include <iostream>

namespace vpn {

Secure::~Secure() {
    sock.close_socket();
}


bool Secure::set_up() {
    if (sock.check_result(sock.create_socket()) == false) {
        return false;
    }
    if (sock.check_result(sock.bind_socket()) == false) {
        sock.close_socket();
        return false;
    }

    // Open live TUN deveice
    if (!capture.set_up()) {
        return false;
    }

    return true;
}


std::thread Secure::start_rx() {
    return std::thread([this] {rx_loop();});
}

std::thread Secure::start_tx() {
    return std::thread([this] {tx_loop();});
}

void Secure::rx_loop() {
    while (this -> is_active) {
        
    }
}

void Secure::tx_loop() {
    std::string addres = Config::get_instance()["server_ip"];
    int port = std::stoi(Config::get_instance()["sec_port"]);
    while (this -> is_active) {
        Buffer <std::uint8_t> buff = capture.receive();
        if (!buff.empty()) {
            Buffer <std::uint8_t> enc = crypto.set_up_tunnel(buff);
            sock.send_to(enc.data(), enc.size(), 0, addres.c_str(), port);
            Logger::getInstance().info("tx_loop");
        }
    }
}

void Secure::stop() {
    this -> is_active = false;
}

} // namespace vpn
