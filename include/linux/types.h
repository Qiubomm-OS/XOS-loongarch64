#ifndef _LINUX_TYPES_H
#define _LINUX_TYPES_H

typedef enum {
    FALSE = 0,
    TRUE = 1
} bool;

typedef unsigned char		u8;
typedef u8			__u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long long	u64;
typedef u64			__u64;

typedef signed short		s16;

typedef unsigned long		size_t;
typedef u64			phys_addr_t;

#define __aligned_u64 __u64 __attribute__((aligned(8)))

typedef struct {
	__u8 b[16];
} guid_t;

#endif /* _LINUX_TYPES_ */
