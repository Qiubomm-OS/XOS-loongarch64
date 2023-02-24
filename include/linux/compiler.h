#ifndef _LINUX_COMPILER_H
#define _LINUX_COMPILER_H

#include <linux/compiler-intel.h>
#include <linux/types.h>
#include <linux/string.h>

#ifndef __always_inline
# define __always_inline	inline __attribute__((always_inline))
#endif

typedef __u8  __attribute__((__may_alias__))  __u8_alias_t;
typedef __u16 __attribute__((__may_alias__)) __u16_alias_t;
typedef __u32 __attribute__((__may_alias__)) __u32_alias_t;
typedef __u64 __attribute__((__may_alias__)) __u64_alias_t;

#endif /* _LINUX_VOMPILER_H */
