#include <limits.h>
#include <libcxx.h>
#include <stdarg.h>

/* Return the `ldiv_t' representation of NUMER over DENOM.  */
ldiv_t ldiv (unsigned long int numer, unsigned long int denom)
{
    ldiv_t result;

    result.quot = numer / denom;
    result.rem = numer % denom;

  /* The ANSI standard says that |QUOT| <= |NUMER / DENOM|, where
     NUMER / DENOM is to be computed in infinite precision.  In
     other words, we should always truncate the quotient towards
     zero, never -infinity.  Machine division and remainer may
     work either way when one or both of NUMER or DENOM is
     negative.  If only one is negative and QUOT has been
     truncated towards -infinity, REM will have the same sign as
     DENOM and the opposite sign of NUMER; if both are negative
     and QUOT has been truncated towards -infinity, REM will be
     positive (will have the opposite sign of NUMER).  These are
     considered `wrong'.  If both are NUM and DENOM are positive,
     RESULT will always be positive.  This all boils down to: if
     NUMER >= 0, but REM < 0, we got the wrong answer.  In that
     case, to get the right answer, add 1 to QUOT and subtract
     DENOM from REM.  */

    if (numer >= 0 && result.rem < 0)
    {
        ++result.quot;
        result.rem -= denom;
    }

    return result;
}


#define BUFSIZE (sizeof(unsigned long long) * 8 + 1)

char *ltoa(unsigned long long N, char *str, int base)
{
      register int i = 2;
      unsigned long long uarg;
      char *tail, *head = str, buf[BUFSIZE] = {0};

      if (36 < base || 2 > base)
            base = 10;                    /* can only use 0-9, A-Z        */
      tail = &buf[BUFSIZE - 1];           /* last character position      */
      *tail-- = '\0';

      if (10 == base && N < 0L)
      {
            *head++ = '-';
            uarg    = -N;
      }
      else  uarg = N;

      if (uarg)
      {
            for (i = 1; uarg; ++i)
            {
                  register ldiv_t r;

                  r       = ldiv(uarg, base);
                  *tail-- = (char)(r.rem + ((9L < r.rem) ?
                                  ('A' - 10L) : '0'));
                  uarg    = r.quot;
            }
      }
      else  *tail-- = '0';

      memcpy(head, ++tail, i);
      return str;
}

char* itoa (int value, char* result, int base) {

		// check that the base if valid
		if (base < 2 || base > 36) { *result = '\0'; return result; }

		char* ptr = result, *ptr1 = result, tmp_char;
		int tmp_value;

		do {
			tmp_value = value;
			value /= base;
			*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
		} while ( value );

		// Apply negative sign
		if (tmp_value < 0) *ptr++ = '-';
		*ptr-- = '\0';
		while(ptr1 < ptr) {
			tmp_char = *ptr;
			*ptr--= *ptr1;
			*ptr1++ = tmp_char;
		}
		return result;

}

int putchar(int ic) {
	char c = (char) ic;
	//terminal_write(&c, sizeof(c));
	return ic;
}



static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == -1)
			return false;
	return true;
}

int printf(const char* format, ...) {
	va_list parameters;
	va_start(parameters, format);

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 'x') {
			format++;

			char buffer[8];

			ltoa(va_arg(parameters, long), buffer, 16);

			char * hex = "0x";
			strcat(hex, buffer);

			if (!print(hex, sizeof(hex)))
				return -1;
			written++;
		} else if (*format == 'd' || *format == 'i') {
			format++;

			char buffer[8];
			itoa((int) va_arg(parameters, int), buffer, 10);

			char *c = (char*)buffer;

			if (!print((char*)c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);

			if (!print(str, len))
				return -1;
			written += len;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);

			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}

int sprintf(char* buffer, const char* format, ...) {
	va_list parameters;
	va_start(parameters, format);

	// set the buffer to zero before any writing
	int i = 0;
	while(buffer[i] != 0) { buffer[i] = 0; i++; }

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		buffer[written] = format[0];

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%') {
				buffer[written+amount] = format[amount];
				amount++;
			}
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}

			written++;
		} 
		else if (*format == 'x') {
			format++;

			char fmt_buffer[16] = {0};

			ltoa(va_arg(parameters, unsigned long long), fmt_buffer, 16);

			buffer[written] = '0';
			buffer[written+1] = 'x';

			size_t amount = 0;
			while (fmt_buffer[amount] != '\0') {
				buffer[written+amount+2] = fmt_buffer[amount];
				amount++;
			}

			written+=amount+2;
		} 
		else if (*format == 'd' || *format == 'i') {
			format++;

			char fmt_buffer[8];
			itoa((int) va_arg(parameters, int), fmt_buffer, 10);

			size_t amount = 0;
			while (fmt_buffer[amount] != '\0') {
				buffer[written+amount] = fmt_buffer[amount];
				amount++;
			}

			written+=amount;
			
		} 
		else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);

			written += len;
		} 
		else {
			format = format_begun_at;
			size_t len = strlen(format);

			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}
