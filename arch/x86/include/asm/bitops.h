#ifndef _I386_BITOPS_H
#define _I386_BITOPS_H

#define LOCK_PREFIX ""
#define ADDR (*(volatile long *) addr)

static __inline__ void set_bit(int nr, volatile void * addr)
{
    __asm__ __volatile__( LOCK_PREFIX
                          "btsl %1,%0"
    :"=m" (ADDR)
    :"Ir" (nr));
}

static __inline__ void clear_bit(int nr, volatile void * addr)
{
    __asm__ __volatile__( LOCK_PREFIX
                          "btrl %1,%0"
    :"=m" (ADDR)
    :"Ir" (nr));
}

/*
 * ffz = Find First Zero in word. Undefined if no zero exists,
 * so code should check against ~0UL first..
 */
static __inline__ unsigned long ffz(unsigned long word)
{
    __asm__("bsfl %1,%0"
    :"=r" (word)
    :"r" (~word));
    return word;
}

/**
 * test_and_clear_bit - Clear a bit and return its old value
 *                      在起始地址为addr的位图中清除第nr位，并返回原来的值，原子操作
 * @nr: Bit to clear
 * @addr: Address to count from
 *
 * This operation is atomic and cannot be reordered.
 * It also implies a memory barrier.
 */
// #define ADDR (*(volatile long *) addr)
static __inline__ int test_and_clear_bit(int nr, volatile void * addr)
{
    int oldbit;

    __asm__ __volatile__( LOCK_PREFIX
    "btrl %2,%1\n\tsbbl %0,%0"
    :"=r" (oldbit),"=m" (ADDR)
    :"Ir" (nr) : "memory");
    return oldbit;
}

/**
 * test_and_set_bit - Set a bit and return its old value
 * @nr: Bit to set
 * @addr: Address to count from
 *
 * This operation is atomic and cannot be reordered.
 * It also implies a memory barrier.
 */
static  int test_and_set_bit(int nr, volatile void * addr)
{
    int oldbit;

    __asm__ __volatile__( LOCK_PREFIX
                          "btsl %2,%1\n\tsbbl %0,%0"
    :"=r" (oldbit),"=m" (ADDR)
    :"Ir" (nr) : "memory");
    return oldbit;
}

static __inline__ int constant_test_bit(int nr, const volatile void * addr)
{
    return ((1UL << (nr & 31)) & (((const volatile unsigned int *) addr)[nr >> 5])) != 0;
}

static __inline__ int variable_test_bit(int nr, volatile void * addr)
{
    int oldbit;

    __asm__ __volatile__(
    "btl %2,%1\n\tsbbl %0,%0"
    :"=r" (oldbit)
    :"m" (ADDR),"Ir" (nr));
    return oldbit;
}

/* WARNING: non atomic and it can be reordered! */
static __inline__ int __test_and_change_bit(int nr, volatile void * addr)
{
    int oldbit;

    __asm__ __volatile__(
    "btcl %2,%1\n\tsbbl %0,%0"
    :"=r" (oldbit),"=m" (ADDR)
    :"Ir" (nr) : "memory");
    return oldbit;
}

static __inline__ void __change_bit(int nr, volatile void * addr)
{
    __asm__ __volatile__(
    "btcl %1,%0"
    :"=m" (ADDR)
    :"Ir" (nr));
}

#define test_bit(nr,addr) constant_test_bit((nr),(addr))

#endif /* _I386_BITOPS_H */
