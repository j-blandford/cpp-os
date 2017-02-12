#include <string.h>

int strlen( char * ptr ){    
  int len = 0;
  char * p = ptr;
  while( *p != '\0' ) {
    len++;
    p++;
  }
  return len;
}

void strncpy( char * ptr_dest, char * ptr_src, int n ){
  for( int i=0; i < n; i++ )
    ptr_dest[i] = ptr_src[i];
}

string operator+( const string& s1, const string& s2 )
{
  string res;
  res.length = s1.length + s2.length;
  res.buff = new char[ res.length ];
  strncpy( res.buff, s1.buff, s1.length );
  strncpy( res.buff + s1.length,            // pointer arithmetic! 
	      s2.buff, 
	      s2.length );
  return res;
}