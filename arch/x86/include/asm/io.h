#ifndef _ASM_X86_IO_H
#define _ASM_X86_IO_H

#include <linux/types.h>

extern void native_io_delay(void);

static inline void slow_down_io(void)
{
	native_io_delay();
#ifdef REALLY_SLOW_IO
	native_io_delay();
	native_io_delay();
	native_io_delay();
#endif
}

#define __BUILDIO(bwl, bw, type)				\
static inline void out##bwl(unsigned type value, int port)	\
{								\
	out##bwl##_local(value, port);				\
}								\
								\
static inline unsigned type in##bwl(int port)			\
{								\
	return in##bwl##_local(port);				\
}

#define BUILDIO(bwl, bw, type)						\
static inline void out##bwl##_local(unsigned type value, int port)	\
{									\
	asm volatile("out" #bwl " %" #bw "0, %w1"		\
		     : : "a"(value), "Nd"(port));			\
}									\
									\
static inline unsigned type in##bwl##_local(int port)			\
{									\
	unsigned type value;						\
	asm volatile("in" #bwl " %w1, %" #bw "0"		\
		     : "=a"(value) : "Nd"(port));			\
	return value;							\
}									\
									\
static inline void out##bwl##_local_p(unsigned type value, int port)	\
{									\
	out##bwl##_local(value, port);					\
	slow_down_io();							\
}									\
									\
static inline unsigned type in##bwl##_local_p(int port)			\
{									\
	unsigned type value = in##bwl##_local(port);			\
	slow_down_io();							\
	return value;							\
}									\
									\
__BUILDIO(bwl, bw, type)						\
									\
static inline void out##bwl##_p(unsigned type value, int port)		\
{									\
	out##bwl(value, port);						\
	slow_down_io();							\
}									\
									\
static inline unsigned type in##bwl##_p(int port)			\
{									\
	unsigned type value = in##bwl(port);				\
	slow_down_io();							\
	return value;							\
}									\
									\
static inline void outs##bwl(int port, const void *addr, unsigned long count) \
{									\
	asm volatile("rep; outs" #bwl					\
		     : "+S"(addr), "+c"(count) : "d"(port));		\
}									\
									\
static inline void ins##bwl(int port, void *addr, unsigned long count)	\
{									\
	asm volatile("rep; ins" #bwl					\
		     : "+D"(addr), "+c"(count) : "d"(port));		\
}

BUILDIO(b, b, char)
BUILDIO(w, w, short)
BUILDIO(l, , int)

#endif /* _ASM_X86_IO_H */
