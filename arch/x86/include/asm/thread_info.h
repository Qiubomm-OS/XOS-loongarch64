#ifndef _ASM_THREAD_INFO_H
#define _ASM_THREAD_INFO_H

#include <asm/page.h>
#include <asm/types.h>

#ifndef __ASSEMBLY__

struct thread_info {
        unsigned long flags;
#ifdef CONFIG_SMP
        u32 cpu;
#endif /* CONFIG_SMP */
};

#define INIT_THREAD_INFO(tsk)			\
{						\
	.flags		= 0,			\
}

#endif /*__ASSEMBLY__*/

#endif /* _ASM_THREAD_INFO_H */
