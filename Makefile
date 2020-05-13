C_SOURCES = $(shell find . -type f -name '*.c')
H_SOURCES = $(shell find . -type f -name '*.h')
O_SOURCES = $(shell find . -type f -name '*.o')
A_SOURCES = $(shell find . -type f -name '*.asm')
R_SOURCES = $(shell find . -type f -name '*.real')

OBJ = ${C_SOURCES:.c=.o} ${A_SOURCES:.asm=.o}

ARCH = x86_64
CROSS = toolchain/bin

CC = clang -target x86_64-unknown-none
LD = gcc -no-pie
AS = nasm

KNL_TARGET = boot/kernel.elf

CFLAGS =	-ggdb 					\
			-nostdlib 				\
			-fno-stack-protector	\
			-nostartfiles 			\
			-nodefaultlibs 			\
			-Wall					\
			-Wextra 				\
			-Wpedantic 				\
			-ffreestanding 			\
			-std=gnu11 				\
			-mcmodel=kernel 		\
			-I. 					\
			-Ilib					\
			-fno-pic

QEMUFLAGS =	-m 1G 											\
			-device isa-debug-exit,iobase=0xf4,iosize=0x04	\
			-boot menu=on 									\
			-hda slate.img

O_LEVEL = 	2

LDFLAGS =	-ffreestanding 			\
			-O${O_LEVEL}			\
			-nostdlib				\
			-z max-page-size=0x1000

all:
	rm -rf slate.img
	make slate.img
	sudo make run

ci: slate.img

run:
	qemu-system-x86_64 ${QEMUFLAGS} -serial stdio

slate.img:
	rm -rf slate.img slate_image/
	mkdir slate_image
	dd if=/dev/zero bs=1M count=0 seek=64 of=slate.img
	parted -s slate.img mklabel msdos
	parted -s slate.img mkpart primary 1 100%
	make $(FS)
	sudo rm -rf slate_image loopback_dev
	sudo ./boot/qloader2-install boot/qloader2.bin slate.img

ifndef $(FS):
FS := ext2
endif

ext2: ${KNL_TARGET}
	sudo losetup -Pf --show slate.img > loopback_dev
	sudo mkfs.ext2 `cat loopback_dev`p1
	sudo mount `cat loopback_dev`p1 slate_image/
	sudo mkdir slate_image/boot/
	sudo cp ${KNL_TARGET} slate_image/boot/
	sudo cp boot/qloader2.cfg slate_image/
	sync
	sudo umount slate_image/
	sudo losetup -d `cat loopback_dev`

ext4: ${KNL_TARGET}
	sudo losetup -Pf --show slate.img > loopback_dev
	sudo mkfs.ext4 `cat loopback_dev`p1
	sudo mount `cat loopback_dev`p1 slate_image/
	sudo mkdir slate_image/boot/
	sudo cp ${KNL_TARGET} slate_image/boot/
	sudo cp boot/qloader2.cfg slate_image/
	sync
	sudo umount slate_image/
	sudo losetup -d `cat loopback_dev`

echfs: ${KNL_TARGET}
	echfs-utils -g -p1 test.img quick-format 512
	echfs-utils -g -p1 test.img import ${KNL_TARGET} ${KNL_TARGET}
	echfs-utils -g -p1 test.img import boot/qloader2.cfg qloader2.cfg

boot/kernel.elf: ${N_SOURCES:.real=.bin} ${OBJ}
	${LD} ${LDFLAGS} -o $@ -T boot/linker.ld ${OBJ}

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

%.bin: %.real
	nasm -f bin -o $@ $<

%.o: %.asm
	nasm -f elf64 -F dwarf -g -o $@ $<

clean:
	find . 		-type f -name '*.o' 	-delete
	find . 		-type f -name '*.elf' 	-delete
	find real/ 	-type f -name '*.bin' 	-delete
