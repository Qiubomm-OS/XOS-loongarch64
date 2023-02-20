#include <linux/efi.h>
#include <linux/types.h>
#include <linux/compiler.h>
#include <linux/printk.h>

#include "efistub.h"

efi_status_t __efiapi efi_pe_entry(efi_handle_t handle, efi_system_table_t *systab)
{	
	efi_loaded_image_t *image;
	efi_status_t status;
	unsigned long image_addr;
	unsigned long image_size = 0;
	/* addr/point and size pairs for memory management*/
	char *cmdline_ptr = NULL;
	efi_guid_t loaded_image_proto = LOADED_IMAGE_PROTOCOL_GUID;
	unsigned long reserve_addr = 0;
	unsigned long reserve_size = 0;

	// WRITE_ONCE(efi_system_table, systab);
	memcpy(&efi_system_table, systab, sizeof(efi_system_table));

	/* Check if we were booted by the EFI firmware */
	if (efi_system_table->hdr.signature != EFI_SYSTEM_TABLE_SIGNATURE)
		return EFI_INVALID_PARAMETER;
	
	/*
	 * Get a handle to the loaded image protocol.  This is used to get
	 * information about the running image, such as size and the command
	 * line.
	 */
	status = efi_bs_call(handle_protocol, handle, &loaded_image_proto,
			     (void *)&image);
	if (status != EFI_SUCCESS)
		return status;
	
	efi_puts("hello world!\n");
	while(1);
}
