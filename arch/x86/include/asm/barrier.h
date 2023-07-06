#ifndef _ASM_BARRIER_H
#define _ASM_BARRIER_H

#include <asm/alternative.h>
#include <asm/asm.h>
#include <asm/system.h>

/*
 * 强制严格的CPU顺序。
 * 当我们与设备通信时，即使在单处理器系统上也可能需要这样做。
 */

#ifdef CONFIG_X86_32
#define mb() asm volatile(ALTERNATIVE("lock; addl $0,-4(%%esp)", "mfence", \
				      X86_FEATURE_XMM2) ::: "memory", "cc")
#define rmb() asm volatile(ALTERNATIVE("lock; addl $0,-4(%%esp)", "lfence", \
				       X86_FEATURE_XMM2) ::: "memory", "cc")
#define wmb() asm volatile(ALTERNATIVE("lock; addl $0,-4(%%esp)", "sfence", \
				       X86_FEATURE_XMM2) ::: "memory", "cc")
#else /* CONFIG_X86_32 */
/**
 * 全屏障:
 * 同时刷新store buffer和invalidata queue，
 * 保证mfence前后的读写操作顺序，同时mfence之后写操作结果全局可见之前，mfence之前的写操作结果全局可见
 */
#define __mb()	asm volatile("mfence":::"memory")
/**
 * 读屏障(加载屏障):
 * 会将invalidata queue失效，强制读取入L1缓存，且lfence之后的读操作不会被调度到之前，
 * lfence之前的读操作在lfense完成，未规定全局可见
 */
#define __rmb()	asm volatile("lfence":::"memory")
/**
 * 写屏障(存储屏障):
 * 会将store buffer中缓存的修改刷入L1缓存，使得其他CPU核可以观察到这些修改，且之后的写操作不会被调度到之前，
 * sfence之前的写操作在sfense完成并全局可见。
 */
#define __wmb()	asm volatile("sfence" ::: "memory")
#endif /* !CONFIG_X86_32 */

/**
 * array_index_mask_nospec() - 当检查到访问越界时返回~0，未越界返回0
 * @index: 数组元素索引
 * @size: 数组元素个数
 *
 * Returns:
 *     0 - (index < size)
 */
static inline unsigned long array_index_mask_nospec(unsigned long index,
		unsigned long size)
{
	unsigned long mask;

	/**
	 * sbb指令：dest - src - CF标识位
	*/
	asm volatile ("cmp %1,%2; sbb %0,%0;"
			:"=r" (mask)
			:"g"(size),"r" (index)
			:"cc");
	return mask;
}

/* Override the default implementation from linux/nospec.h. */
#define array_index_mask_nospec array_index_mask_nospec

/* Prevent speculative execution past this barrier. */
#define barrier_nospec() alternative("", "lfence", X86_FEATURE_LFENCE_RDTSC)

#define __dma_rmb()	barrier()
#define __dma_wmb()	barrier()

#define __smp_mb()	asm volatile("lock; addl $0,-4(%%" _ASM_SP ")" ::: "memory", "cc")

#define __smp_rmb()	dma_rmb()
#define __smp_wmb()	barrier()
#define __smp_store_mb(var, value) do { (void)xchg(&var, value); } while (0)

#define __smp_store_release(p, v)					\
do {									\
	compiletime_assert_atomic_type(*p);				\
	barrier();							\
	WRITE_ONCE(*p, v);						\
} while (0)

#define __smp_load_acquire(p)						\
({									\
	typeof(*p) ___p1 = READ_ONCE(*p);				\
	compiletime_assert_atomic_type(*p);				\
	barrier();							\
	___p1;								\
})

/* Atomic operations are already serializing on x86 */
#define __smp_mb__before_atomic()	do { } while (0)
#define __smp_mb__after_atomic()	do { } while (0)

// #include <asm-generic/barrier.h>

/*
 * Make previous memory operations globally visible before
 * a WRMSR.
 *
 * MFENCE makes writes visible, but only affects load/store
 * instructions.  WRMSR is unfortunately not a load/store
 * instruction and is unaffected by MFENCE.  The LFENCE ensures
 * that the WRMSR is not reordered.
 *
 * Most WRMSRs are full serializing instructions themselves and
 * do not require this barrier.  This is only required for the
 * IA32_TSC_DEADLINE and X2APIC MSRs.
 */
static inline void weak_wrmsr_fence(void)
{
	asm volatile("mfence; lfence" : : : "memory");
}

#endif /* _ASM_BARRIER_H */
