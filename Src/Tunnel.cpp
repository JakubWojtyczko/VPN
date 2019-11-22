#include "Tunnel.h"

#include <cstring>

namespace vpn {
    

Buffer<std::uint8_t> Tunnel::set_up_tunnel(Buffer<std::uint8_t> const& data) {
    // prepare AH
    AuthenticationHeader ah = auth.prepare_header();
    // prepare ESP
    Buffer<std::uint8_t> esp = crypto.set_up_tunnel(data);
    if (esp.empty())  {
        // ESP failed - return empty buffer 
        return esp;
    }
    // prepare AH + ESP tunnel
    Buffer<std::uint8_t> tunnel;
    tunnel.insert(&ah, sizeof(ah));
    tunnel.insert(esp);
    return tunnel;
}


Buffer<std::uint8_t> Tunnel::tear_down_tunnel(Buffer<std::uint8_t> tunnel) {
    Buffer<std::uint8_t> data;
    // get authentication header
    AuthenticationHeader ah;
    Buffer<std::uint8_t> ah_buff = tunnel.get_first_n(sizeof(ah));
    std::memcpy(&ah, ah_buff.data(), sizeof(ah));
    // verify authentication header
    if(auth.vevify_header(ah) == false) {
        // auth failed - return empty buffer
        return data;
    }
    // The rest "belongs" to ESP. Tear down the ESP tunnel
    // In error case the method below returns an empty buffer
    return crypto.tear_down_tunnel(tunnel);
}


} // namespace vpn
