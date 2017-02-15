#include <std.h>
#include <stddef.h>

namespace std {

    template<class T>
    vector<T>::vector() :
        _capacity(0),
        _size(0),
        buffer(new char[sizeof(T) * _capacity])
    { }

    template<class T>
    vector<T>::vector(const vector<T>& vec) :
        _capacity(vec.capacity),
        _size(vec.size),
        buffer(new char[sizeof(T) * _capacity]) 
    { 
        for(size_t i = 0; i < _size; i++) {
            buffer[i] = vec.buffer[i];
        }
    }

    template<class T>
    void vector<T>::push_back(const T& value) {

        if(_size >= _capacity) {
            reserve(_capacity*1.5);
        }

        buffer[_size] = value;
        _size++;
    }
    
    template<class T>
    void vector<T>::pop_back() {
        (reinterpret_cast<T*>(buffer)[_size-1]).~T();
        --_size;
    }

    template<class T>
    void vector<T>::reserve(size_t capacity) {
        T* buffer_new = new T[capacity];

         _capacity = capacity;

        for(size_t i = 0; i < _size; i++) {
            buffer_new[i] = buffer[i];
        }
        delete[] buffer;
        buffer = buffer_new;
    }

    template<class T>
    T& vector<T>::at(size_t index) {
        return buffer[index];
    }

    template<class T>
    T& vector<T>::operator[](size_t index) {
        return buffer[index];
    }
}