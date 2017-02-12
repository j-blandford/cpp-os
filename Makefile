# Kernel flags and targets
KTARGET = i686-elf
ARCH = i386

# Emulator flags and targets
QEMU = qemu-system-i386
QEMU_FLAGS = -s
QEMU_FLAGS += -m 512
QEMU_FLAGS += -serial stdio
QEMU_FLAGS += -cdrom myos.iso
QEMU_FLAGS += -vga cirrus

QEMU_VIDEO = vid=qemu,,1280,,720

# Helper scripts
INFO = scripts/info.sh
SUCCESS = scripts/success.sh

# Kernel object files and header files
KERNEL_OBJS = $(patsubst %.cpp,%.o,$(wildcard kernel/*.cpp))
KERNEL_OBJS+= $(patsubst %.cpp,%.o,$(wildcard kernel/*/*.cpp))
KERNEL_OBJS+= $(patsubst %.cpp,%.o,$(wildcard kernel/*/*/*.cpp))
KERNEL_HEADERS = $(shell find kernel/include/ -type f -name '*.h')

# Libc object files and header files
# LIBC_OBJS = $(patsubst %.c,%.o,$(wildcard libc/*/*.c))
# LIBC_OBJS+= $(patsubst %.c,%.o,$(wildcard libc/*/*/*.c))
# LIBC_OBJS+= $(patsubst %.c,%.o,$(wildcard libc/*/*/*/*.c))
# LIBC_HEADERS = $(shell find libc/include/ -type f -name '*.h')

KERNEL_ASMOBJS = $(patsubst %.s,%.o,$(wildcard kernel/*.s))

################################################################################################
# Lets go!
################################################################################################

.PHONY: all install run-iso build-isodir clean

.SECONDARY:

.SUFFIXES:

################################
# QEMU stuff
################################

build-isodir:
	mkdir -p isodir
	mkdir -p isodir/boot
	mkdir -p isodir/boot/grub

	cp sysroot/boot/myos.elf isodir/boot/myos.elf
	cp sysroot/boot/grub/grub.cfg isodir/boot/grub/grub.cfg

myos.iso: build-isodir
	grub-mkrescue -o myos.iso isodir

run-iso: myos.iso
	${QEMU} ${QEMU_FLAGS} 
#-append "${QEMU_VIDEO}"

run: install myos.iso
	${QEMU} ${QEMU_FLAGS} 

clean:
	@-rm -f kernel/*.o
	@-rm -f kernel/*/*.o
	@-rm -f kernel/*/*/*.o
	@-rm -f sysroot/boot/myos.elf
	@-rm -rf isodir
	@-rm -f libc/*.o



################################
# Libc build stuff
################################

# # Collect all of the object files and make sure they're compiled
# build-libc: ${LIBC_OBJS}
# 	@${INFO} "---->" "Libc built..."

# # This provides the object files for the target "build-kernel"
# libc/%.o: libc/%.c ${LIBC_HEADERS}
# 	@${KERNEL_GCC} ${CFLAGS} -nostdlib -g -I./libc/include -c -o $@ $< ${ERRORS}


################################
# Kernel build stuff
################################
# CFLAGS  = -O1 -std=c99
# CFLAGS += -finline-functions -ffreestanding
# CFLAGS += -Wall -Wextra -Wno-unused-function -Wno-unused-parameter -Wno-format -Warray-bounds
# CFLAGS += -D_KERNEL_
# ASMFLAGS = --32

CPPFLAGS  = -O2 -std=gnu++11 -nostdinc++
CPPFLAGS += -finline-functions -ffreestanding -nostdlib
CPPFLAGS += -Wall -Wextra -fno-exceptions -Warray-bounds -fno-rtti
CPPFLAGS += -Wno-write-strings
CPPFLAGS += -DKERNEL


NASM = nasm
NASM_FLAGS = -f elf		# output in ELF format
NASM_FLAGS+= -g 		# include debug information

KERNEL_GCC = ${KTARGET}-gcc
KERNEL_GPP = ${KTARGET}-g++

# This assembles the object files and links them into the finished kernel file
build-kernel: ${KERNEL_ASMOBJS} ${KERNEL_OBJS} ${KERNEL_ASMOBJS}
	@${NASM} ${NASM_FLAGS} kernel/boot.s
	@${KERNEL_GPP} -T kernel/arch/i386/linker.ld ${CPPFLAGS} -o sysroot/boot/myos.elf ${KERNEL_ASMOBJS} ${KERNEL_OBJS}
	@${INFO} "---->" "Kernel built..."

# This provides the object files for the target "build-kernel"
kernel/%.o: kernel/%.cpp ${KERNEL_HEADERS}
	@${KERNEL_GPP} ${CPPFLAGS} -g -c -I./kernel/include -o $@ $< ${ERRORS}

# Compiles .s (boot.s, etc.) into object files with NASM
kernel/boot.o:
	@${NASM} ${NASM_FLAGS} kernel/boot.s

install: clean build-kernel
	@${SUCCESS} "*---------------------------------*"
	@${SUCCESS} "*-*-*" "Install Success! :)"
	@${SUCCESS} "*---------------------------------*"
