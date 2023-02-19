#ifndef _DRIVER_FIRMWARE_EFI_EFISTUB_H
#define _DRIVER_FIRMWARE_EFI_EFISTUB_H

#include <linux/efi.h>
#include <linux/types.h>
#include <linux/compiler_attributes.h>

extern const efi_system_table_t *efi_system_table;

#ifndef ARCH_HAS_EFISTUB_WRAPPERS

#define efi_is_native()			(true)
#define efi_table_attr(inst, attr)	(inst)->attr
#define efi_fn_call(inst, func, ...)	(inst)->func(__VA_ARGS__)

#endif /* ARCH_HAS_EFISTUB_WRAPPERS */

#define efi_call_proto(inst, func, ...) ({			\
	__typeof__(inst) __inst = (inst);			\
	efi_fn_call(__inst, func, __inst, ##__VA_ARGS__);	\
})
#define efi_bs_call(func, ...) \
	efi_fn_call(efi_table_attr(efi_system_table, boottime), func, ##__VA_ARGS__)
#define efi_rt_call(func, ...) \
	efi_fn_call(efi_table_attr(efi_system_table, runtime), func, ##__VA_ARGS__)
#define efi_dxe_call(func, ...) \
	efi_fn_call(efi_dxe_table, func, ##__VA_ARGS__)

typedef struct efi_generic_dev_path efi_device_path_protocol_t;

typedef union efi_device_path_from_text_protocol efi_device_path_from_text_protocol_t;

typedef void *efi_event_t;
/* Note that notifications won't work in mixed mode */
typedef void (__efiapi *efi_event_notify_t)(efi_event_t, void *);

#define EFI_EVT_TIMER		0x80000000U
#define EFI_EVT_RUNTIME		0x40000000U
#define EFI_EVT_NOTIFY_WAIT	0x00000100U
#define EFI_EVT_NOTIFY_SIGNAL	0x00000200U

/**
 * efi_set_event_at() - add event to events array
 *
 * @events:	array of UEFI events
 * @ids:	index where to put the event in the array
 * @event:	event to add to the aray
 *
 * boottime->wait_for_event() takes an array of events as input.
 * Provide a helper to set it up correctly for mixed mode.
 */
static inline
void efi_set_event_at(efi_event_t *events, size_t idx, efi_event_t event)
{
	if (efi_is_native())
		events[idx] = event;
	else
		((u32 *)events)[idx] = (u32)(unsigned long)event;
}

#define EFI_TPL_APPLICATION	4
#define EFI_TPL_CALLBACK	8
#define EFI_TPL_NOTIFY		16
#define EFI_TPL_HIGH_LEVEL	31

typedef enum {
	EfiTimerCancel,
	EfiTimerPeriodic,
	EfiTimerRelative
} EFI_TIMER_DELAY;

/*
 * EFI Boot Services table
 */
union efi_boot_services {
	struct {
		efi_table_hdr_t hdr;
		void *raise_tpl;
		void *restore_tpl;
		efi_status_t (__efiapi *allocate_pages)(int, int, unsigned long,
							efi_physical_addr_t *);
		efi_status_t (__efiapi *free_pages)(efi_physical_addr_t,
						    unsigned long);
		efi_status_t (__efiapi *get_memory_map)(unsigned long *, void *,
							unsigned long *,
							unsigned long *, u32 *);
		efi_status_t (__efiapi *allocate_pool)(int, unsigned long,
						       void **);
		efi_status_t (__efiapi *free_pool)(void *);
		efi_status_t (__efiapi *create_event)(u32, unsigned long,
						      efi_event_notify_t, void *,
						      efi_event_t *);
		efi_status_t (__efiapi *set_timer)(efi_event_t,
						  EFI_TIMER_DELAY, u64);
		efi_status_t (__efiapi *wait_for_event)(unsigned long,
							efi_event_t *,
							unsigned long *);
		void *signal_event;
		efi_status_t (__efiapi *close_event)(efi_event_t);
		void *check_event;
		void *install_protocol_interface;
		void *reinstall_protocol_interface;
		void *uninstall_protocol_interface;
		efi_status_t (__efiapi *handle_protocol)(efi_handle_t,
							 efi_guid_t *, void **);
		void *__reserved;
		void *register_protocol_notify;
		efi_status_t (__efiapi *locate_handle)(int, efi_guid_t *,
						       void *, unsigned long *,
						       efi_handle_t *);
		efi_status_t (__efiapi *locate_device_path)(efi_guid_t *,
							    efi_device_path_protocol_t **,
							    efi_handle_t *);
		efi_status_t (__efiapi *install_configuration_table)(efi_guid_t *,
								     void *);
		efi_status_t (__efiapi *load_image)(bool, efi_handle_t,
						    efi_device_path_protocol_t *,
						    void *, unsigned long,
						    efi_handle_t *);
		efi_status_t (__efiapi *start_image)(efi_handle_t, unsigned long *,
						     efi_char16_t **);
		efi_status_t __noreturn (__efiapi *exit)(efi_handle_t,
							 efi_status_t,
							 unsigned long,
							 efi_char16_t *);
		efi_status_t (__efiapi *unload_image)(efi_handle_t);
		efi_status_t (__efiapi *exit_boot_services)(efi_handle_t,
							    unsigned long);
		void *get_next_monotonic_count;
		efi_status_t (__efiapi *stall)(unsigned long);
		void *set_watchdog_timer;
		void *connect_controller;
		efi_status_t (__efiapi *disconnect_controller)(efi_handle_t,
							       efi_handle_t,
							       efi_handle_t);
		void *open_protocol;
		void *close_protocol;
		void *open_protocol_information;
		void *protocols_per_handle;
		void *locate_handle_buffer;
		efi_status_t (__efiapi *locate_protocol)(efi_guid_t *, void *,
							 void **);
		efi_status_t (__efiapi *install_multiple_protocol_interfaces)(efi_handle_t *, ...);
		efi_status_t (__efiapi *uninstall_multiple_protocol_interfaces)(efi_handle_t, ...);
		void *calculate_crc32;
		void (__efiapi *copy_mem)(void *, const void *, unsigned long);
		void (__efiapi *set_mem)(void *, unsigned long, unsigned char);
		void *create_event_ex;
	};
	struct {
		efi_table_hdr_t hdr;
		u32 raise_tpl;
		u32 restore_tpl;
		u32 allocate_pages;
		u32 free_pages;
		u32 get_memory_map;
		u32 allocate_pool;
		u32 free_pool;
		u32 create_event;
		u32 set_timer;
		u32 wait_for_event;
		u32 signal_event;
		u32 close_event;
		u32 check_event;
		u32 install_protocol_interface;
		u32 reinstall_protocol_interface;
		u32 uninstall_protocol_interface;
		u32 handle_protocol;
		u32 __reserved;
		u32 register_protocol_notify;
		u32 locate_handle;
		u32 locate_device_path;
		u32 install_configuration_table;
		u32 load_image;
		u32 start_image;
		u32 exit;
		u32 unload_image;
		u32 exit_boot_services;
		u32 get_next_monotonic_count;
		u32 stall;
		u32 set_watchdog_timer;
		u32 connect_controller;
		u32 disconnect_controller;
		u32 open_protocol;
		u32 close_protocol;
		u32 open_protocol_information;
		u32 protocols_per_handle;
		u32 locate_handle_buffer;
		u32 locate_protocol;
		u32 install_multiple_protocol_interfaces;
		u32 uninstall_multiple_protocol_interfaces;
		u32 calculate_crc32;
		u32 copy_mem;
		u32 set_mem;
		u32 create_event_ex;
	} mixed_mode;
};

typedef enum {
	EfiGcdMemoryTypeNonExistent,
	EfiGcdMemoryTypeReserved,
	EfiGcdMemoryTypeSystemMemory,
	EfiGcdMemoryTypeMemoryMappedIo,
	EfiGcdMemoryTypePersistent,
	EfiGcdMemoryTypeMoreReliable,
	EfiGcdMemoryTypeMaximum
} efi_gcd_memory_type_t;

typedef struct {
	efi_physical_addr_t base_address;
	u64 length;
	u64 capabilities;
	u64 attributes;
	efi_gcd_memory_type_t gcd_memory_type;
	void *image_handle;
	void *device_handle;
} efi_gcd_memory_space_desc_t;

/*
 * EFI DXE Services table
 */
union efi_dxe_services_table {
	struct {
		efi_table_hdr_t hdr;
		void *add_memory_space;
		void *allocate_memory_space;
		void *free_memory_space;
		void *remove_memory_space;
		efi_status_t (__efiapi *get_memory_space_descriptor)(efi_physical_addr_t,
								     efi_gcd_memory_space_desc_t *);
		efi_status_t (__efiapi *set_memory_space_attributes)(efi_physical_addr_t,
								     u64, u64);
		void *get_memory_space_map;
		void *add_io_space;
		void *allocate_io_space;
		void *free_io_space;
		void *remove_io_space;
		void *get_io_space_descriptor;
		void *get_io_space_map;
		void *dispatch;
		void *schedule;
		void *trust;
		void *process_firmware_volume;
		void *set_memory_space_capabilities;
	};
	struct {
		efi_table_hdr_t hdr;
		u32 add_memory_space;
		u32 allocate_memory_space;
		u32 free_memory_space;
		u32 remove_memory_space;
		u32 get_memory_space_descriptor;
		u32 set_memory_space_attributes;
		u32 get_memory_space_map;
		u32 add_io_space;
		u32 allocate_io_space;
		u32 free_io_space;
		u32 remove_io_space;
		u32 get_io_space_descriptor;
		u32 get_io_space_map;
		u32 dispatch;
		u32 schedule;
		u32 trust;
		u32 process_firmware_volume;
		u32 set_memory_space_capabilities;
	} mixed_mode;
};

typedef union efi_uga_draw_protocol efi_uga_draw_protocol_t;

union efi_uga_draw_protocol {
	struct {
		efi_status_t (__efiapi *get_mode)(efi_uga_draw_protocol_t *,
						  u32*, u32*, u32*, u32*);
		void *set_mode;
		void *blt;
	};
	struct {
		u32 get_mode;
		u32 set_mode;
		u32 blt;
	} mixed_mode;
};

typedef struct {
	u16 scan_code;
	efi_char16_t unicode_char;
} efi_input_key_t;

union efi_simple_text_input_protocol {
	struct {
		void *reset;
		efi_status_t (__efiapi *read_keystroke)(efi_simple_text_input_protocol_t *,
							efi_input_key_t *);
		efi_event_t wait_for_key;
	};
	struct {
		u32 reset;
		u32 read_keystroke;
		u32 wait_for_key;
	} mixed_mode;
};

efi_status_t efi_wait_for_key(unsigned long usec, efi_input_key_t *key);

union efi_simple_text_output_protocol {
	struct {
		void *reset;
		efi_status_t (__efiapi *output_string)(efi_simple_text_output_protocol_t *,
						       efi_char16_t *);
		void *test_string;
	};
	struct {
		u32 reset;
		u32 output_string;
		u32 test_string;
	} mixed_mode;
};

#define PIXEL_RGB_RESERVED_8BIT_PER_COLOR		0
#define PIXEL_BGR_RESERVED_8BIT_PER_COLOR		1
#define PIXEL_BIT_MASK					2
#define PIXEL_BLT_ONLY					3
#define PIXEL_FORMAT_MAX				4

typedef struct {
	u32 red_mask;
	u32 green_mask;
	u32 blue_mask;
	u32 reserved_mask;
} efi_pixel_bitmask_t;

typedef struct {
	u32 version;
	u32 horizontal_resolution;
	u32 vertical_resolution;
	int pixel_format;
	efi_pixel_bitmask_t pixel_information;
	u32 pixels_per_scan_line;
} efi_graphics_output_mode_info_t;

typedef union efi_graphics_output_protocol_mode efi_graphics_output_protocol_mode_t;

union efi_graphics_output_protocol_mode {
	struct {
		u32 max_mode;
		u32 mode;
		efi_graphics_output_mode_info_t *info;
		unsigned long size_of_info;
		efi_physical_addr_t frame_buffer_base;
		unsigned long frame_buffer_size;
	};
	struct {
		u32 max_mode;
		u32 mode;
		u32 info;
		u32 size_of_info;
		u64 frame_buffer_base;
		u32 frame_buffer_size;
	} mixed_mode;
};

typedef union efi_graphics_output_protocol efi_graphics_output_protocol_t;

union efi_graphics_output_protocol {
	struct {
		efi_status_t (__efiapi *query_mode)(efi_graphics_output_protocol_t *,
						    u32, unsigned long *,
						    efi_graphics_output_mode_info_t **);
		efi_status_t (__efiapi *set_mode)  (efi_graphics_output_protocol_t *, u32);
		void *blt;
		efi_graphics_output_protocol_mode_t *mode;
	};
	struct {
		u32 query_mode;
		u32 set_mode;
		u32 blt;
		u32 mode;
	} mixed_mode;
};

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

typedef struct {
	u64			size;
	u64			file_size;
	u64			phys_size;
	efi_time_t		create_time;
	efi_time_t		last_access_time;
	efi_time_t		modification_time;
	__aligned_u64		attribute;
	efi_char16_t		filename[];
} efi_file_info_t;

typedef union efi_file_protocol efi_file_protocol_t;

union efi_file_protocol {
	struct {
		u64		revision;
		efi_status_t	(__efiapi *open)	(efi_file_protocol_t *,
							 efi_file_protocol_t **,
							 efi_char16_t *, u64,
							 u64);
		efi_status_t	(__efiapi *close)	(efi_file_protocol_t *);
		efi_status_t	(__efiapi *delete)	(efi_file_protocol_t *);
		efi_status_t	(__efiapi *read)	(efi_file_protocol_t *,
							 unsigned long *,
							 void *);
		efi_status_t	(__efiapi *write)	(efi_file_protocol_t *,
							 unsigned long, void *);
		efi_status_t	(__efiapi *get_position)(efi_file_protocol_t *,
							 u64 *);
		efi_status_t	(__efiapi *set_position)(efi_file_protocol_t *,
							 u64);
		efi_status_t	(__efiapi *get_info)	(efi_file_protocol_t *,
							 efi_guid_t *,
							 unsigned long *,
							 void *);
		efi_status_t	(__efiapi *set_info)	(efi_file_protocol_t *,
							 efi_guid_t *,
							 unsigned long,
							 void *);
		efi_status_t	(__efiapi *flush)	(efi_file_protocol_t *);
	};
	struct {
		u64 revision;
		u32 open;
		u32 close;
		u32 delete;
		u32 read;
		u32 write;
		u32 get_position;
		u32 set_position;
		u32 get_info;
		u32 set_info;
		u32 flush;
	} mixed_mode;
};

typedef union efi_simple_file_system_protocol efi_simple_file_system_protocol_t;

union efi_simple_file_system_protocol {
	struct {
		u64		revision;
		efi_status_t	(__efiapi *open_volume)(efi_simple_file_system_protocol_t *,
							efi_file_protocol_t **);
	};
	struct {
		u64 revision;
		u32 open_volume;
	} mixed_mode;
};

#define EFI_FILE_MODE_READ	0x0000000000000001
#define EFI_FILE_MODE_WRITE	0x0000000000000002
#define EFI_FILE_MODE_CREATE	0x8000000000000000

typedef enum {
	EfiPciIoWidthUint8,
	EfiPciIoWidthUint16,
	EfiPciIoWidthUint32,
	EfiPciIoWidthUint64,
	EfiPciIoWidthFifoUint8,
	EfiPciIoWidthFifoUint16,
	EfiPciIoWidthFifoUint32,
	EfiPciIoWidthFifoUint64,
	EfiPciIoWidthFillUint8,
	EfiPciIoWidthFillUint16,
	EfiPciIoWidthFillUint32,
	EfiPciIoWidthFillUint64,
	EfiPciIoWidthMaximum
} EFI_PCI_IO_PROTOCOL_WIDTH;

typedef enum {
	EfiPciIoAttributeOperationGet,
	EfiPciIoAttributeOperationSet,
	EfiPciIoAttributeOperationEnable,
	EfiPciIoAttributeOperationDisable,
	EfiPciIoAttributeOperationSupported,
    EfiPciIoAttributeOperationMaximum
} EFI_PCI_IO_PROTOCOL_ATTRIBUTE_OPERATION;

typedef struct {
	u32 read;
	u32 write;
} efi_pci_io_protocol_access_32_t;

typedef union efi_pci_io_protocol efi_pci_io_protocol_t;

typedef
efi_status_t (__efiapi *efi_pci_io_protocol_cfg_t)(efi_pci_io_protocol_t *,
						   EFI_PCI_IO_PROTOCOL_WIDTH,
						   u32 offset,
						   unsigned long count,
						   void *buffer);

typedef struct {
	void *read;
	void *write;
} efi_pci_io_protocol_access_t;

typedef struct {
	efi_pci_io_protocol_cfg_t read;
	efi_pci_io_protocol_cfg_t write;
} efi_pci_io_protocol_config_access_t;

union efi_pci_io_protocol {
	struct {
		void *poll_mem;
		void *poll_io;
		efi_pci_io_protocol_access_t mem;
		efi_pci_io_protocol_access_t io;
		efi_pci_io_protocol_config_access_t pci;
		void *copy_mem;
		void *map;
		void *unmap;
		void *allocate_buffer;
		void *free_buffer;
		void *flush;
		efi_status_t (__efiapi *get_location)(efi_pci_io_protocol_t *,
						      unsigned long *segment_nr,
						      unsigned long *bus_nr,
						      unsigned long *device_nr,
						      unsigned long *func_nr);
		void *attributes;
		void *get_bar_attributes;
		void *set_bar_attributes;
		uint64_t romsize;
		void *romimage;
	};
	struct {
		u32 poll_mem;
		u32 poll_io;
		efi_pci_io_protocol_access_32_t mem;
		efi_pci_io_protocol_access_32_t io;
		efi_pci_io_protocol_access_32_t pci;
		u32 copy_mem;
		u32 map;
		u32 unmap;
		u32 allocate_buffer;
		u32 free_buffer;
		u32 flush;
		u32 get_location;
		u32 attributes;
		u32 get_bar_attributes;
		u32 set_bar_attributes;
		u64 romsize;
		u32 romimage;
	} mixed_mode;
};

#define EFI_PCI_IO_ATTRIBUTE_ISA_MOTHERBOARD_IO 0x0001
#define EFI_PCI_IO_ATTRIBUTE_ISA_IO 0x0002
#define EFI_PCI_IO_ATTRIBUTE_VGA_PALETTE_IO 0x0004
#define EFI_PCI_IO_ATTRIBUTE_VGA_MEMORY 0x0008
#define EFI_PCI_IO_ATTRIBUTE_VGA_IO 0x0010
#define EFI_PCI_IO_ATTRIBUTE_IDE_PRIMARY_IO 0x0020
#define EFI_PCI_IO_ATTRIBUTE_IDE_SECONDARY_IO 0x0040
#define EFI_PCI_IO_ATTRIBUTE_MEMORY_WRITE_COMBINE 0x0080
#define EFI_PCI_IO_ATTRIBUTE_IO 0x0100
#define EFI_PCI_IO_ATTRIBUTE_MEMORY 0x0200
#define EFI_PCI_IO_ATTRIBUTE_BUS_MASTER 0x0400
#define EFI_PCI_IO_ATTRIBUTE_MEMORY_CACHED 0x0800
#define EFI_PCI_IO_ATTRIBUTE_MEMORY_DISABLE 0x1000
#define EFI_PCI_IO_ATTRIBUTE_EMBEDDED_DEVICE 0x2000
#define EFI_PCI_IO_ATTRIBUTE_EMBEDDED_ROM 0x4000
#define EFI_PCI_IO_ATTRIBUTE_DUAL_ADDRESS_CYCLE 0x8000
#define EFI_PCI_IO_ATTRIBUTE_ISA_IO_16 0x10000
#define EFI_PCI_IO_ATTRIBUTE_VGA_PALETTE_IO_16 0x20000
#define EFI_PCI_IO_ATTRIBUTE_VGA_IO_16 0x40000

struct efi_dev_path;

typedef union apple_properties_protocol apple_properties_protocol_t;

union apple_properties_protocol {
	struct {
		unsigned long version;
		efi_status_t (__efiapi *get)(apple_properties_protocol_t *,
					     struct efi_dev_path *,
					     efi_char16_t *, void *, u32 *);
		efi_status_t (__efiapi *set)(apple_properties_protocol_t *,
					     struct efi_dev_path *,
					     efi_char16_t *, void *, u32);
		efi_status_t (__efiapi *del)(apple_properties_protocol_t *,
					     struct efi_dev_path *,
					     efi_char16_t *);
		efi_status_t (__efiapi *get_all)(apple_properties_protocol_t *,
						 void *buffer, u32 *);
	};
	struct {
		u32 version;
		u32 get;
		u32 set;
		u32 del;
		u32 get_all;
	} mixed_mode;
};

typedef u32 efi_tcg2_event_log_format;

#define INITRD_EVENT_TAG_ID 0x8F3B22ECU
#define LOAD_OPTIONS_EVENT_TAG_ID 0x8F3B22EDU
#define EV_EVENT_TAG 0x00000006U
#define EFI_TCG2_EVENT_HEADER_VERSION	0x1

struct efi_tcg2_event {
	u32		event_size;
	struct {
		u32	header_size;
		u16	header_version;
		u32	pcr_index;
		u32	event_type;
	} __packed event_header;
	/* u8[] event follows here */
} __packed;

struct efi_tcg2_tagged_event {
	u32 tagged_event_id;
	u32 tagged_event_data_size;
	/* u8  tagged event data follows here */
} __packed;

typedef struct efi_tcg2_event efi_tcg2_event_t;
typedef struct efi_tcg2_tagged_event efi_tcg2_tagged_event_t;
typedef union efi_tcg2_protocol efi_tcg2_protocol_t;

union efi_tcg2_protocol {
	struct {
		void *get_capability;
		efi_status_t (__efiapi *get_event_log)(efi_tcg2_protocol_t *,
						       efi_tcg2_event_log_format,
						       efi_physical_addr_t *,
						       efi_physical_addr_t *,
						       efi_bool_t *);
		efi_status_t (__efiapi *hash_log_extend_event)(efi_tcg2_protocol_t *,
							       u64,
							       efi_physical_addr_t,
							       u64,
							       const efi_tcg2_event_t *);
		void *submit_command;
		void *get_active_pcr_banks;
		void *set_active_pcr_banks;
		void *get_result_of_set_active_pcr_banks;
	};
	struct {
		u32 get_capability;
		u32 get_event_log;
		u32 hash_log_extend_event;
		u32 submit_command;
		u32 get_active_pcr_banks;
		u32 set_active_pcr_banks;
		u32 get_result_of_set_active_pcr_banks;
	} mixed_mode;
};

struct riscv_efi_boot_protocol {
	u64 revision;

	efi_status_t (__efiapi *get_boot_hartid)(struct riscv_efi_boot_protocol *,
						 unsigned long *boot_hartid);
};

typedef union efi_load_file_protocol efi_load_file_protocol_t;
typedef union efi_load_file_protocol efi_load_file2_protocol_t;

union efi_load_file_protocol {
	struct {
		efi_status_t (__efiapi *load_file)(efi_load_file_protocol_t *,
						   efi_device_path_protocol_t *,
						   bool, unsigned long *, void *);
	};
	struct {
		u32 load_file;
	} mixed_mode;
};

typedef struct {
	u32 attributes;
	u16 file_path_list_length;
	u8 variable_data[];
	// efi_char16_t description[];
	// efi_device_path_protocol_t file_path_list[];
	// u8 optional_data[];
} __packed efi_load_option_t;

#define EFI_LOAD_OPTION_ACTIVE		0x0001U
#define EFI_LOAD_OPTION_FORCE_RECONNECT	0x0002U
#define EFI_LOAD_OPTION_HIDDEN		0x0008U
#define EFI_LOAD_OPTION_CATEGORY	0x1f00U
#define   EFI_LOAD_OPTION_CATEGORY_BOOT	0x0000U
#define   EFI_LOAD_OPTION_CATEGORY_APP	0x0100U

#define EFI_LOAD_OPTION_BOOT_MASK \
	(EFI_LOAD_OPTION_ACTIVE|EFI_LOAD_OPTION_HIDDEN|EFI_LOAD_OPTION_CATEGORY)
#define EFI_LOAD_OPTION_MASK (EFI_LOAD_OPTION_FORCE_RECONNECT|EFI_LOAD_OPTION_BOOT_MASK)

typedef struct {
	u32 attributes;
	u16 file_path_list_length;
	const efi_char16_t *description;
	const efi_device_path_protocol_t *file_path_list;
	u32 optional_data_size;
	const void *optional_data;
} efi_load_option_unpacked_t;

#endif /* _DRIVER_FIRMWARE_EFI_EFISTUB_H */