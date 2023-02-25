#ifndef _LINUX_STRING_H
#define _LINUX_STRING_H

#include <linux/types.h>

/* 将dst_起始的size个字节置为value */
extern void memset(void* dst_, uint8_t value, uint32_t size);
/* 将src_起始的size个字节复制到dst_ */
extern void memcpy(void* dst_,  const void* src_, unsigned int size);

char *strcpy(char *dest, const char *src);

#endif /* _LINUX_STRING_H */
