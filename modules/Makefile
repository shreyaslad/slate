C_SOURCES = $(shell find . -type f -name '*.c')

OBJ = ${C_SOURCES:.c=.o}

ARCH = x86_64
CC = clang -target ${ARCH}-unknown-none
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

modules:  ${OBJ}

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@