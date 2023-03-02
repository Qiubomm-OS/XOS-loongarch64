#include <linux/efi.h>
#include <asm/efi.h>

#include "efistub.h"

struct screen_info * __weak alloc_screen_info(void)
{
	return &screen_info;
}

void __weak free_screen_info(struct screen_info *si)
{
}

