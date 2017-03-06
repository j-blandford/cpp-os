#include <stdbool.h> 
#include <stddef.h> 

#define CHAR_BIT 8

namespace std {
    template<size_t n>
    class bitset {
        
        
    public:
        int bitArray[n];
        static const int numBits = CHAR_BIT * sizeof(int);

        bitset();
        bitset(unsigned long initialSet);

        void set(int bit);
        void reset(int bit);
        void flip(int bit);
        bool test(int bit) const;

        int operator[](int bit) const;
    };
}

namespace std {
    template<size_t n>
    bitset<n>::bitset() : bitArray() { }

    template<size_t n>
    bitset<n>::bitset(unsigned long initialSet) : bitArray() {
        for(unsigned int i = 0; i < n; i++) {
            bool isSet = ((initialSet & (1U << i)) != 0);
            if(isSet) {
                this->set(i);
            }
        }
    }

    template<size_t n>
    void bitset<n>::set(int bit) { 
        bitArray[bit/numBits] |= 1U << (bit % numBits);
    }

    template<size_t n>
    void bitset<n>::reset(int bit) { 
        bitArray[bit/numBits] &= ~(1U << (bit % numBits));
    }

    template<size_t n>
    void bitset<n>::flip(int bit) { 
        bitArray[bit/numBits] ^= 1U << (bit % numBits);
    }

    template<size_t n>
    bool bitset<n>::test(int bit) const { 
        return (bitArray[bit/numBits] & (1U << (bit % numBits))) != 0;
    }

    template<size_t n>
    int bitset<n>::operator[](int bit) const {
        return bitArray[bit/numBits] << (bit % numBits);
    }

}