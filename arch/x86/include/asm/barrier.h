#ifndef _ASM_BARRIER_H
#define _ASM_BARRIER_H

#include <asm/alternative.h>

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

#endif /* _ASM_BARRIER_H */
