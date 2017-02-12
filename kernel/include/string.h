#pragma once

#include <stddef.h>
#include <libcxx.h>


// Helper functions:
//
// my_strlen : count the length of a string until a null char
//
// my_strncpy : copy n chars from source to destination

int strlen( char * ptr );

void strncpy( char * ptr_dest, char * ptr_src, int n );

class string {
  int length;               // length of the string
  char * buff;              // pointer to strorage
public:

  // Constructors: default, from a string literal (char*), copy constr.
  // When possible, constructors should use initialization lists rather 
  // than assignments. Thus:
  //        buff( new char[length] )   in the init list is better that
  //        buff = new char[length]    in the body of the constructor,
  //   although they do the same thing -- allocate dyn. memory

  string() : length(0), buff(NULL) 
    {
    }

  string( char* init_val ) : 
    length( strlen(init_val) ),    // count the length of init value
    buff(   new char[length] )     // allocate storage 
    { 
      strncpy( buff, init_val, length );  // copy init value into storage
    }

  string( const string& other ) :
    length( other.length ),        
    buff(   new char[length] ) 
    {
      strncpy( buff, other.buff, length );
    }

  ~string() 
    { 
      delete [] buff; 
    }

  int size() { return length; }
  
  // operator= returns  string&  to allow multiple assignments

  string& operator= ( const string& other )
    {
      if( this != &other ){          // guard against  a = a;  
        delete [] buff;              // release old memory & then
        length = other.length;       // allocate new memory 
        buff = new char[length];        
        strncpy( buff, other.buff, length );
      }
      return *this;                  // return a reference to itself
    }                                // to allow a = b = c; 
    
  // concatenation of strings; we should also handle chars.
  // notice that + is overloaded thrice: 
  //    string + string,  string + char, char + string 
  // making operator+ a _friend_ allows for all three types; while
  // the first two can be done with a _member_ as well, the last cannot!

  friend string operator+( const string& s1, const string& s2 );
  friend string operator+( const string& s, char c );
  friend string operator+( char c, const string& s );

  string& operator+=( const string& s2 )
  {
    strncpy( this->buff + this->length, s2.buff, s2.length);

    this->length += s2.length;
  
    return *this;
  }


  operator char*() const { return this->buff; };
  
//  friend ostream& operator<< ( ostream&, const string& );

  // finally, we want to reveal characters of a string for lookup
  // and assignment (hence we return a _reference_ to a char!)

  char& operator[] (int index )
    {
      if( index < 0 || index > length ) {
        //return '\0';
      }
      return buff[index];
    }
};


// ostream& operator<< ( ostream& os, const string& s )
// {
//   // print char after char from buff
//   for( int i=0; i < s.length; i++ )  os.put( s.buff[i] );
//   return os;         // this is to allow multiple <<, as in  cout << a << b;
// }