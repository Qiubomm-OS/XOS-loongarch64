#ifndef _ASM_ATOMIC_H
#define _ASM_ATOMIC_H

#include <linux/types.h>

#if __SIZEOF_LONG__ == 4
#define __LL		"ll.w	"
#define __SC		"sc.w	"
#define __AMADD		"amadd.w	"
#define __AMAND_DB	"amand_db.w	"
#define __AMOR_DB	"amor_db.w	"
#define __AMXOR_DB	"amxor_db.w	"
#elif __SIZEOF_LONG__ == 8
#define __LL		"ll.d	"
#define __SC		"sc.d	"
#define __AMADD		"amadd.d	"
#define __AMAND_DB	"amand_db.d	"
#define __AMOR_DB	"amor_db.d	"
#define __AMXOR_DB	"amxor_db.d	"
#endif

#define ATOMIC_INIT(i)	  { (i) }

/*
 * arch_atomic_read - read atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically reads the value of @v.
 */
#define arch_atomic_read(v)	READ_ONCE((v)->counter)

/*
 * arch_atomic_set - set atomic variable
 * @v: pointer of type atomic_t
 * @i: required value
 *
 * Atomically sets the value of @v to @i.
 */
#define arch_atomic_set(v, i)	WRITE_ONCE((v)->counter, (i))

#define ATOMIC_OP(op, I, asm_op)					\
static inline void arch_atomic_##op(int i, atomic_t *v)			\
{									\
	__asm__ __volatile__(						\
	"am"#asm_op"_db.w" " $zero, %1, %0	\n"			\
	: "+ZB" (v->counter)						\
	: "r" (I)							\
	: "memory");							\
}

#define ATOMIC_OP_RETURN(op, I, asm_op, c_op)				\
static inline int arch_atomic_##op##_return_relaxed(int i, atomic_t *v)	\
{									\
	int result;							\
									\
	__asm__ __volatile__(						\
	"am"#asm_op"_db.w" " %1, %2, %0		\n"			\
	: "+ZB" (v->counter), "=&r" (result)				\
	: "r" (I)							\
	: "memory");							\
									\
	return result c_op I;						\
}

#define ATOMIC_FETCH_OP(op, I, asm_op)					\
static inline int arch_atomic_fetch_##op##_relaxed(int i, atomic_t *v)	\
{									\
	int result;							\
									\
	__asm__ __volatile__(						\
	"am"#asm_op"_db.w" " %1, %2, %0		\n"			\
	: "+ZB" (v->counter), "=&r" (result)				\
	: "r" (I)							\
	: "memory");							\
									\
	return result;							\
}

#define ATOMIC_OPS(op, I, asm_op, c_op)					\
	ATOMIC_OP(op, I, asm_op)					\
	ATOMIC_OP_RETURN(op, I, asm_op, c_op)				\
	ATOMIC_FETCH_OP(op, I, asm_op)

ATOMIC_OPS(add, i, add, +)
ATOMIC_OPS(sub, -i, add, +)



#endif /* _ASM_ATOMIC_H */
