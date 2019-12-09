#include "Queue.h"
#include "Buffer.h"
#include "Logger.h"



namespace vpn {
    
void Queue::push(Buffer <std::uint8_t> const& buffer) {
    this -> data.push(buffer);
}


Buffer <std::uint8_t> Queue::pull() {
#if defined(DEBUG) || defined(W_DEBUG)
    if (data.empty()) {
        Logger::getInstance().error("Queue pull but empty");
    }
#endif // defined(DEBUG) || defined(W_DEBUG)

    Buffer <std::uint8_t> buffer = this -> data.front();
    this -> data.pop();
    return buffer;
}


bool Queue::empty() const {
    return this -> data.empty();
}



} // namespace vpn
