#pragma once

#include <stddef.h>
#include <std.h>

// TODO: Fix string class

int strlen( char * ptr );
void strncpy( char * ptr_dest, char * ptr_src, int n );
char tolower(char upper);

class string {
	int length;               // length of the string
	char * buff;              // pointer to strorage
public:
	// Constructors
	string() : length(0), buff(NULL) { }
	string(char* init_val) : length(strlen(init_val)), buff(new char[length]) { 
		strncpy( buff, init_val, length );  // copy init value into storage
	}
	string(char init_val) : length(1), buff(new char[1]) {
		buff[0] = init_val;
	}
	string(const string& other) : length(other.length), buff(new char[length]) {
		strncpy( buff, other.buff, length );
	}
	~string() { 
		delete [] buff; 
	}

	// Member methods (need to provide more to meet the STL standards...)
	int size() { return length; }
	char* c_str() { return buff; }

	// Operator overloads
	string& operator= ( const string& other ) {
		if( this != &other ){          // guard against  a = a;  
			length = other.length;       // allocate new memory 
			buff = new char[length];        
			strncpy( buff, other.buff, length );
		}
		return *this;
	}

	char& operator[] (int index ) {
		if( index < 0 || index > length ) {
		//	return '\0';
		}
		return buff[index];
	}

	friend string operator+( const string& s1, const string& s2 );
	friend string operator+( const string& s, char c );
	friend string operator+( char c, const string& s );

	string& operator+=( const string& s2 ) {
		strncpy( this->buff + this->length, s2.buff, s2.length);

		this->length += s2.length;
	
		return *this;
	}

	string& operator+=( const char& s2 ) {
		char* newBuff = new char[this->length+1];
		strncpy( newBuff, buff, this->length);
		
		newBuff[this->length] = s2;

		this->buff = newBuff;
		this->length += 2;
	
		return *this;
	}

	operator char*() const { 
		char * nullBuffer = new char[this->length+1];

		strncpy( nullBuffer, buff, this->length);
		nullBuffer[this->length] = '\0';

		return nullBuffer; 
	};
};