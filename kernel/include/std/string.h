#pragma once

#include <stddef.h>
#include <std.h>

// TODO: Fix string class

int strlen( char * ptr );
int strcmp(const char* s1, const char* s2);

void strncpy( char * ptr_dest, char * ptr_src, int n );
char tolower(char upper);

char * strpbrk(const char *s, const char *accept);
size_t strspn (const char *s, const char *accept);
char * strtok(char *str, const char *delim);

class string {
	int length;               // length of the string
	char * buff;              // pointer to strorage
public:
	// Constructors
	string() : length(0), buff(NULL) { }
	string(char* init_val) : length(strlen(init_val)), buff(new char[length+1]) { 
//		terminal_printf("INIT string(char*): %s @ %x", init_val, &init_val);
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

	bool operator== (const string& rhs) {
		return strcmp(buff, rhs.buff) == 0;
	}
	bool operator== (const char* rhs) {
		return strcmp(buff, rhs) == 0;
	}

	string& operator+=( const string& s2 ) {
		char* newBuff = new char[this->length+s2.length];

		strncpy( newBuff, this->buff, this->length);
		strncpy( newBuff + this->length, s2.buff, s2.length);

		this->buff = newBuff;
		this->length += s2.length;

		return *this;
	}

	string& operator+=( const char& s2 ) {
		char* newBuff = new char[this->length+1];
		strncpy( newBuff, buff, this->length);
		
		newBuff[this->length] = s2;

		this->buff = newBuff;
		this->length += 1;
	
		return *this;
	}

	// string& operator+=( const char* s2 ) {
	// 	char* newBuff = new char[this->length+strlen((char*)s2)+1];
	// 	strncpy( newBuff, buff, this->length);
	// 	strncpy( *(&newBuff+this->length), (char*)s2, strlen((char*)s2));

	// 	this->buff = newBuff;
	// 	this->length = strlen(this->buff);
	
	// 	return *this;
	// }

	operator char*() const { 
		char * nullBuffer = new char[this->length+1];

		strncpy( nullBuffer, buff, this->length);
		nullBuffer[this->length] = '\0';

		return nullBuffer; 
	};
};