#ifndef _LINUX_CTYPE_H
#define _LINUX_CTYPE_H

static inline int isdigit(int c)
{
	return '0' <= c && c <= '9';
}

#endif /* _LINUX_CTYPE_H */
