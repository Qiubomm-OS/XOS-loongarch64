#ifndef _ASM_ADDRSPACE_H
#define _ASM_ADDRSPACE_H

#define DMW_PABITS	48
#define TO_PHYS_MASK	((1ULL << DMW_PABITS) - 1)

#define _ATYPE32_	int
#define _ATYPE64_	__s64

#define _ACAST32_		(_ATYPE_)(_ATYPE32_)	/* widen if necessary */
#define _ACAST64_		(_ATYPE64_)		/* do _not_ narrow */

#define PHYSADDR(a)		((_ACAST64_(a)) & TO_PHYS_MASK)

#endif /* _ASM_ADDRSPACE_H */
