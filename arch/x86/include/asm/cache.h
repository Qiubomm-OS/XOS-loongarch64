#ifndef _ASM_CACHE_H
#define _ASM_CACHE_H

/* L1缓存行大小 */
#define L1_CACHE_SHIFT	(CONFIG_X86_L1_CACHE_SHIFT)
#define L1_CACHE_BYTES	(1 << L1_CACHE_SHIFT)

#define SMP_CACHE_BYTES L1_CACHE_BYTES

#define __read_mostly __section(".data..read_mostly")

#define INTERNODE_CACHE_SHIFT CONFIG_X86_INTERNODE_CACHE_SHIFT
#define INTERNODE_CACHE_BYTES (1 << INTERNODE_CACHE_SHIFT)

#endif /* _ASM_CACHE_H */
