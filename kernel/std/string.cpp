#include <std/string.h>
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

int strcmp(const char* s1, const char* s2) {
	while(*s1 && (*s1==*s2))
		s1++,s2++;
	return *(const unsigned char*)s1-*(const unsigned char*)s2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
	for ( ; n > 0; s1++, s2++, --n)
			if (*s1 != *s2)
					return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : +1);
			else if (*s1 == '\0')
					return 0;
	return 0;
}

char tolower(char upper) {
	if (upper <= 'Z' && upper >= 'A')
		return upper - ('Z' - 'z');
	return upper;
}


char *strcat(char *dest, const char *src) {
	char *rdest = dest;

	while (*dest)
		dest++;
	while ((*dest++ = *src++))
		;
	return rdest;
}

// taken from GLibC 
char * strpbrk(const char *s, const char *accept) {
	while (*s != '\0') {
		const char *a = accept;

		while (*a != '\0')
			if (*a++ == *s)
				return (char *) s;
				++s;
	}
	return NULL;
}

// taken from GLibC 
size_t strspn (const char *s, const char *accept) {
	const char *p;
	const char *a;
	size_t count = 0;

	for (p = s; *p != '\0'; ++p){
		for (a = accept; *a != '\0'; ++a)
			if (*p == *a)
				break;
			if (*a == '\0')
				return count;
			else
				++count;
	}

	return count;
}

// thanks to David Refaeli for this code
char * strtok(char *str, const char *delim) {

	static char *nxt; /* static variable used to advance the string to replace delimiters */
	static int size;  /* static variable used to count until the end of the string        */

	/* IMPORTANT: any advance to 'nxt' must be followed by a diminution of 'size', and vice verce */

	int i; /* counter of delimiter(s) in string */

	/* initialize the string when strtok2 is first calles and supplied with a valid string */
	if(str != NULL)
	{
		nxt = str;
		size = strlen(str);
	}

	/* if we havn't reached the end of the string, any other call to strtok2 with NULL will come here */
	else if(size > 0)
	{
		nxt++;      /* last run left nxt on a null terminator, so we have to advance it */
		size--;     /* any advancement must follow diminution of size                   */
		str = nxt;  /* string now points to the first char after the last delimiter     */ 
	}

	/* if we reached the end of string, return NULL pointer */
	else
	{
		str = NULL;
	}

	/* nxt is used to advance the string until a delimiter or a series of delimiters are encountered; 
	* it then stops on the last delimiter which has turned to NULL terminator
	*/
	while(*nxt)
	{
		i = strspn(nxt, delim);
		while(i > 1)        /* turns delimiters to NULL terminator (except the last one) */
		{
			*nxt = '\0';
			nxt++;
			size--;
			i--;
		}                   /* in the last delimiter we have to do something a      */
		if(1 == i)          /* bit different we have to actually take nxt backwards */
		{                   /* one step, to break the while(*nxt) loop              */
			*nxt = '\0';
			if(size > 1)    /* if the delimiter is last char, don't do next lines   */
			{
				nxt--;
				size++;     /* nxt is diminished so size increases                    */
			}
		}
		nxt++;          /* if no delimiter is found, advance nxt                  */
		size--;         /* in case of the last delimiter in a series, we took nxt */
	}                   /* a step back, so now moving it a step forward means     */
						/* it rests on a NULL terminator                          */
	return str;
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