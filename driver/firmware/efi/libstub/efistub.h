#ifndef _DRIVER_FIRMWARE_EFI_EFISTUB_H
#define _DRIVER_FIRMWARE_EFI_EFISTUB_H

#include <linux/efi.h>
#include <linux/types.h>

typedef union {
	struct {
		u32			revision;
		efi_handle_t		parent_handle;
		efi_system_table_t	*system_table;
		efi_handle_t		device_handle;
		void			*file_path;
		void			*reserved;
		u32			load_options_size;
		void			*load_options;
		void			*image_base;
		__aligned_u64		image_size;
		unsigned int		image_code_type;
		unsigned int		image_data_type;
		efi_status_t		(__efiapi *unload)(efi_handle_t image_handle);
	};
	struct {
		u32		revision;
		u32		parent_handle;
		u32		system_table;
		u32		device_handle;
		u32		file_path;
		u32		reserved;
		u32		load_options_size;
		u32		load_options;
		u32		image_base;
		__aligned_u64	image_size;
		u32		image_code_type;
		u32		image_data_type;
		u32		unload;
	} mixed_mode;
} efi_loaded_image_t;

#endif /* _DRIVER_FIRMWARE_EFI_EFISTUB_H */
