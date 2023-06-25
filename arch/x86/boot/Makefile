#
# arch/arm/boot/Makefile
#
# This file is included by the global makefile so that you can add your own
# architecture-specific flags and dependencies.
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 1995-2002 Russell King
#

MKIMAGE         := $(srctree)/scripts/mkuboot.sh

ifneq ($(MACHINE),)
include $(srctree)/$(MACHINE)/Makefile.boot
endif

targets := Image

$(obj)/Image: vmlinux FORCE
	$(call if_changed,objcopy)
	@echo '  Kernel: $@ is ready'
