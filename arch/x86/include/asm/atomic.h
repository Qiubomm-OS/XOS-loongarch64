#ifndef _ASM_ATOMIC_H
#define _ASM_ATOMIC_H

#include <linux/compiler.h>
#include <asm-generic/rwonce.h>
#include <asm/alternative.h>

/*
 * Atomic operations that C can't guarantee us.  Useful for
 * resource counting etc..
 */

/**
 * arch_atomic_read - read atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically reads the value of @v.
 */
static __always_inline int arch_atomic_read(const atomic_t *v)
{
	/*
	 * Note for KASAN: we deliberately don't use READ_ONCE_NOCHECK() here,
	 * it's non-inlined function that increases binary size and stack usage.
	 */
	return __READ_ONCE((v)->counter);
}

/**
 * arch_atomic_set - set atomic variable
 * @v: pointer of type atomic_t
 * @i: required value
 *
 * Atomically sets the value of @v to @i.
 */
static __always_inline void arch_atomic_set(atomic_t *v, int i)
{
	__WRITE_ONCE(v->counter, i);
}

/**
 * arch_atomic_add - add integer to atomic variable
 * @i: integer value to add
 * @v: pointer of type atomic_t
 *
 * Atomically adds @i to @v.
 */
static __always_inline void arch_atomic_add(int i, atomic_t *v)
{
	asm volatile(LOCK_PREFIX "addl %1,%0"
		     : "+m" (v->counter)
		     : "ir" (i) : "memory");
}

/**
 * arch_atomic_sub - subtract integer from atomic variable
 * @i: integer value to subtract
 * @v: pointer of type atomic_t
 *
 * Atomically subtracts @i from @v.
 */
static __always_inline void arch_atomic_sub(int i, atomic_t *v)
{
	asm volatile(LOCK_PREFIX "subl %1,%0"
		     : "+m" (v->counter)
		     : "ir" (i) : "memory");
}

/**
 * arch_atomic_inc - increment atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically increments @v by 1.
 */
static __always_inline void arch_atomic_inc(atomic_t *v)
{
	asm volatile(LOCK_PREFIX "incl %0"
		     : "+m" (v->counter) :: "memory");
}
#define arch_atomic_inc arch_atomic_inc

/**
 * arch_atomic_dec - decrement atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically decrements @v by 1.
 */
static __always_inline void arch_atomic_dec(atomic_t *v)
{
	asm volatile(LOCK_PREFIX "decl %0"
		     : "+m" (v->counter) :: "memory");
}
#define arch_atomic_dec arch_atomic_dec

static __always_inline void arch_atomic_and(int i, atomic_t *v)
{
	asm volatile(LOCK_PREFIX "andl %1,%0"
			: "+m" (v->counter)
			: "ir" (i)
			: "memory");
}

static __always_inline void arch_atomic_or(int i, atomic_t *v)
{
	asm volatile(LOCK_PREFIX "orl %1,%0"
			: "+m" (v->counter)
			: "ir" (i)
			: "memory");
}

static __always_inline void arch_atomic_xor(int i, atomic_t *v)
{
	asm volatile(LOCK_PREFIX "xorl %1,%0"
			: "+m" (v->counter)
			: "ir" (i)
			: "memory");
}

#endif /* _ASM_ATOMIC_H */
