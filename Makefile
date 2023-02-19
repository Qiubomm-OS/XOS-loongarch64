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

C_FLAGS = -nostdinc -I./arch/loongarch/include -I./include -I./arch/loongarch/include/uapi \
	-I./include/uapi -DVMLINUX_LOAD_ADDRESS=0x9000000000200000 \
	-mabi=lp64s -G0 -pipe -msoft-float -mexplicit-relocs \
	-c -g
LD_FLAGS = -m elf64loongarch --no-warn-rwx-segments -T ./arch/loongarch/kernel/vmlinux.lds -Map ./build/kernel.map -G0 -static -n -nostdlib
ASM_FLAGS = -f elf -g -F stabs

all: $(S_OBJECTS) $(C_OBJECTS) link

.c.o:
	@echo 编译代码文件 $< ...
	$(CC) $(C_FLAGS) $< -o $@

.S.o:
	@echo 编译汇编文件 $< ...
	$(CC) $(C_FLAGS) $< -o $@

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
