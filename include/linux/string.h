#ifndef _LINUX_STRING_H
#define _LINUX_STRING_H

#include <linux/types.h>

/* 将src_起始的size个字节复制到dst_ */
// extern void memcpy(void* dst_,  void* src_, unsigned int size);

char *strcpy(char *dest, const char *src);

#endif /* _LINUX_STRING_H */
