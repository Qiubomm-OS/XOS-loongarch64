#ifndef _ASM_PROCESSOR_H
#define _ASM_PROCESSOR_H

#include <asm/page.h>

#define load_cr3(pgdir) \
	asm volatile("movl %0,%%cr3": :"r" (__pa(pgdir)));

#endif /* _ASM_PROCESSOR_H */
