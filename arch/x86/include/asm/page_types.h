#ifndef _ASM_PAGE_TYPES_H
#define _ASM_PAGE_TYPES_H

#include <linux/const.h>

/* PAGE_SHIFT 确定页面大小 */
#define PAGE_SHIFT		12
#define PAGE_SIZE		(_AC(1,UL) << PAGE_SHIFT)
#define PAGE_MASK		(~(PAGE_SIZE-1))

#endif /* _ASM_PAGE_TYPES_H */
