
#   Makefile - Makefile script.
#
#   Copyright (c) 2001 - Monaco F. J. 
#
#   This file is part of SYSeg.
#
#   SYSeg is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.



## 
## Stage 1: boot loader
## Stage 2: kernel
##

bin = boot.img

# Stage 1 uses some basic functions implemented in utils.c

stage1_obj = stage1.o utils.o

# Stage 2 also uses utils.c, but includes a lot more implemented in tyos.c

stage2_obj = stage2.o utils.o 

# Size of stage2 in 512-byte sectors

STAGE2_SIZE=1


AUXDIR =../../tools#

all : $(bin) 


# Update Makefile from Makefile.m4 if needed, and then invoke make again.
# If the source is from a pack-distribution, the lack of Makefile.m4
# inhibits the updating. 

ifndef UPDATED
#$(MAKECMDGOALS) : Makefile #### I removed this May 10, 2023.

Makefile_deps = Makefile.m4 ../../tools/docm4.m4 ../../tools/bintools.m4 ../../tools/Makegyver.mk

Makefile : $(shell if test -f Makefile.m4; then echo $(Makefile_deps); fi);
	@if ! test -f .dist; then\
	  cd .. && make;\
	  make -f Makefile clean;\
	  make -f Makefile UPDATED=1 $(MAKECMDGOAL);\
	fi

endif



###########################################################
##
## These are the rules of interest in this set of examples.
##



## 
## First and second stage, plus auxiliary items.
## 

boot_obj = stage1.o stage2.o utils.o

boot.bin : $(boot_obj) boot.ld rt0.o
	ld -melf_i386 --orphan-handling=discard  -T boot.ld $(boot_obj) -o $@

stage1.o stage2.o utils.o :%.o: %.c
	gcc -m16 -O0 -I. -Wall -fno-pic -fcf-protection=none  --freestanding -c $<  -o $@

stage1.o stage2.o utils.o : utils.h

rt0.o : rt0.S
	gcc -m16 -O0 -Wall -c $< -o $@

# Create a 1.44M floppy disk image (which is actually 1440 KB)

floppy.img: boot.bin
	rm -f $@
	dd bs=512 count=2880 if=/dev/zero of=$@ # Dummy 1440 kB floppy image.
	mkfs.fat -R 2 floppy.img

# Write boot.bin at begining of the floppy image.

boot.img : boot.bin floppy.img
	cp floppy.img $@
	dd conv=notrunc ibs=1 obs=1 skip=62 seek=62 if=boot.bin of=$@


# losetup

#
# Housekeeping
#

clean:
	rm -f *.bin *.elf *.o *.s *.iso *.img *.i
	make clean-extra

clean-extra:
	rm -f *~ \#*


#
# Programming exercise
#

EXPORT_FILES  = utils.c  utils.h rt0.S  stage1.c  stage2.c  boot.ld
EXPORT_FILES += Makefile
EXPORT_FILES += README ../../tools/COPYING 



# Self-contained pack distribution.
#
# make syseg-export     creates a tarball with the essential files, which
#       	        can be distributed independently of the rest of
#			this project.
#
# A pack distribution contain all the items necessary to build and run the
# relevant piece of software. It's useful,a for instance, to bundle
# self-contained subsections of SYSeg meant to be delivered as
# programming exercise assignments or illustrative source code examples.
#		
# In order to select which files should be included in the pack, list them
# in the appropriate variables
# 
# EXPORT_FILES_C    = C files (source, headers, linker scripts)
# EXPORT_FILES_MAKE = Makefiles
# EXPORT_FILES_TEXT = Text files (e.g. README)
# EXPORT_FILES_SH   = Shell scripts (standard /bin/sh)
#
# Except by text files, all other files will have their heading comment
# (the very first comment found in the file) replaced by a corresponding
# standard comments containing boilerplate copyright notice and licensing
# information, with blank fields to be filled in by the pack user.
# Attribution to SYSeg is also included for convenience.

TARNAME=tyos-0.1.0



syseg-export export:
	@if ! test -f .dist; then\
	  make do_export;\
	 else\
	  echo "This is a distribution pack already. Nothing to be done.";\
	fi

do_export:
	rm -rf $(TARNAME)
	mkdir $(TARNAME)
	(cd .. && make clean && make)
	for i in $(EXPORT_FILES); do\
	  ../../tools/syseg-export $$i $(TARNAME);\
	done
	touch $(TARNAME)/.dist
	tar zcvf $(TARNAME).tar.gz $(TARNAME)

clean-export:
	rm -f $(TARNAME).tar.gz
	rm -rf $(TARNAME)

.PHONY: syseg-export export clean-export


##
## Configuration
##


## Run on the emulator
##

%/run : %
	make run IMG=$<

run: $(IMG)
	qemu-system-i386 -drive format=raw,file=$< -net none

# These are deprecate; use %/run, instead.

run-bin: $(IMG)
	qemu-system-i386 -drive format=raw,file=$< -net none
	@echo "Shortcut run-bin is deprecated: use 'make run' instead."

run-iso: $(IMG)
	qemu-system-i386 -drive format=raw,file=$< -net none
	@echo "Shortcut run-iso is deprecated: use 'make run' instead."

run-fd : $(IMG)
	qemu-system-i386 -drive format=raw,file=$< -net none
	@echo "Shortcut run-fd is deprecated: use 'make run' instead."

##
## Create bootable USP stick if BIOS needs it
##

%.iso : %.bin
	xorriso -as mkisofs -b $< -o $@ -isohybrid-mbr $< -no-emul-boot -boot-load-size 4 ./

%.img : %.bin
	dd if=/dev/zero of=$@ bs=1024 count=1440
	dd if=$< of=$@ seek=0 conv=notrunc



stick: $(IMG)
	@if test -z "$(DEVICE)"; then \
	echo "*** ATTENTION: make IMG=foo.bin DEVICE=/dev/X"; exit 1; fi 
	dd if=$< of=$(DEVICE)

%/stick: %
	make stick IMG=$* DEVICE=$(wordlist 2, 2, $(MAKECMDGOALS))



.PHONY: clean clean-extra intel att 16 32 diss /diss /i16 /i32 /a16 /a32

