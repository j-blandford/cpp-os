#pragma once

#include <std.h>
#include <stddef.h>

#include <cpu/io.h>

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
        _capacity(vec._capacity),
        _size(vec._size),
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
            reserve(_capacity*1.5); // 1.5 is a bit less than the golden ratio, perfect for our memory allocation uses
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
        //delete[] buffer;
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

    template<class T>
    size_t vector<T>::capacity() const {
        return _capacity;   
    };

    template<class T>
    size_t vector<T>::size() const {
        return _size;   
    };

    template<class T>
    void vector<T>::clear() {
        _size = 0;
        _capacity = 0;
        buffer = NULL;
    }
}















namespace std {
    template<class T> 
    class vector_debug {
        size_t _size;
        size_t _capacity;
        T* buffer;

    public:
        typedef T* iterator; // used as vector::iterator

        // Ways to initialise this list
        vector_debug();
       // vector(size_t size);
        vector_debug(const vector_debug<T>& vec);
        ~vector_debug();

        void clear();

        void resize(size_t size);
        void reserve(size_t capacity);

        void push_back(const T& value);
        void pop_back();

        size_t capacity() const;
        size_t size() const;

        T& operator[](size_t index);
        vector_debug<T>& operator=(const vector_debug<T> & v);

    };
}


namespace std {

    template<class T>
    vector_debug<T>::vector_debug() :
        _capacity(0),
        _size(0),
        buffer(NULL)
    { }

    template<class T>
    vector_debug<T>::vector_debug(const vector_debug<T>& vec) :
        _capacity(vec._capacity),
        _size(vec._size),
        buffer(NULL) 
    { 
        for(size_t i = 0; i < _size; i++) {
            buffer[i] = vec.buffer[i];
        }
    }

	template<class T>
	vector_debug<T>::~vector_debug() { }

    template<class T> 
    void vector_debug<T>::push_back(const T& value) {
        // causes a magic breakpoint in Bochs
        //__asm__ __volatile__ ("xchg %bx, %bx"); 

        //BochsBreak();
        if(_size >= _capacity) {
            reserve(_capacity*1.5); // 1.5 is a bit less than the golden ratio, perfect for our memory allocation uses
        }

        buffer[_size] = value;
        _size++;

        char printbuff[80] = {0};
        sprintf(printbuff, "\n[end push_back] _size:%d,_capacity:%d\n", _size, _capacity);
        BochsConsolePrint(printbuff);

        __asm__ __volatile__ ("xchg %bx, %bx"); 
    }
    
    template<class T>
    void vector_debug<T>::pop_back() {
        (reinterpret_cast<T*>(buffer)[_size-1]).~T();
        --_size;
    }

    template<class T>
    void vector_debug<T>::reserve(size_t capacity) {
        _capacity = capacity + 1;

        T* buffer_new = new T[_capacity];

        char printbuff[90];
        sprintf(printbuff, "reserve():_size:%d,_capacity:%d\n", _size, _capacity);
        BochsConsolePrint(printbuff);

        for(size_t i = 0; i < _size; i++) {
            buffer_new[i] = buffer[i];
        }
        //delete[] buffer;
        buffer = buffer_new;

        sprintf(printbuff, "end reserve(%d,%d)\n", _size, _capacity);
        BochsConsolePrint(printbuff);
    }


	template<class T>
	vector_debug<T>& vector_debug<T>::operator= (const vector_debug<T> & v) {
		delete[] buffer;
		_size = v._size;

		_capacity = v._capacity;
		buffer = new T [_capacity];
		for (unsigned int i = 0; i < _size; i++)
			buffer[i] = v.buffer[i];
		return *this;
	}

	template<class T>
	T& vector_debug<T>::operator[](size_t index) {
		return buffer[index];
	}

    template<class T>
    size_t vector_debug<T>::capacity() const {
        return _capacity;   
    };

    template<class T>
    size_t vector_debug<T>::size() const {
        return _size;   
    };

    template<class T>
    void vector_debug<T>::clear() {
        _size = 0;
        _capacity = 0;
        buffer = NULL;
    }
}