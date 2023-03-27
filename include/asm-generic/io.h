#ifndef _ASM_GENERIC_IO_H
#define _ASM_GENERIC_IO_H

#include <linux/types.h>
#include <asm/barrier.h>

#ifndef __io_br
#define __io_br()      barrier()
#endif

/* prevent prefetching of coherent DMA data ahead of a dma-complete */
#ifndef __io_ar
#ifdef rmb
#define __io_ar(v)      rmb()
#else
#define __io_ar(v)      barrier()
#endif
#endif

/* flush writes to coherent DMA data before possibly triggering a DMA read */
#ifndef __io_bw
#ifdef wmb
#define __io_bw()      wmb()
#else
#define __io_bw()      barrier()
#endif
#endif

static inline void log_write_mmio(u64 val, u8 width, volatile void *addr,
				  unsigned long caller_addr, unsigned long caller_addr0) {}
static inline void log_post_write_mmio(u64 val, u8 width, volatile void *addr,
				       unsigned long caller_addr, unsigned long caller_addr0) {}
static inline void log_read_mmio(u8 width, const volatile void *addr,
				 unsigned long caller_addr, unsigned long caller_addr0) {}
static inline void log_post_read_mmio(u64 val, u8 width, const volatile void *addr,
				      unsigned long caller_addr, unsigned long caller_addr0) {}

static inline u8 __raw_readb(const volatile void *addr)
{
	return *(const volatile u8 *)addr;
}

static inline u16 __raw_readw(const volatile void *addr)
{
	return *(const volatile u16 *)addr;
}

static inline u32 __raw_readl(const volatile void *addr)
{
	return *(const volatile u32 *)addr;
}

static inline u64 __raw_readq(const volatile void *addr)
{
	return *(const volatile u64 *)addr;
}

static inline void __raw_writeb(u8 value, volatile void *addr)
{
	*(volatile u8 *)addr = value;
}

static inline void __raw_writew(u16 value, volatile void *addr)
{
	*(volatile u16 *)addr = value;
}

static inline void __raw_writel(u32 value, volatile void *addr)
{
	*(volatile u32 *)addr = value;
}

static inline void __raw_writeq(u64 value, volatile void *addr)
{
	*(volatile u64 *)addr = value;
}

#endif /* _ASM_GENERIC_IO_H */
