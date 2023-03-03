#!Makefile

BUILD_DIR = ./build
C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))
S_SOURCES = $(shell find . -name "*.S")
S_OBJECTS = $(patsubst %.S, %.o, $(S_SOURCES))

CC = /opt/cross-tools/bin/loongarch64-unknown-linux-gnu-gcc
LD = /opt/cross-tools/bin/loongarch64-unknown-linux-gnu-ld
OBJCOPY = /opt/cross-tools/bin/loongarch64-unknown-linux-gnu-objcopy
ASM = /opt/cross-tools/bin/loongarch64-unknown-linux-gnu-gcc
QEMU = ../qemu-7.2.0/build/qemu-system-loongarch64

C_FLAGS = -nostdinc -I./arch/loongarch/include -I./include -I./arch/loongarch/include/uapi -I./include/uapi\
	-Wall -Wundef -Werror=strict-prototypes -Wno-trigraphs -fno-strict-aliasing \
	-fno-common -fshort-wchar -fno-PIE -Werror=implicit-function-declaration -Werror=implicit-int -Werror=return-type \
	-Wno-format-security -funsigned-char -std=gnu11 -mabi=lp64s -G0 -pipe -msoft-float -mexplicit-relocs -ffreestanding \
	-mno-check-zero-division -fno-asynchronous-unwind-tables -isystem /opt/cross-tools/lib/gcc/loongarch64-unknown-linux-gnu/13.0.0/include \
	-fno-delete-null-pointer-checks -Wno-frame-address -Wno-format-truncation -Wno-format-overflow -Wno-address-of-packed-member \
	-O2 -fno-allow-store-data-races -Wframe-larger-than=2048 -fstack-protector-strong -Wno-main -Wno-unused-but-set-variable \
	-Wno-unused-const-variable -Wno-dangling-pointer -fomit-frame-pointer -ftrivial-auto-var-init=zero -fno-stack-clash-protection \
	-Wdeclaration-after-statement -Wvla -Wno-pointer-sign -Wcast-function-type -Wno-stringop-truncation -Wno-stringop-overflow -Wno-restrict \
	-Wno-maybe-uninitialized -Wno-alloc-size-larger-than -Wimplicit-fallthrough=5 -fno-strict-overflow -fno-stack-check -fconserve-stack \
	-Werror=date-time -Werror=incompatible-pointer-types -Werror=designated-init -Wno-packed-not-aligned -fpie -Os -DDISABLE_BRANCH_PROFILING \
	-ffreestanding -fno-stack-protector -mdirect-extern-access -g -c

LD_FLAGS = -m elf64loongarch --no-warn-rwx-segments -T ./arch/loongarch/kernel/vmlinux.lds -Map ./build/kernel.map -G0 -n -nostdlib
ASM_FLAGS = -f elf -g -F stabs

all: $(S_OBJECTS) $(C_OBJECTS) link

arch/loongarch/kernel/head.o: arch/loongarch/kernel/head.S
	@echo 编译代码文件 $< ...
	/opt/cross-tools/bin/loongarch64-unknown-linux-gnu-gcc -nostdinc -I./arch/loongarch/include \
	-I./include -I./arch/loongarch/include/uapi -I./include/uapi \
	-D__ASSEMBLY__ -fno-PIE -mabi=lp64s -G0 -pipe -msoft-float -mexplicit-relocs \
	-ffreestanding -mno-check-zero-division \
	-c -o arch/loongarch/kernel/head.o arch/loongarch/kernel/head.S

drivers/firmware/efi/libstub/efi-stub-entry.o: drivers/firmware/efi/libstub/efi-stub-entry.c
	@echo 编译代码文件 $< ...
	@$(CC) $(C_FLAGS) -o $@ $<

drivers/firmware/efi/libstub/printk.o: drivers/firmware/efi/libstub/printk.c
	@echo 编译代码文件 $< ...
	@$(CC) $(C_FLAGS) -o $@ $<

drivers/firmware/efi/libstub/systable.o: drivers/firmware/efi/libstub/systable.c
	@echo 编译代码文件 $< ...
	@$(CC) $(C_FLAGS) -o $@ $<

lib/string.o: lib/string.c
	@echo 编译代码文件 $< ...
	@$(CC) $(C_FLAGS) -o $@ $<

drivers/firmware/efi/libstub/vsprintf.o: drivers/firmware/efi/libstub/vsprintf.c
	@echo 编译代码文件 $< ...
	@$(CC) $(C_FLAGS) -o $@ $<

drivers/firmware/efi/libstub/loongarch-stub.o: drivers/firmware/efi/libstub/loongarch-stub.c
	@echo 编译代码文件 $< ...
	@$(CC) $(C_FLAGS) -o $@ $<

drivers/firmware/efi/libstub/screen_info.o: drivers/firmware/efi/libstub/screen_info.c
	@echo 编译代码文件 $< ...
	@$(CC) $(C_FLAGS) -o $@ $<

drivers/firmware/efi/libstub/efi-stub.o: drivers/firmware/efi/libstub/efi-stub.c
	@echo 编译代码文件 $< ...
	@$(CC) $(C_FLAGS) -o $@ $<

drivers/firmware/efi/libstub/loongarch.o: drivers/firmware/efi/libstub/loongarch.c
	@echo 编译代码文件 $< ...
	@$(CC) $(C_FLAGS) -o $@ $<	

drivers/firmware/efi/libstub/efi-stub-helper.o: drivers/firmware/efi/libstub/efi-stub-helper.c
	@echo 编译代码文件 $< ...
	@$(CC) $(C_FLAGS) -o $@ $<

drivers/firmware/efi/libstub/mem.o: drivers/firmware/efi/libstub/mem.c
	@echo 编译代码文件 $< ...
	@$(CC) $(C_FLAGS) -o $@ $<

link:
	@echo 链接内核文件...
	mkdir -p build/
	@$(LD) $(LD_FLAGS) $(S_OBJECTS) $(C_OBJECTS) -o kernel
	$(OBJCOPY) -O binary --remove-section=.comment --remove-section=.note \
		--remove-section=.options --remove-section=.note.gnu.build-id \
		-S kernel kernel.efi

.PHONY:clean
clean:
	$(RM) $(S_OBJECTS) $(C_OBJECTS) kernel kernel.efi

.PHONY:qemu
qemu:
	$(QEMU) -machine virt -m 4G -cpu la464 -smp 1 -bios ../qemu-binary/QEMU_EFI.fd -kernel kernel.efi -nographic

.PHONY:debug
debug:
	$(QEMU) -machine virt -m 4G -cpu la464 -smp 1 -bios ../qemu-binary/QEMU_EFI.fd -kernel kernel.efi -nographic -s -S
