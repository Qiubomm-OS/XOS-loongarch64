#include <linux/init.h>
#include <linux/linkage.h>
#include <linux/pe.h>

#include <asm/loongarch.h>

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
	.long	PE_MAGIC
.Lcoff_header:
	.short	IMAGE_FILE_MACHINE_LOONGARCH64		/* Machine */
	.short	.Lsection_count				/* NumberOfSections */
	.long	0 					/* TimeDateStamp */
	.long	0					/* PointerToSymbolTable */
	.long	0					/* NumberOfSymbols */
	.short	.Lsection_table - .Loptional_header	/* SizeOfOptionalHeader */
	.short	IMAGE_FILE_DEBUG_STRIPPED | \
		IMAGE_FILE_EXECUTABLE_IMAGE | \
		IMAGE_FILE_LINE_NUMS_STRIPPED		/* Characteristics */

.Loptional_header:
	.short	PE_OPT_MAGIC_PE32PLUS			/* PE32+ format */
	.byte	0x02					/* MajorLinkerVersion */
	.byte	0x14					/* MinorLinkerVersion */
	.long	__inittext_end - .Lefi_header_end	/* SizeOfCode */
	.long	_end - __initdata_begin			/* SizeOfInitializedData */
	.long	0					/* SizeOfUninitializedData */
	.long	efi_pe_entry - _head			/* AddressOfEntryPoint */
	.long	.Lefi_header_end - _head		/* BaseOfCode */

.Lextra_header_fields:
	.quad	0					/* ImageBase */
	.long	PECOFF_SEGMENT_ALIGN			/* SectionAlignment */
	.long	PECOFF_FILE_ALIGN			/* FileAlignment */
	.short	0					/* MajorOperatingSystemVersion */
	.short	0					/* MinorOperatingSystemVersion */
	.short	LINUX_EFISTUB_MAJOR_VERSION		/* MajorImageVersion */
	.short	LINUX_EFISTUB_MINOR_VERSION		/* MinorImageVersion */
	.short	0					/* MajorSubsystemVersion */
	.short	0					/* MinorSubsystemVersion */
	.long	0					/* Win32VersionValue */

	.long	_end - _head				/* SizeOfImage */

	/* Everything before the kernel image is considered part of the header */
	.long	.Lefi_header_end - _head		/* SizeOfHeaders */
	.long	0					/* CheckSum */
	.short	IMAGE_SUBSYSTEM_EFI_APPLICATION		/* Subsystem */
	.short	0					/* DllCharacteristics */
	.quad	0					/* SizeOfStackReserve */
	.quad	0					/* SizeOfStackCommit */
	.quad	0					/* SizeOfHeapReserve */
	.quad	0					/* SizeOfHeapCommit */
	.long	0					/* LoaderFlags */
	.long	(.Lsection_table - .) / 8		/* NumberOfRvaAndSizes */

	.quad	0					/* ExportTable */
	.quad	0					/* ImportTable */
	.quad	0					/* ResourceTable */
	.quad	0					/* ExceptionTable */
	.quad	0					/* CertificationTable */
	.quad	0					/* BaseRelocationTable */

	/* Section table */
.Lsection_table:
	.ascii	".text\0\0\0"
	.long	__inittext_end - .Lefi_header_end	/* VirtualSize */
	.long	.Lefi_header_end - _head		/* VirtualAddress */
	.long	__inittext_end - .Lefi_header_end	/* SizeOfRawData */
	.long	.Lefi_header_end - _head		/* PointerToRawData */

	.long	0					/* PointerToRelocations */
	.long	0					/* PointerToLineNumbers */
	.short	0					/* NumberOfRelocations */
	.short	0					/* NumberOfLineNumbers */
	.long	IMAGE_SCN_CNT_CODE | \
		IMAGE_SCN_MEM_READ | \
		IMAGE_SCN_MEM_EXECUTE			/* Characteristics */

	.ascii	".data\0\0\0"
	.long	_end - __initdata_begin			/* VirtualSize */
	.long	__initdata_begin - _head		/* VirtualAddress */
	.long	_edata - __initdata_begin		/* SizeOfRawData */
	.long	__initdata_begin - _head		/* PointerToRawData */

	.long	0					/* PointerToRelocations */
	.long	0					/* PointerToLineNumbers */
	.short	0					/* NumberOfRelocations */
	.short	0					/* NumberOfLineNumbers */
	.long	IMAGE_SCN_CNT_INITIALIZED_DATA | \
		IMAGE_SCN_MEM_READ | \
		IMAGE_SCN_MEM_WRITE			/* Characteristics */

	.set	.Lsection_count, (. - .Lsection_table) / 40

	.balign	0x10000					/* PECOFF_SEGMENT_ALIGN */
.Lefi_header_end:

SYM_DATA(kernel_asize, .long _end - _text);
SYM_DATA(kernel_fsize, .long _edata - _text);
SYM_DATA(kernel_offset, .long kernel_offset - _text);

	__REF

	.align 12

SYM_CODE_START(kernel_entry)			# kernel entry point

SYM_CODE_END(kernel_entry)

SYM_ENTRY(kernel_entry_end, SYM_L_GLOBAL, SYM_A_NONE)
