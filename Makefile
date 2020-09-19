C_SOURCES = $(shell find . -type f -name '*.c' | grep -v "modules")
H_SOURCES = $(shell find . -type f -name '*.h' | grep -v "modules")
A_SOURCES = $(shell find . -type f -name '*.asm' | grep -v "modules")

OBJ = ${C_SOURCES:.c=.o} ${A_SOURCES:.asm=.o}

ARCH = x86_64

CC = clang
LD = gcc
AS = nasm

KNL_TARGET = boot/kernel.elf

CFLAGS =	-target ${ARCH}-unknown-none	\
			-ggdb							\
			-nostdlib						\
			-fno-stack-protector			\
			-nostartfiles					\
			-nodefaultlibs					\
			-Wall							\
			-Wextra							\
			-Wpedantic						\
			-ffreestanding					\
			-std=gnu11						\
			-mcmodel=kernel					\
			-I.								\
			-Ilib							\
			-fno-pic						\
			-mno-red-zone					\
			-mno-sse						\
			-mno-sse2						\
			#-fsanitize=undefined			\

QEMUFLAGS =	-m 3G			\
			-boot menu=on	\
			-hda slate.img	\
			-smp cpus=4		\
			-machine q35	\
			-name slate		\

O_LEVEL =	2

LDFLAGS =	-no-pie					\
			-ffreestanding			\
			-O${O_LEVEL}			\
			-nostdlib				\
			-z max-page-size=0x1000	\
			-T boot/linker.ld		\

all: ci run

ci:
	rm -rf slate.img slate_image
	make -C modules
	make slate.img

run:
	qemu-system-x86_64 ${QEMUFLAGS} -serial stdio | tee "dump.log"

debug: ci
	qemu-system-x86_64 ${QEMUFLAGS} -monitor stdio -d int -no-shutdown -no-reboot | tee "dump.log"

gdb: ci
	qemu-system-x86_64 -s -S ${QEMUFLAGS} &
	gdb -ex "target remote localhost:1234" -ex "symbol-file boot/kernel.elf"

slate.img:
	mkdir slate_image
	dd if=/dev/zero bs=1M count=0 seek=64 of=slate.img
	parted -s slate.img mklabel msdos
	parted -s slate.img mkpart primary 1 100%
	make $(FS)
	sudo rm -rf slate_image loopback_dev
	sudo ./boot/qloader2-install boot/qloader2.bin slate.img

ifndef $(FS)
FS := ext2
endif

ext2: ${KNL_TARGET}
	sudo losetup -Pf --show slate.img > loopback_dev
	sudo mkfs.ext2 `cat loopback_dev`p1
	sudo mount `cat loopback_dev`p1 slate_image/
	sudo mkdir slate_image/boot/
	sudo mkdir slate_image/modules/
	sudo cp ${KNL_TARGET} slate_image/boot/
	sudo cp boot/qloader2.cfg slate_image/boot/
	sync
	sudo umount slate_image/
	sudo losetup -d `cat loopback_dev`

echfs: ${KNL_TARGET}
	echfs-utils -g -p1 slate.img quick-format 512
	echfs-utils -g -p1 slate.img import ${KNL_TARGET} ${KNL_TARGET}
	echfs-utils -g -p1 slate.img import boot/qloader2.cfg boot/qloader2.cfg

${KNL_TARGET}: ${R_SOURCES:.real=.bin} ${OBJ} symlist
	${LD} ${LDFLAGS} ${OBJ} sys/symlist.o -o $@
	./gensyms.sh
	${CC} -x c ${CFLAGS} -c sys/symlist.gen -o sys/symlist.o
	${LD} ${LDFLAGS} ${OBJ} sys/symlist.o -o $@

symlist:
	echo '#include <sys/symlist.h>' > sys/symlist.gen
	echo 'struct symlist_t symlist[] = {{0xffffffffffffffff,""}};' >> sys/symlist.gen
	${CC} -x c ${CFLAGS} -c sys/symlist.gen -o sys/symlist.o

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@

%.o: %.asm
	nasm -f elf64 -F dwarf -g -o $@ $<

clean:
	rm ${OBJ} dump.log slate.img
	find . -type f -name '*.elf' -delete

.PHONY: all ci clean
