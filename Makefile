C_SOURCES = $(shell find . -type f -name '*.c' | grep -v "modules")
H_SOURCES = $(shell find . -type f -name '*.h' | grep -v "modules")
A_SOURCES = $(shell find . -type f -name '*.asm' | grep -v "modules")
R_SOURCES = $(shell find . -type f -name '*.real' | grep -v "modules")

OBJ = ${C_SOURCES:.c=.o} ${A_SOURCES:.asm=.o}

ARCH = x86_64

CC = clang -target ${ARCH}-unknown-none
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
			-fno-pic				\
			-mno-red-zone			\
			-mno-sse				\
			-mno-sse2

QEMUFLAGS =	-m 1G 			\
			-boot menu=on	\
			-hda slate.img	\

O_LEVEL = 	2

LDFLAGS =	-ffreestanding 			\
			-O${O_LEVEL}			\
			-nostdlib				\
			-z max-page-size=0x1000

all: 
	rm -rf slate.img slate_image/
	make slate.img
	make -C modules
	sudo make run

ci:
	make -C modules
	make slate.img

run: 
	qemu-system-x86_64 ${QEMUFLAGS} -serial stdio | tee "dump.log"

debug: slate.img
	make -C modules
	qemu-system-x86_64 ${QEMUFLAGS} -monitor stdio -d int -no-shutdown -no-reboot | tee "dump.log"

slate.img:
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
	sudo make cpy_files
	sync
	sudo umount slate_image/
	sudo losetup -d `cat loopback_dev`

ext4: ${KNL_TARGET}
	sudo losetup -Pf --show slate.img > loopback_dev
	sudo mkfs.ext4 `cat loopback_dev`p1
	sudo mount `cat loopback_dev`p1 slate_image/
	sudo make cpy_files
	sync
	sudo umount slate_image/
	sudo losetup -d `cat loopback_dev`

echfs: ${KNL_TARGET}
	echfs-utils -g -p1 slate.img quick-format 512
	echfs-utils -g -p1 slate.img import ${KNL_TARGET} ${KNL_TARGET}
	echfs-utils -g -p1 slate.img import boot/qloader2.cfg qloader2.cfg

cpy_files:
	sudo mkdir slate_image/boot/
	sudo mkdir slate_image/modules/
	sudo cp ${KNL_TARGET} slate_image/boot/
	sudo cp boot/qloader2.cfg slate_image/
	sudo cp modules/mod.o slate_image/modules/

boot/kernel.elf: ${R_SOURCES:.real=.bin} ${OBJ}
	${LD} ${LDFLAGS} -o $@ -T boot/linker.ld ${OBJ}

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

%.bin: %.real
	nasm -f bin -o $@ $<

%.o: %.asm
	nasm -f elf64 -F dwarf -g -o $@ $<

clean:
	rm ${OBJ}
	rm -f dump.log
	find . 		-type f -name '*.elf' 		-delete
	find real/ 	-type f -name '*.bin' 		-delete
	find .		-type f -name 'slate.img' 	-delete
