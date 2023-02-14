#!Makefile

BUILD_DIR = ./build
C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))
S_SOURCES = $(shell find . -name "*.S")
S_OBJECTS = $(patsubst %.S, %.o, $(S_SOURCES))

CC = /opt/cross-tools/bin/loongarch64-unknown-linux-gnu-gcc
LD = /opt/cross-tools/bin/loongarch64-unknown-linux-gnu-ld
ASM = nasm

C_FLAGS = -I ./include/ -c -fno-builtin -m32 -fno-stack-protector -nostdinc -fno-pic -gdwarf-2
LD_FLAGS = -m elf_i386 -T ./script/kernel.ld -Map ./build/kernel.map -nostdlib
ASM_FLAGS = -f elf -g -F stabs

all: $(S_OBJECTS) $(C_OBJECTS) link update_image

.c.o:
	@echo 编译代码文件 $< ...
	$(CC) $(C_FLAGS) $< -o $@

.S.o:
	@echo 编译汇编文件 $< ...
	# $(ASM) $(ASM_FLAGS) $< -o $@
	$(CC) $(C_FLAGS) $< -o $@

link:
	@echo 链接内核文件...
	$(LD) $(LD_FLAGS) $(S_OBJECTS) $(C_OBJECTS) -o kernel.bin

.PHONY:clean
clean:
	$(RM) $(S_OBJECTS) $(C_OBJECTS) kernel.bin

.PHONY:update_image
update_image:
	sudo cp kernel.bin ./hdisk/boot/
	sleep 1
	sync;sync;sync

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
