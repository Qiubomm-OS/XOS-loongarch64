#ifndef _LINUX_TYPES_H
#define _LINUX_TYPES_H

#ifndef __ASSEMBLY__

#define asmlinkage __attribute__((regparm(0)))

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define NULL ((void*)0)
#define BITS_PER_LONG 64

#define true 1
#define false 0

typedef enum {
    FALSE = 0,
    TRUE = 1
} bool;

typedef unsigned char		u8;
typedef u8			        __u8;
typedef unsigned short		u16;
typedef u16                 __u16;
typedef unsigned int		u32;
typedef u32                 __u32;
typedef unsigned long long	u64;
typedef u64			        __u64;

typedef signed short		s16;
typedef signed long long    __s64;

typedef u8                  uint8_t;
typedef u16                 uint16_t;
typedef u32                 uint32_t;
typedef u64                 uint64_t;

typedef unsigned long		size_t;
typedef u64			        phys_addr_t;

#define __aligned_u64 __u64 __attribute__((aligned(8)))

typedef struct {
	__u8 b[16];
} guid_t;

#define INT_MAX			((int)(~0U>>1))
#define UINT32_MAX		((u32)~0U)
#define INT32_MAX		((s32)(UINT32_MAX >> 1))

typedef struct {
	int counter;
} atomic_t;

#define _ULCAST_ (unsigned long)
#define _U64CAST_ (u64)

#endif /* !__ASSEMBLY__ */

#endif /* _LINUX_TYPES_ */
