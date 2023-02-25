#!Makefile

BUILD_DIR = ./build
C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))
S_SOURCES = $(shell find . -name "*.S")
S_OBJECTS = $(patsubst %.S, %.o, $(S_SOURCES))

CC = /opt/cross-tools/bin/loongarch64-unknown-linux-gnu-gcc
LD = /opt/cross-tools/bin/loongarch64-unknown-linux-gnu-ld
OBJCOPY = /opt/cross-tools/bin/loongarch64-unknown-linux-gnu-objcopy
ASM = nasm

C_FLAGS = -nostdinc \
	-I./arch/loongarch/include -I./include -I./arch/loongarch/include/uapi -I./include/uapi \
	-mabi=lp64s -G0 -pipe -msoft-float -mexplicit-relocs -fno-builtin \
	-c -g
LD_FLAGS = -m elf64loongarch --no-warn-rwx-segments -T ./arch/loongarch/kernel/vmlinux.lds -Map ./build/kernel.map -G0 -n -nostdlib
ASM_FLAGS = -f elf -g -F stabs

all: $(S_OBJECTS) $(C_OBJECTS) link

arch/loongarch/kernel/head.o: arch/loongarch/kernel/head.S
	loongarch64-unknown-linux-gnu-gcc \
	-nostdinc -I./arch/loongarch/include \
	-I./include -I./arch/loongarch/include/uapi \
	-I./include/uapi \
	-fno-PIE -mabi=lp64s -G0 -pipe -msoft-float -mexplicit-relocs \
	-ffreestanding -mno-check-zero-division \
	-c -o arch/loongarch/kernel/head.o arch/loongarch/kernel/head.S

drivers/firmware/efi/libstub/efi-stub-entry.o: drivers/firmware/efi/libstub/efi-stub-entry.c
	loongarch64-unknown-linux-gnu-gcc -nostdinc -I./arch/loongarch/include -I./include -I./arch/loongarch/include/uapi \
	-I./include/uapi -Wall -Wundef -Werror=strict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common \
	-fshort-wchar -fno-PIE -Werror=implicit-function-declaration -Werror=implicit-int \
	-Werror=return-type -Wno-format-security -funsigned-char -std=gnu11 \
	-mabi=lp64s -G0 -pipe -msoft-float -mexplicit-relocs -ffreestanding -mno-check-zero-division \
	-fno-asynchronous-unwind-tables -isystem /opt/cross-tools/lib/gcc/loongarch64-unknown-linux-gnu/13.0.0/include \
	-fno-delete-null-pointer-checks -Wno-frame-address -Wno-format-truncation -Wno-format-overflow \
	-Wno-address-of-packed-member -O2 -fno-allow-store-data-races -Wframe-larger-than=2048 \
	-fstack-protector-strong -Wno-main -Wno-unused-but-set-variable -Wno-unused-const-variable -Wno-dangling-pointer \
	-fomit-frame-pointer -ftrivial-auto-var-init=zero -fno-stack-clash-protection \
	-Wdeclaration-after-statement -Wvla -Wno-pointer-sign -Wcast-function-type -Wno-stringop-truncation \
	-Wno-stringop-overflow -Wno-restrict -Wno-maybe-uninitialized -Wno-alloc-size-larger-than -Wimplicit-fallthrough=5 \
	-fno-strict-overflow -fno-stack-check -fconserve-stack -Werror=date-time -Werror=incompatible-pointer-types -Werror=designated-init \
	-Wno-packed-not-aligned -fpie -Os -DDISABLE_BRANCH_PROFILING -ffreestanding -fno-stack-protector -D__DISABLE_EXPORTS \
	-mdirect-extern-access -c -o drivers/firmware/efi/libstub/efi-stub-entry.o drivers/firmware/efi/libstub/efi-stub-entry.c

drivers/firmware/efi/libstub/printk.o: drivers/firmware/efi/libstub/printk.c
	loongarch64-unknown-linux-gnu-gcc -nostdinc -I./arch/loongarch/include -I./include -I./arch/loongarch/include/uapi \
	-I./include/uapi -Wall -Wundef -Werror=strict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -fshort-wchar \
	-fno-PIE -Werror=implicit-function-declaration -Werror=implicit-int -Werror=return-type -Wno-format-security -funsigned-char -std=gnu11 \
	-mabi=lp64s -G0 -pipe -msoft-float -mexplicit-relocs -ffreestanding -mno-check-zero-division -fno-asynchronous-unwind-tables \
	-isystem /opt/cross-tools/lib/gcc/loongarch64-unknown-linux-gnu/13.0.0/include -fno-delete-null-pointer-checks -Wno-frame-address \
	-Wno-format-truncation -Wno-format-overflow -Wno-address-of-packed-member -O2 -fno-allow-store-data-races -Wframe-larger-than=2048 \
	-fstack-protector-strong -Wno-main -Wno-unused-but-set-variable -Wno-unused-const-variable -Wno-dangling-pointer -fomit-frame-pointer \
	-ftrivial-auto-var-init=zero -fno-stack-clash-protection -Wdeclaration-after-statement -Wvla -Wno-pointer-sign -Wcast-function-type \
	-Wno-stringop-truncation -Wno-stringop-overflow -Wno-restrict -Wno-maybe-uninitialized -Wno-alloc-size-larger-than -Wimplicit-fallthrough=5 \
	-fno-strict-overflow -fno-stack-check -fconserve-stack -Werror=date-time -Werror=incompatible-pointer-types -Werror=designated-init \
	-Wno-packed-not-aligned -fpie -Os -DDISABLE_BRANCH_PROFILING -D__NO_FORTIFY -ffreestanding -fno-stack-protector -D__DISABLE_EXPORTS \
	-mdirect-extern-access \
	-c -o drivers/firmware/efi/libstub/printk.o drivers/firmware/efi/libstub/printk.c

drivers/firmware/efi/libstub/systable.o: drivers/firmware/efi/libstub/systable.c
	loongarch64-unknown-linux-gnu-gcc -nostdinc -I./arch/loongarch/include -I./include -I./arch/loongarch/include/uapi \
	-I./include/uapi -Wall -Wundef -Werror=strict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -fshort-wchar -fno-PIE \
	-Werror=implicit-function-declaration -Werror=implicit-int -Werror=return-type -Wno-format-security -funsigned-char -std=gnu11 \
	-mabi=lp64s -G0 -pipe -msoft-float -mexplicit-relocs -ffreestanding -mno-check-zero-division -fno-asynchronous-unwind-tables \
	-isystem /opt/cross-tools/lib/gcc/loongarch64-unknown-linux-gnu/13.0.0/include -fno-delete-null-pointer-checks -Wno-frame-address \
	-Wno-format-truncation -Wno-format-overflow -Wno-address-of-packed-member -O2 -fno-allow-store-data-races -Wframe-larger-than=2048 \
	-fstack-protector-strong -Wno-main -Wno-unused-but-set-variable -Wno-unused-const-variable -Wno-dangling-pointer -fomit-frame-pointer \
	-ftrivial-auto-var-init=zero -fno-stack-clash-protection -Wdeclaration-after-statement -Wvla -Wno-pointer-sign -Wcast-function-type \
	-Wno-stringop-truncation -Wno-stringop-overflow -Wno-restrict -Wno-maybe-uninitialized -Wno-alloc-size-larger-than -Wimplicit-fallthrough=5 \
	-fno-strict-overflow -fno-stack-check -fconserve-stack -Werror=date-time -Werror=incompatible-pointer-types -Werror=designated-init \
	-Wno-packed-not-aligned -fpie -Os -DDISABLE_BRANCH_PROFILING -D__NO_FORTIFY -ffreestanding -fno-stack-protector -D__DISABLE_EXPORTS \
	-mdirect-extern-access \
	-c -o drivers/firmware/efi/libstub/systable.o drivers/firmware/efi/libstub/systable.c

lib/string.o: lib/string.c
	loongarch64-unknown-linux-gnu-gcc -nostdinc -I./arch/loongarch/include -I./include -I./arch/loongarch/include/uapi \
	-I./include/uapi -Wall -Wundef -Werror=strict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -fshort-wchar -fno-PIE \
	-Werror=implicit-function-declaration -Werror=implicit-int -Werror=return-type -Wno-format-security -funsigned-char -std=gnu11 -mabi=lp64s \
	-G0 -pipe -msoft-float -mexplicit-relocs -ffreestanding -mno-check-zero-division -fno-asynchronous-unwind-tables \
	-isystem /opt/cross-tools/lib/gcc/loongarch64-unknown-linux-gnu/13.0.0/include -fno-delete-null-pointer-checks -Wno-frame-address \
	-Wno-format-truncation -Wno-format-overflow -Wno-address-of-packed-member -O2 -fno-allow-store-data-races -Wframe-larger-than=2048 \
	-fstack-protector-strong -Wno-main -Wno-unused-but-set-variable -Wno-unused-const-variable -Wno-dangling-pointer -fomit-frame-pointer \
	-ftrivial-auto-var-init=zero -fno-stack-clash-protection -Wdeclaration-after-statement -Wvla -Wno-pointer-sign -Wcast-function-type \
	-Wno-stringop-truncation -Wno-stringop-overflow -Wno-restrict -Wno-maybe-uninitialized -Wno-alloc-size-larger-than -Wimplicit-fallthrough=5 \
	-fno-strict-overflow -fno-stack-check -fconserve-stack -Werror=date-time -Werror=incompatible-pointer-types -Werror=designated-init \
	-Wno-packed-not-aligned -ffreestanding   -mdirect-extern-access \
	-c -o lib/string.o lib/string.c

link:
	@echo 链接内核文件...
	$(LD) $(LD_FLAGS) $(S_OBJECTS) $(C_OBJECTS) -o kernel
	$(OBJCOPY) -O binary --remove-section=.comment --remove-section=.note \
		--remove-section=.options --remove-section=.note.gnu.build-id \
		-S kernel kernel.efi

.PHONY:clean
clean:
	$(RM) $(S_OBJECTS) $(C_OBJECTS) kernel kernel.efi

.PHONY:update_image

.PHONY:mount_image
mount_image:
	sudo mount -o loop ./hd.img ./hdisk/

.PHONY:umount_image
umount_image:
	sudo umount ./hdisk

.PHONY:qemu
qemu:
	qemu-system-i386 -serial stdio -drive file=./hd.img,format=raw,index=0,media=disk -m 512

.PHONY:debug
debug:
	qemu-system-i386 -serial stdio -S -s -drive file=./hd.img,format=raw,index=0,media=disk -m 512
