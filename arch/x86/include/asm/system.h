#ifndef _ASM_SYSTEM_H
#define _ASM_SYSTEM_H

/*
 * Force strict CPU ordering.
 * And yes, this is required on UP too when we're talking
 * to devices.
 *
 * For now, "wmb()" doesn't actually do anything, as all
 * Intel CPU's follow what Intel calls a *Processor Order*,
 * in which all writes are seen in the program order even
 * outside the CPU.
 *
 * I expect future Intel CPU's to have a weaker ordering,
 * but I'd also expect them to finally get their act together
 * and add some real memory barriers if so.
 *
 * The Pentium III does add a real memory barrier with the
 * sfence instruction, so we use that where appropriate.
 */
#ifndef CONFIG_X86_XMM
#define mb() 	__asm__ __volatile__ ("lock; addl $0,0(%%esp)": : :"memory")
#else
#define mb()	__asm__ __volatile__ ("sfence": : :"memory")
#endif

#define __cli() 		__asm__ __volatile__("cli": : :"memory")
#define __sti()			__asm__ __volatile__("sti": : :"memory")
#define cli() __cli()
#define sti() __sti()

/* interrupt control.. */
#define __save_flags(x)		__asm__ __volatile__("pushfl ; popl %0":"=g" (x): /* no input */)
#define __restore_flags(x) 	__asm__ __volatile__("pushl %0 ; popfl": /* no output */ :"g" (x):"memory", "cc")
#define __cli() 		__asm__ __volatile__("cli": : :"memory")
#define __sti()			__asm__ __volatile__("sti": : :"memory")
/* used in the idle loop; sti takes one instruction cycle to complete */
#define safe_halt()		__asm__ __volatile__("sti; hlt": : :"memory")

#define __save_and_cli(x)	do { __save_flags(x); __cli(); } while(0);
#define __save_and_sti(x)	do { __save_flags(x); __sti(); } while(0);
#define local_irq_save(x)	__save_and_cli(x)
#define local_irq_restore(x)	__restore_flags(x)
#define save_flags(x) __save_flags(x)
#define restore_flags(x) __restore_flags(x)

#define prepare_to_switch()	do { } while(0)
#define switch_to(prev,next,last) do {					\
	asm volatile("pushl %%esi\n\t"					\
		     "pushl %%edi\n\t"					\
		     "pushl %%ebp\n\t"					\
		     "movl %%esp,%0\n\t"	/* save ESP */		\
		     "movl %3,%%esp\n\t"	/* restore ESP */	\
		     "movl $1f,%1\n\t"		/* save EIP */		\
		     "pushl %4\n\t"		/* restore EIP */	\
		     "jmp __switch_to\n"				\
		     "1:\t"						\
		     "popl %%ebp\n\t"					\
		     "popl %%edi\n\t"					\
		     "popl %%esi\n\t"					\
		     :"=m" (prev->thread.esp),"=m" (prev->thread.eip),	\
		      "=b" (last)					\
		     :"m" (next->thread.esp),"m" (next->thread.eip),	\
		      "a" (prev), "d" (next));					\
} while (0)

/*
 * Load a segment. Fall back on loading the zero
 * segment if something goes wrong..
 */
#define loadsegment(seg,value)			\
	asm volatile("\n"			\
		"mov %0,%%" #seg "\n"		\
		: :"m" (*(unsigned int *)&(value)))

#define xchg(ptr,v) ((__typeof__(*(ptr)))__xchg((unsigned long)(v),(ptr),sizeof(*(ptr))))

struct __xchg_dummy { unsigned long a[100]; };
#define __xg(x) ((struct __xchg_dummy *)(x))

/*
 * Note: no "lock" prefix even on SMP: xchg always implies lock anyway
 * Note 2: xchg has side effect, so that attribute volatile is necessary,
 *	  but generally the primitive is invalid, *ptr is output argument. --ANK
 */
static inline unsigned long __xchg(unsigned long x, volatile void * ptr, int size)
{
	switch (size) {
		case 1:
			__asm__ __volatile__("xchgb %b0,%1"
				:"=q" (x)
				:"m" (*__xg(ptr)), "0" (x)
				:"memory");
			break;
		case 2:
			__asm__ __volatile__("xchgw %w0,%1"
				:"=r" (x)
				:"m" (*__xg(ptr)), "0" (x)
				:"memory");
			break;
		case 4:
			__asm__ __volatile__("xchgl %0,%1"
				:"=r" (x)
				:"m" (*__xg(ptr)), "0" (x)
				:"memory");
			break;
	}
	return x;
}

#endif /* _ASM_SYSTEM_H */
