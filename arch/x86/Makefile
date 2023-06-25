KBUILD_DEFCONFIG := i386_defconfig

KBUILD_CFLAGS	+= -nostdinc -I./arch/i386/include -I./include -I./arch/i386/include/uapi -I./include/uapi\
	-fno-builtin -m32

head-y		:= arch/i386/boot/head.o

boot := arch/i386/boot

Image: vmlinux
	$(Q)$(MAKE) $(build)=$(boot) $(boot)/$@
