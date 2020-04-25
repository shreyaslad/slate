C_SOURCES = $(shell find . -type f -name '*.c')
H_SOURCES = $(shell find . -type f -name '*.h')
O_SOURCES = $(shell find . -type f -name '*.o')
A_SOURCES = $(shell find . -type f -name '*.asm')
R_SOURCES = $(shell find . -type f -name '*.real')

OBJ = ${C_SOURCES:.c=.o} ${A_SOURCES:.asm=.o}

ARCH = x86_64
CROSS = toolchain/x86_64-elf-cross/bin

CC = ${CROSS}/${ARCH}-elf-gcc
AS = nasm

CFLAGS =	-ggdb \
			-nostdlib \
			-f-no-stack-protector \
			-nostartfiles \
			-nodefaultlibs \
			-Wall \
			-Wextra \
			-Wpedantic \
			-ffreestanding \
			-std=gnu11 \
			-mcmodel=kernel \
			-Iinclude

QEMUFLAGS =	-m 1G \
			-device isa-debug-exit,iobase=0xf4,iosize=0x04 \
			-boot menu=on \
			-hda slate.img

O_LEVEL = 2

LDFLAGS =	-ffreestanding \
			-O${O_LEVEL} \
			-nostdlib \
			-z max-page-size=0x1000

slate.img: kernel.elf
	rm -rf slate.img slate_image/
	mkdir slate_image
	dd if=/dev/zero bs=1M count=0 seek=64 of=slate.img
	parted -s test.img mklabel msdos
	parted -s test.img mkpart primary 1 100%
	sudo losetup -Pf --show test.img > loopback_dev
	sudo mkfs.ext2 `cat loopback_dev`p1
	sudo mount `cat loopback_dev`p1 slate_image/
	sudo cp boot/slate.elf slate_image/
	sudo cp boot/qloader2.cfg slate_image/
	sync
	sudo umount slate_image/
	sudo losetup -d `cat loopback_dev`
	rm -rf slate_image loopback_dev
	boot/qloader2-install boot/qloader2.bin slate.img
	qemu-system-x86_64 ${QEMUFLAGS}

kernel.elf: ${N_SOURCES:.real=.bin} ${OBJ}
	${CC} ${LDFLAGS} -o $@ -T boot/linker.ld ${OBJ}

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

%.bin: %.real
	nasm -f bin -o $@ $<

%.o: %.asm
	nasm -f elf64 -F dwarf -g -o $@ $<

clean:
	rm -rf *.bin *.o slate.img
