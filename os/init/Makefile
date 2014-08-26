
#####################
# Makefile for init #
#####################

# Programs, flags, etc.
ASM		= nasm
LD		= ld
CC		= gcc
ASMFLAGS	= -I ../include/ -f elf
#由于我使用的系统是64位，ld默认生成的文件格式是elf64-x86-64,因此多了一个-m参数。
LDFLAGS		= -s -m elf_i386 -Ttext $(ENTRYPOINT)
CFLAGS		= -m32 -I ../include/ -I include/ -c -fno-builtin -fno-stack-protector

OBJS		= init.o init_TABLE.o init_GDT.o init_IDT.o                 \
                  init_8259A.o init_8254.o global.o

# All Phony Targets
.PHONY : all clean

# Default starting position
all : $(OBJS)

clean :
	rm -f $(OBJS)

init.o : init.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

init_TABLE.o : init_TABLE.c
	$(CC) $(CFLAGS) -o $@ $<

init_GDT.o : init_GDT.c
	$(CC) $(CFLAGS) -o $@ $<

init_IDT.o : init_IDT.c
	$(CC) $(CFLAGS) -o $@ $<

init_8259A.o : init_8259A.c
	$(CC) $(CFLAGS) -o $@ $<

init_8254.o : init_8254.c
	$(CC) $(CFLAGS) -o $@ $<

global.o : global.c
	$(CC) $(CFLAGS) -o $@ $<

