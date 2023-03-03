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

static u32 get_supported_rt_services(void)
{
	const efi_rt_properties_table_t *rt_prop_table;
	u32 supported = EFI_RT_SUPPORTED_ALL;

	rt_prop_table = get_efi_config_table(EFI_RT_PROPERTIES_TABLE_GUID);
	if (rt_prop_table)
		supported &= rt_prop_table->runtime_services_supported;

	return supported;
}

efi_status_t efi_alloc_virtmap(efi_memory_desc_t **virtmap,
			       unsigned long *desc_size, u32 *desc_ver)
{
	unsigned long size, mmap_key;
	efi_status_t status;

	/*
	 * Use the size of the current memory map as an upper bound for the
	 * size of the buffer we need to pass to SetVirtualAddressMap() to
	 * cover all EFI_MEMORY_RUNTIME regions.
	 */
	size = 0;
	status = efi_bs_call(get_memory_map, &size, NULL, &mmap_key, desc_size,
			     desc_ver);
	if (status != EFI_BUFFER_TOO_SMALL)
		return EFI_LOAD_ERROR;

	return efi_bs_call(allocate_pool, EFI_LOADER_DATA, size,
			   (void **)virtmap);
}

efi_status_t efi_stub_common(efi_handle_t handle,
			     efi_loaded_image_t *image,
			     unsigned long image_addr,
			     char *cmdline_ptr)
{
	// struct screen_info *si = NULL;
	efi_status_t status;

	status = check_platform_features();
	if (status != EFI_SUCCESS)
		return status;

	efi_novamap |= !(get_supported_rt_services() &
			 EFI_RT_SUPPORTED_SET_VIRTUAL_ADDRESS_MAP);
	
	return status;
}
