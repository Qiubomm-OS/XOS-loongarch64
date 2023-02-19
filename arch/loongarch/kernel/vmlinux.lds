/* __efistub_strcmp = strcmp; */
__efistub_kernel_entry = kernel_entry;
/* __efistub_kernel_asize = kernel_asize; */
__efistub_kernel_fsize = kernel_fsize;
__efistub_kernel_offset = kernel_offset;
/* __efistub_screen_info = screen_info; */
PECOFF_FILE_ALIGN = 0x200;
PECOFF_SEGMENT_ALIGN = 0x10000;
OUTPUT_ARCH(loongarch)
ENTRY(kernel_entry)
PHDRS {
 text PT_LOAD FLAGS(7);
 note PT_NOTE FLAGS(4);
}
/* jiffies = jiffies_64; */
SECTIONS
{
 . = 0x9000000000200000;
 _text = .;
 .head.text : AT(ADDR(.head.text) - 0) { KEEP(*(.head.text)) }
 . = ALIGN(PECOFF_SEGMENT_ALIGN);
 _stext = .;
 .text : {
  . = ALIGN(0); *(.text.hot .text.hot.*) *(.text .text.fixup) *(.text.unlikely .text.unlikely.*) *(.text.unknown .text.unknown.*) . = ALIGN(0); __noinstr_text_start = .; *(.noinstr.text) __noinstr_text_end = .; *(.text..refcount) *(.ref.text) *(.text.asan.* .text.tsan.*) *(.meminit.text*) *(.memexit.text*)
  . = ALIGN(0); __sched_text_start = .; *(.sched.text) __sched_text_end = .;
  . = ALIGN(0); __cpuidle_text_start = .; *(.cpuidle.text) __cpuidle_text_end = .;
  . = ALIGN(0); __lock_text_start = .; *(.spinlock.text) __lock_text_end = .;
  . = ALIGN(0); __kprobes_text_start = .; *(.kprobes.text) __kprobes_text_end = .;
  . = ALIGN(0); __irqentry_text_start = .; *(.irqentry.text) __irqentry_text_end = .;
  . = ALIGN(0); __softirqentry_text_start = .; *(.softirqentry.text) __softirqentry_text_end = .;
  *(.fixup)
  *(.gnu.warning)
 } :text = 0
 . = ALIGN(PECOFF_SEGMENT_ALIGN);
 _etext = .;
 . = ALIGN(4);
 .altinstructions : AT(ADDR(.altinstructions) - 0) {
  __alt_instructions = .;
  *(.altinstructions)
  __alt_instructions_end = .;
 }
 .got : ALIGN(16) { *(.got) }
 .plt : ALIGN(16) { *(.plt) }
 .got.plt : ALIGN(16) { *(.got.plt) }
 . = ALIGN(PECOFF_SEGMENT_ALIGN);
 __init_begin = .;
 __inittext_begin = .;
 . = ALIGN(16384); .init.text : AT(ADDR(.init.text) - 0) { _sinittext = .; *(.init.text .init.text.*) *(.text.startup) _einittext = .; }
 .exit.text : {
  *(.exit.text) *(.text.exit)
 }
 . = ALIGN(PECOFF_SEGMENT_ALIGN);
 __inittext_end = .;
 __initdata_begin = .;
 .init.data : AT(ADDR(.init.data) - 0) { KEEP(*(SORT(___kentry+*))) *(.init.data init.data.*) *(.init.rodata .init.rodata.*) . = ALIGN(8); __clk_of_table = .; KEEP(*(__clk_of_table)) KEEP(*(__clk_of_table_end)) . = ALIGN(8); __reservedmem_of_table = .; KEEP(*(__reservedmem_of_table)) KEEP(*(__reservedmem_of_table_end)) . = ALIGN(8); __cpu_method_of_table = .; KEEP(*(__cpu_method_of_table)) KEEP(*(__cpu_method_of_table_end)) . = ALIGN(32); __dtb_start = .; KEEP(*(.dtb.init.rodata)) __dtb_end = .; . = ALIGN(8); __irqchip_of_table = .; KEEP(*(__irqchip_of_table)) KEEP(*(__irqchip_of_table_end)) . = ALIGN(8); __irqchip_acpi_probe_table = .; KEEP(*(__irqchip_acpi_probe_table)) __irqchip_acpi_probe_table_end = .; . = ALIGN(8); __timer_acpi_probe_table = .; KEEP(*(__timer_acpi_probe_table)) __timer_acpi_probe_table_end = .; . = ALIGN(8); __governor_thermal_table = .; KEEP(*(__governor_thermal_table)) __governor_thermal_table_end = .; . = ALIGN(8); __earlycon_table = .; KEEP(*(__earlycon_table)) __earlycon_table_end = .; . = ALIGN(8); __start_lsm_info = .; KEEP(*(.lsm_info.init)) __end_lsm_info = .; . = ALIGN(8); __start_early_lsm_info = .; KEEP(*(.early_lsm_info.init)) __end_early_lsm_info = .; . = ALIGN(8); __kunit_suites_start = .; KEEP(*(.kunit_test_suites)) __kunit_suites_end = .; . = ALIGN(16); __setup_start = .; KEEP(*(.init.setup)) __setup_end = .; __initcall_start = .; KEEP(*(.initcallearly.init)) __initcall0_start = .; KEEP(*(.initcall0.init)) KEEP(*(.initcall0s.init)) __initcall1_start = .; KEEP(*(.initcall1.init)) KEEP(*(.initcall1s.init)) __initcall2_start = .; KEEP(*(.initcall2.init)) KEEP(*(.initcall2s.init)) __initcall3_start = .; KEEP(*(.initcall3.init)) KEEP(*(.initcall3s.init)) __initcall4_start = .; KEEP(*(.initcall4.init)) KEEP(*(.initcall4s.init)) __initcall5_start = .; KEEP(*(.initcall5.init)) KEEP(*(.initcall5s.init)) __initcallrootfs_start = .; KEEP(*(.initcallrootfs.init)) KEEP(*(.initcallrootfss.init)) __initcall6_start = .; KEEP(*(.initcall6.init)) KEEP(*(.initcall6s.init)) __initcall7_start = .; KEEP(*(.initcall7.init)) KEEP(*(.initcall7s.init)) __initcall_end = .; __con_initcall_start = .; KEEP(*(.con_initcall.init)) __con_initcall_end = .; . = ALIGN(4); __initramfs_start = .; KEEP(*(.init.ramfs)) . = ALIGN(8); KEEP(*(.init.ramfs.info)) }
 .exit.data : {
  *(.exit.data .exit.data.*) *(.fini_array .fini_array.*) *(.dtors .dtors.*)
 }
 . = ALIGN(16384); .data..percpu : AT(ADDR(.data..percpu) - 0) { __per_cpu_load = .; __per_cpu_start = .; *(.data..percpu..first) . = ALIGN(16384); *(.data..percpu..page_aligned) . = ALIGN(1 << 6); *(.data..percpu..read_mostly) . = ALIGN(1 << 6); *(.data..percpu) *(.data..percpu..shared_aligned) __per_cpu_end = .; }
 .rela.dyn : ALIGN(8) { *(.rela.dyn) *(.rela*) }
 .init.bss : {
  *(.init.bss)
 }
 . = ALIGN(PECOFF_SEGMENT_ALIGN);
 __initdata_end = .;
 __init_end = .;
 _sdata = .;
 . = ALIGN((4096)); .rodata : AT(ADDR(.rodata) - 0) { __start_rodata = .; *(.rodata) *(.rodata.*) . = ALIGN(32); __sched_class_highest = .; *(__stop_sched_class) *(__dl_sched_class) *(__rt_sched_class) *(__fair_sched_class) *(__idle_sched_class) __sched_class_lowest = .; . = ALIGN(8); __start_ro_after_init = .; *(.data..ro_after_init) . = ALIGN(8); __start___jump_table = .; KEEP(*(__jump_table)) __stop___jump_table = .; __end_ro_after_init = .; . = ALIGN(8); __start___tracepoints_ptrs = .; KEEP(*(__tracepoints_ptrs)) __stop___tracepoints_ptrs = .; *(__tracepoints_strings) } .rodata1 : AT(ADDR(.rodata1) - 0) { *(.rodata1) } .pci_fixup : AT(ADDR(.pci_fixup) - 0) { __start_pci_fixups_early = .; KEEP(*(.pci_fixup_early)) __end_pci_fixups_early = .; __start_pci_fixups_header = .; KEEP(*(.pci_fixup_header)) __end_pci_fixups_header = .; __start_pci_fixups_final = .; KEEP(*(.pci_fixup_final)) __end_pci_fixups_final = .; __start_pci_fixups_enable = .; KEEP(*(.pci_fixup_enable)) __end_pci_fixups_enable = .; __start_pci_fixups_resume = .; KEEP(*(.pci_fixup_resume)) __end_pci_fixups_resume = .; __start_pci_fixups_suspend = .; KEEP(*(.pci_fixup_suspend)) __end_pci_fixups_suspend = .; __start_pci_fixups_resume_early = .; KEEP(*(.pci_fixup_resume_early)) __end_pci_fixups_resume_early = .; __start_pci_fixups_suspend_late = .; KEEP(*(.pci_fixup_suspend_late)) __end_pci_fixups_suspend_late = .; } .builtin_fw : AT(ADDR(.builtin_fw) - 0) ALIGN(8) { __start_builtin_fw = .; KEEP(*(.builtin_fw)) __end_builtin_fw = .; } __ksymtab : AT(ADDR(__ksymtab) - 0) { __start___ksymtab = .; KEEP(*(SORT(___ksymtab+*))) __stop___ksymtab = .; } __ksymtab_gpl : AT(ADDR(__ksymtab_gpl) - 0) { __start___ksymtab_gpl = .; KEEP(*(SORT(___ksymtab_gpl+*))) __stop___ksymtab_gpl = .; } __kcrctab : AT(ADDR(__kcrctab) - 0) { __start___kcrctab = .; KEEP(*(SORT(___kcrctab+*))) __stop___kcrctab = .; } __kcrctab_gpl : AT(ADDR(__kcrctab_gpl) - 0) { __start___kcrctab_gpl = .; KEEP(*(SORT(___kcrctab_gpl+*))) __stop___kcrctab_gpl = .; } __ksymtab_strings : AT(ADDR(__ksymtab_strings) - 0) { *(__ksymtab_strings) } __init_rodata : AT(ADDR(__init_rodata) - 0) { *(.ref.rodata) *(.meminit.rodata) *(.memexit.rodata) } __param : AT(ADDR(__param) - 0) { __start___param = .; KEEP(*(__param)) __stop___param = .; } __modver : AT(ADDR(__modver) - 0) { __start___modver = .; KEEP(*(__modver)) __stop___modver = .; } . = ALIGN(4); __ex_table : AT(ADDR(__ex_table) - 0) { __start___ex_table = .; KEEP(*(__ex_table)) __stop___ex_table = .; } /DISCARD/ : { *(.note.GNU-stack) } .notes : AT(ADDR(.notes) - 0) { __start_notes = .; KEEP(*(.note.*)) __stop_notes = .; } . = ALIGN((4096)); __end_rodata = .;
 . = ALIGN(16384); .data : AT(ADDR(.data) - 0) { . = ALIGN(0x00004000); __start_init_task = .; init_thread_union = .; init_stack = .; KEEP(*(.data..init_task)) KEEP(*(.data..init_thread_info)) . = __start_init_task + 0x00004000; __end_init_task = .; . = ALIGN(16384); __nosave_begin = .; *(.data..nosave) . = ALIGN(16384); __nosave_end = .; . = ALIGN(16384); *(.data..page_aligned) . = ALIGN(16384); . = ALIGN(1 << 6); *(.data..cacheline_aligned) . = ALIGN(1 << 6); *(.data..read_mostly) . = ALIGN(1 << 6); *(.xiptext) *(.data) *(.data..decrypted) *(.ref.data) *(.data..shared_aligned) *(.meminit.data*) *(.memexit.data*) *(.data.unlikely) __start_once = .; *(.data.once) __end_once = .; . = ALIGN(32); *(__tracepoints) . = ALIGN(8); __start___dyndbg_classes = .; KEEP(*(__dyndbg_classes)) __stop___dyndbg_classes = .; __start___dyndbg = .; KEEP(*(__dyndbg)) __stop___dyndbg = .; CONSTRUCTORS } . = ALIGN(8); __bug_table : AT(ADDR(__bug_table) - 0) { __start___bug_table = .; KEEP(*(__bug_table)) __stop___bug_table = .; }
 .sdata : {
  *(.sdata)
 }
 .edata_padding : { BYTE(0); . = ALIGN(PECOFF_FILE_ALIGN); }
 _edata = .;
 . = ALIGN(0); __bss_start = .; . = ALIGN(0); .sbss : AT(ADDR(.sbss) - 0) { *(.dynsbss) *(.sbss) *(.scommon) } . = ALIGN(0x00010000); .bss : AT(ADDR(.bss) - 0) { *(.bss..swapper_pg_dir) . = ALIGN(16384); *(.bss..page_aligned) . = ALIGN(16384); *(.dynbss) *(.bss) *(COMMON) } . = ALIGN(8); __bss_stop = .;
 . = ALIGN(PECOFF_SEGMENT_ALIGN);
 _end = .;
 .stab 0 : { *(.stab) } .stabstr 0 : { *(.stabstr) } .stab.excl 0 : { *(.stab.excl) } .stab.exclstr 0 : { *(.stab.exclstr) } .stab.index 0 : { *(.stab.index) } .stab.indexstr 0 : { *(.stab.indexstr) }
 .debug 0 : { *(.debug) } .line 0 : { *(.line) } .debug_srcinfo 0 : { *(.debug_srcinfo) } .debug_sfnames 0 : { *(.debug_sfnames) } .debug_aranges 0 : { *(.debug_aranges) } .debug_pubnames 0 : { *(.debug_pubnames) } .debug_info 0 : { *(.debug_info .gnu.linkonce.wi.*) } .debug_abbrev 0 : { *(.debug_abbrev) } .debug_line 0 : { *(.debug_line) } .debug_frame 0 : { *(.debug_frame) } .debug_str 0 : { *(.debug_str) } .debug_loc 0 : { *(.debug_loc) } .debug_macinfo 0 : { *(.debug_macinfo) } .debug_pubtypes 0 : { *(.debug_pubtypes) } .debug_ranges 0 : { *(.debug_ranges) } .debug_weaknames 0 : { *(.debug_weaknames) } .debug_funcnames 0 : { *(.debug_funcnames) } .debug_typenames 0 : { *(.debug_typenames) } .debug_varnames 0 : { *(.debug_varnames) } .debug_gnu_pubnames 0 : { *(.debug_gnu_pubnames) } .debug_gnu_pubtypes 0 : { *(.debug_gnu_pubtypes) } .debug_types 0 : { *(.debug_types) } .debug_addr 0 : { *(.debug_addr) } .debug_line_str 0 : { *(.debug_line_str) } .debug_loclists 0 : { *(.debug_loclists) } .debug_macro 0 : { *(.debug_macro) } .debug_names 0 : { *(.debug_names) } .debug_rnglists 0 : { *(.debug_rnglists) } .debug_str_offsets 0 : { *(.debug_str_offsets) }
 .comment 0 : { *(.comment) } .symtab 0 : { *(.symtab) } .strtab 0 : { *(.strtab) } .shstrtab 0 : { *(.shstrtab) }
 .gptab.sdata : {
  *(.gptab.data)
  *(.gptab.sdata)
 }
 .gptab.sbss : {
  *(.gptab.bss)
  *(.gptab.sbss)
 }
 /DISCARD/ : { *(.exit.text) *(.text.exit) *(.exit.data .exit.data.*) *(.fini_array .fini_array.*) *(.dtors .dtors.*) *(.exitcall.exit) *(.discard) *(.discard.*) *(.modinfo) *(.gnu.version*) }
 /DISCARD/ : {
  *(.gnu.attributes)
  *(.options)
  *(.eh_frame)
 }
}
