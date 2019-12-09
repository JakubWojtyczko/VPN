#ifndef VPN_QUEUE
#define VPN_QUEUE

#include <queue>
#include <cinttypes>

#include "Buffer.h"

namespace vpn {
    

class Queue {

public:

    Queue() {}
    virtual ~Queue() {}

    void push(Buffer <std::uint8_t> const& buffer);
    Buffer <std::uint8_t> pull();

    bool empty() const;

private:
    std::queue <Buffer <std::uint8_t>> data;
};


} // namespace vpn


#endif // VPN_QUEUE
