#
# Makefile for the linux kernel
#

CPPFLAGS_vmlinux.lds = -DVMLINUX_LOAD_ADDRESS=0x100000

obj-y += boot.o
obj-y += io_delay.o
obj-y += setup.o

head-y	:= grub_head.o
extra-y := $(head-y) vmlinux.lds
