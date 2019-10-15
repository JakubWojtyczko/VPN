#ifndef VPN_BUFFER
#define VPN_BUFFER

#include <cstring>

namespace vpn {

template <class T>
class Buffer {
    
public:

    enum status {
        buff_empty,
        buff_overflow,
        buff_underflow,
        buff_full
    };

    Buffer() : data(nullptr), size(0), _allocated(0), _status(buff_empty) {}
    virtual ~Buffer();

    bool reserve(unsigned size);
    bool empty() const;

    unsigned size() const;
    unsigned available_resources() const;
    T * c_data() const;

    T & operator[](int idx);
    T const& operator[](int idx) const;

    bool operator==(Buffer const& buffer) const;
    
    void clear();

    int write(T * data, unsigned data_size);


private:
    
    T * _data;
    unsigned _size;
    unsigned _allocated;
    status _status;
    
};

} // namespace vpn


// def


template <class T>
Buffer<T>::~Buffer() {
    this -> clear();
}


template <class T>
bool Buffer<T>::reserve(unsigned size) {
    try {
        this -> clear();
        this -> _data = new [size] T;
        this -> _allocated = size;
        // zero all resources
        std::memset(this -> _data, 0, sizeof(this -> _data));
    } catch (std::bad_alloc const& e) {
        // TODO log an error
        return 0;
    }
    return 1;
}


template <class T>
bool Buffer<T>::empty() const {
    return this -> _data == nullptr;
}


template <class T>
unsigned Buffer<T>::size() const {
    return this -> _size;
}


template <class T>
unsigned Buffer<T>::available_resources() const {
    return this -> _allocated - this -> _size;
}


template <class T>
T * Buffer<T>::c_data() const {
    return this -> _data;
}


template <class T>
T & Buffer<T>::operator[](int idx) {
    return this -> _data[idx];
}


template <class T>
T const& Buffer<T>::operator[](int idx) const {
    return this -> _data[idx];
}


template <class T>
bool Buffer<T>::operator==(Buffer const& buffer) const {
    if (this -> size != buffer.size()) {
        return 0;
    }
    for (int i=0; i< this -> size(); ++i) {
        if ((*this)[i] != bufffer[i]) {
            return 0;
        }
    }
    return 1;
}


template <class T>
void Buffer<T>::clear() {
    if (this -> empty() == false) {
        delete [] this -> _data;
        this -> _data = nullptr;
        this -> _size = this -> _allocated = 0;
    }
}


template <class T>
int Buffer<T>::write(T * data, unsigned data_size) {
    switch (this -> _status) {
        case buff_full:
        case buff_overflow:
            return 0;
        case buff_empty:
        case buff_underflow:
            if (data_size > this -> available_resources()) {
                this -> _status = buff_overflow;
            } else if (data_size < this -> available_resources()) {
                this -> _status = buff_underflow;
            } else {
                this -> _status = buff_full;
            }
            for (int counter=0; counter < data_size; ++counter) {
                // alloc
            }
    }
}


#enif // VPN_BUFFER
