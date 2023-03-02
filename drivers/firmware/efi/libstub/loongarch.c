#include <linux/efi.h>
#include <asm/efi.h>

#include "efistub.h"

typedef void __noreturn (*kernel_entry_t)(bool efi, unsigned long cmdline,
					  unsigned long systab);

struct exit_boot_struct {
	efi_memory_desc_t	*runtime_map;
	int			runtime_entry_count;
};
