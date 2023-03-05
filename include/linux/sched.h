#ifndef _LINUX_SCHED_H
#define _LINUX_SCHED_H

#include <asm/thread_info.h>

union thread_union {
	unsigned long stack[THREAD_SIZE/sizeof(long)];
};

#endif /* _LINUX_SCHED_H */
