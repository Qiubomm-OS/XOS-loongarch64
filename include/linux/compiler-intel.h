#ifndef _LINUX_COMPILER_INTEL_H
#define _LINUX_COMPILER_INTEL_H

#define barrier() __memory_barrier()
#define barrier_data(ptr) barrier()

#endif /* _LINUX_COMPILER_INTEL_H */
