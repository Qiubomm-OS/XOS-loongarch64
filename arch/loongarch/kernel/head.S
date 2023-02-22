#include <linux/init.h>
#include <linux/linkage.h>

#include <asm/loongarch.h>

#include "efi-head.S"

	__HEAD
	
_head:
	.word MZ_MAGIC
	.org	0x8
	.dword kernel_entry
	.dword	_end - _text		/* Kernel image effective size */
	.quad	0			/* Kernel image load offset from start of RAM */
	.org	0x38			/* 0x20 ~ 0x37 reserved */
	.long	LINUX_PE_MAGIC
	.long	pe_header - _head	/* Offset to the PE header */

pe_header:
	__EFI_PE_HEADER


SYM_DATA(kernel_asize, .long _end - _text);
SYM_DATA(kernel_fsize, .long _edata - _text);
SYM_DATA(kernel_offset, .long kernel_offset - _text);

	__REF

	.align 12

SYM_CODE_START(kernel_entry)			# kernel entry point

SYM_CODE_END(kernel_entry)

SYM_ENTRY(kernel_entry_end, SYM_L_GLOBAL, SYM_A_NONE)
