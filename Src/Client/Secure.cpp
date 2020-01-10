#include "Secure.h"
#include "SocketInc.h"

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
    while (this -> is_active) {

    }
}

void Secure::stop() {
    this -> is_active = false;
}

} // namespace vpn
