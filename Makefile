default: all

.PHONY: all clean

C_SOURCES = $(shell find . -type f -name '*.c' | grep -v "modules")
H_SOURCES = $(shell find . -type f -name '*.h' | grep -v "modules")
A_SOURCES = $(shell find . -type f -name '*.asm' | grep -v "modules")

OBJ = ${C_SOURCES:.c=.o} ${A_SOURCES:.asm=.o}

ARCH = x86_64

CC = clang
LD = gcc
AS = nasm

BUILD_DIR = build

KNL_TARGET = ${BUILD_DIR}/kslate.elf

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

all: clean
	mkdir ${BUILD_DIR}
	make ${KNL_TARGET}
	mv **/*.o ${BUILD_DIR}

${KNL_TARGET}: ${OBJ} symlist
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
	rm -rf ${BUILD_DIR}
