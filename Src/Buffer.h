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

    inline void insert(const void * iter, size_t len) {
        // BEZPIECZNE TO ??
        std::copy((const T *)iter, (const T *)iter + len, std::back_inserter(_data));
    }

    inline void insert(std::vector<T> const& other) {
        this -> _data.insert(_data.end(), other.begin(), other.end());
    }

    inline T * data() {
        return _data.data();
    }

    inline Buffer operator+(Buffer const& other) const {
        Buffer <T> ret(_data);
        ret.insert(ret.end(), other._data.begin(), other._data.end());
        return ret;
    }

    inline size_t size() const {
        return this -> _data.size();
    }

    inline void clear() {
        this -> _data.clear();
    }

private:
    
    std::vector <T> _data;
    
};

} // namespace vpn


#endif // VPN_BUFFER
