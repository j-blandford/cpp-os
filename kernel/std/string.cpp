#include <std.h>
#include <stddef.h>

int strlen( char * ptr ){    
  int len = 0;
  char * p = ptr;
  while( *p != '\0' ) {
    len++;
    p++;
  }
  return len;
}


int strncmp(const char *s1, const char *s2, size_t n) {
    for ( ; n > 0; s1++, s2++, --n)
        if (*s1 != *s2)
            return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : +1);
        else if (*s1 == '\0')
            return 0;
    return 0;
}

char *strcat(char *dest, const char *src) {
    char *rdest = dest;

    while (*dest)
      dest++;
    while (*dest++ = *src++)
      ;
    return rdest;
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