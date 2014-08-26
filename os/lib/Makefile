####################
# Makefile for lib #
####################

# Programs, flags, etc.
ASM		= nasm
LD		= ld
CC		= gcc
ASMFLAGS	= -I ../include/ -f elf
#由于我使用的系统是64位，ld默认生成的文件格式是elf64-x86-64,因此多了一个-m参数。
LDFLAGS		= -s -m elf_i386 -Ttext $(ENTRYPOINT)
CFLAGS		= -m32 -I ../include/ -I include/ -c -fno-builtin -fno-stack-protector

OBJS	= k_lib.o K_LIB.o string.o port.o disp.o

# All Phony Targets
.PHONY : all clean

# Default starting position
all : $(OBJS)

clean :
	rm -f $(OBJS)

k_lib.o : k_lib.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

K_LIB.o : K_LIB.c
	$(CC)  $(CFLAGS)   -o $@ $<

string.o : string.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

port.o : port.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

disp.o : disp.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

