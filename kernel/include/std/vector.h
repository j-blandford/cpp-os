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
        vector(size_t size);
        vector(const vector<T>& vec);
        ~vector();

        T& operator[](size_t index);

        T& at(size_t index);

        void clear();

        void resize(size_t size);
        void reserve(size_t capacity);

        void push_back(const T& value);
        void pop_back();

        size_t capacity() const;
        size_t size() const;

    };
}