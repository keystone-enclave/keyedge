#include "eapp_utils.h"
#include "edge_call.h"
#include <syscall.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

int
strcmp(const char *s1, const char *s2)
{
        while (*s1 == *s2++)
                if (*s1++ == 0)
                        return (0);
        return (*(const unsigned char *)s1 - *(const unsigned char *)(s2 - 1));
}

int
strncmp(const char *s1, const char *s2, unsigned long long n)
{

        if (n == 0)
                return (0);
        do {
                if (*s1 != *s2++)
                        return (*(const unsigned char *)s1 -
                                *(const unsigned char *)(s2 - 1));
                if (*s1++ == 0)
                        break;
        } while (--n != 0);
        return (0);
}

void
__assert_fail(const char *assertion,
              const char *file,
              unsigned int line,
              const char *function)
{
	EAPP_RETURN(1);
}

void
*memchr(const void *s, int c, size_t n)
{
    unsigned char *p = (unsigned char*)s;
    while( n-- )
        if( *p != (unsigned char)c )
            p++;
        else
            return p;
    return 0;
}

int posix_memalign(void **res, size_t align, size_t len)
{
	unsigned char *mem, *new, *end;
	size_t header, footer;

	if ((align & -align) != align) return 1;
	if (len > SIZE_MAX - align) return 1;

	if (align <= 4*sizeof(size_t)) {
		if (!(mem = malloc(len)))
			return 1;
		*res = mem;
		return 0;
	}

	if (!(mem = malloc(len + align-1)))
		return 1;

	header = ((size_t *)mem)[-1];
	end = mem + (header & -8);
	footer = ((size_t *)end)[-2];
	new = (void *)(((uintptr_t)mem + (align-1)) & -align);

	if (!(header & 7)) {
		((size_t *)new)[-2] = ((size_t *)mem)[-2] + (new-mem);
		((size_t *)new)[-1] = ((size_t *)mem)[-1] - (new-mem);
		*res = new;
		return 0;
	}

	((size_t *)mem)[-1] = (header&7) | (new-mem);
	((size_t *)new)[-2] = (footer&7) | (new-mem);
	((size_t *)new)[-1] = (header&7) | (end-new);
	((size_t *)end)[-2] = (footer&7) | (end-new);

	if (new != mem) free(mem);
	*res = new;
	return 0;
}

