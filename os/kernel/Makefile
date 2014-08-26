#######################
# Makefile for kernel #
#######################

# Programs, flags, etc.
ASM		= nasm
AS		= as
LD		= ld
CC		= gcc
ASMFLAGS	= -I ../include/ -f elf
ASFLAGS		= --32
#由于我使用的系统是64位，ld默认生成的文件格式是elf64-x86-64,因此多了一个-m参数。
LDFLAGS		= -s -m elf_i386 -Ttext $(ENTRYPOINT)
CFLAGS		= -m32 -I ../include/ -I include/ -c -fno-builtin -fno-stack-protector

OBJS	= idt.o restart.o IDT.o IPC.o scheduler.o server.o syscall.o user.o

# All Phony Targets
.PHONY : all clean

# Default starting position
all : $(OBJS)

clean :
	rm -f $(OBJS)

restart.o : restart.S
	$(AS) $(ASFLAGS) -o $@ $<

idt.o : idt.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

IDT.o : IDT.c
	$(CC) $(CFLAGS) -o $@ $<

IPC.o : IPC.c
	$(CC) $(CFLAGS) -o $@ $<

scheduler.o : scheduler.c
	$(CC) $(CFLAGS) -o $@ $<

server.o : server.c
	$(CC) $(CFLAGS) -o $@ $<

syscall.o : syscall.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

user.o : user.c
	$(CC) $(CFLAGS) -o $@ $<
