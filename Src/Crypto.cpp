#include "Crypto.h"
#include "Logger.h"
#include "Socket.h"
#include "Utils.h"

#include <string>
#include <cstring>
#include <cstdlib>

namespace vpn {


Buffer <std::uint8_t> Crypto::set_up_tunnel(Buffer <std::uint8_t> const& data) {
    EspHeader head = prepare_header();
    EspTrailer trailer = prepare_trailer(data.size(), 4);
    EspAuthData auth = prepare_auth();

    Buffer <std::uint8_t> tunnel;
    tunnel.insert(&head, sizeof(head));

    Buffer <std::uint8_t> to_encode;
    to_encode.insert(data);
    to_encode.insert(trailer.padding);
    to_encode.insert(trailer.pad_length);
    to_encode.insert(trailer.next_hreder);

    // Encode 
    tunnel.insert(encode(to_encode));

    tunnel.insert(&auth, sizeof(auth));
    return tunnel;
}


Buffer <std::uint8_t> Crypto::tear_down_tunnel(Buffer <std::uint8_t> tunnel) {
    Buffer <std::uint8_t> data;
    // read and veirify the esp header
    EspHeader head;
    Buffer <std::uint8_t> head_buff = tunnel.get_first_n(sizeof(head));
    std::memcpy(&head, head_buff.data(), head_buff.size());
    if (verify_header(head) == false) {
        return data;  // empty data
    }
    // last 32 bytes is an auth
    EspAuthData auth;
    Buffer <std::uint8_t> auth_buff = tunnel.get_last_n(ESP_AUTH_LEN);
    std::memcpy(&auth, auth_buff.data(), auth_buff.size());
    // verify authenticaiton data
    if (verify_auth(auth)) {
        return data;
    }

    // Decode rest of the tunnel
    tunnel = decode(tunnel);

    // Protocol id inside the tunnel
    std::uint8_t next_header = tunnel.get_last_n(1)[0];
    Logger::getInstance().info("AH: next header: " + str(next_header));
    // Padding length
    std::uint8_t pad_len = tunnel.get_last_n(1)[0];
    // read padding. we don't need it - utilize
    tunnel.get_last_n(pad_len);
    return tunnel;
}


void Crypto::reset_seq_num() {
    this -> sequence = 0;
}


std::uint32_t Crypto::next_seq_num() {
    this -> sequence += 1;
    return this -> sequence;
} 


bool Crypto::verify_header(EspHeader const& head) const {
    if (ntohl(head.spi) != this -> oth_spi) {
        Logger::getInstance().error("Esp: Invalid spi: " + str(ntohl(head.spi)) 
            + " but " + str(oth_spi)  + " was expected");
        return false;
    }
    if (ntohl(head.sqn) != this -> sequence) {
        Logger::getInstance().error("Esp: Invalid seq num: " + str(ntohl(head.sqn)) 
            + " but " + str(sequence)  + " was expected");
        return false;
    }
    return true;
}


EspHeader Crypto::prepare_header() const {
    EspHeader head;
    std::memset(&head, 0, sizeof(head));
    head.spi = htonl(this -> spi);
    head.sqn = htonl(this -> sequence);
    return head;
}


EspTrailer Crypto::prepare_trailer(int data_len, std::uint8_t next) const {
    EspTrailer trailer;
    // The data inside must be mutiple of 32 bytes.
    // Rest blocks have to be filled up a random values.
    std::uint8_t padding = data_len % sizeof(std::uint32_t);
    for (std::uint8_t i = 0; i < padding; ++i) {
        trailer.padding.push_back(std::rand() % 256);
    }
    trailer.pad_length = padding;
    trailer.next_hreder = next;
    return trailer;
}


EspAuthData Crypto::prepare_auth() const {
    EspAuthData auth;
    // TODO 
    // TODO
    return auth;
}


bool Crypto::verify_auth(EspAuthData const& auth) const {
    // TODO
    return true;
}


Buffer <std::uint8_t> Crypto::decode(Buffer <std::uint8_t> const& data) {
    // TODO
    return data;
}


Buffer <std::uint8_t> Crypto::encode(Buffer <std::uint8_t> const& data) {
    // TODO
    return data;
}


} // namespace vpn
