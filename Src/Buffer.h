#ifndef VPN_BUFFER
#define VPN_BUFFER

#include <algorithm>
#include <vector>

namespace vpn {

template <class T>
class Buffer {
    
public:

    Buffer() {}
    Buffer(const T * begin, const T * end) : _data(begin, end) {}
    Buffer(const void * iter, size_t len) {
        this -> insert(iter, len);
    }

    virtual ~Buffer() {}

    inline void insert(T const& element) {
        _data.push_back(element);
    }

    inline void insert(Buffer const& other) {
        this -> insert(other._data);
    }

    inline void insert(const void * iter, size_t len) {
        // BEZPIECZNE TO ??
        std::copy((const T *)iter, (const T *)iter + len, std::back_inserter(_data));
    }

    inline void insert(std::vector<T> const& other) {
        this -> _data.insert(_data.end(), other.begin(), other.end());
    }

    inline T const * data() const {
        return _data.data();
    }

    inline T const& operator[](int i) const {
        return _data[i];
    }

    // return new buffer containing first n elements
    // flush returned data from itself
    Buffer get_first_n(size_t n) {
        size_t ret_size = std::min(_data.size(), n);
        Buffer <T> new_buffer(_data.data(), _data.data() + ret_size);
        this -> _data.erase(_data.begin(), _data.begin() + ret_size);
        return new_buffer;
    }

    Buffer get_last_n(size_t n) {
        size_t ret_size = std::min(_data.size(), n);
        Buffer <T> new_buffer(_data.data() + _data.size() - ret_size, _data.data() + _data.size());
        this -> _data.erase(_data.end() - ret_size, _data.end());
        return new_buffer;
    }

    inline Buffer operator+(Buffer const& other) const {
        Buffer <T> ret(_data);
        ret.insert(ret.end(), other._data.begin(), other._data.end());
        return ret;
    }

    inline size_t size() const {
        return this -> _data.size();
    }

    inline bool empty() const {
        return _data.empty();
    }

    inline void clear() {
        this -> _data.clear();
    }

private:
    
    std::vector <T> _data;
    
};

} // namespace vpn


#endif // VPN_BUFFER
