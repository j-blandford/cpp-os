#pragma once

#include <std.h>
#include <stddef.h>


namespace std {
    template<class T> 
    class vector {
        size_t _size;
        size_t _capacity;
        T* buffer;

    public:
        typedef T* iterator; // used as vector::iterator

        // Ways to initialise this list
        vector();
       // vector(size_t size);
        vector(const vector<T>& vec);
        ~vector();

        void clear();

        void resize(size_t size);
        void reserve(size_t capacity);

        void push_back(const T& value);
        void pop_back();

        size_t capacity() const;
        size_t size() const;

        T& operator[](size_t index);
        vector<T>& operator=(const vector<T> & v);

    };
}

namespace std {

    template<class T>
    vector<T>::vector() :
        _capacity(0),
        _size(0),
        buffer(NULL)
    { }

    template<class T>
    vector<T>::vector(const vector<T>& vec) :
        _capacity(vec.capacity),
        _size(vec.size),
        buffer(NULL) 
    { 
        for(size_t i = 0; i < _size; i++) {
            buffer[i] = vec.buffer[i];
        }
    }

	template<class T>
	vector<T>::~vector() { }

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
	vector<T>& vector<T>::operator= (const vector<T> & v) {
		delete[] buffer;
		_size = v._size;

		_capacity = v._capacity;
		buffer = new T [_capacity];
		for (unsigned int i = 0; i < _size; i++)
			buffer[i] = v.buffer[i];
		return *this;
	}

	template<class T>
	T& vector<T>::operator[](size_t index) {
		return buffer[index];
	}
}
