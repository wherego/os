
########################
# Makefile for drivers #
########################

ASM		= nasm
LD		= ld
CC		= gcc
ASMFLAGS	= -I ../include/ -f elf
#由于我使用的系统是64位，ld默认生成的文件格式是elf64-x86-64,因此多了一个-m参数。
INCLUDE_DIRS    = -I ../include/ -I include/ 

LDFLAGS		= -s -m elf_i386 -Ttext $(ENTRYPOINT)
CFLAGS		= -m32 $(INCLUDE_DIRS) -c -fno-builtin -fno-stack-protector

SOURCES = drv_local.c hard_disk.c keyboard.c
OBJECTS = $(SOURCES:.c=.o)

# All Phony Targets
.PHONY : all clean

# Default starting position
all : $(OBJECTS)

clean :
	rm -f $(OBJECTS)

drv_local.o : drv_local.c
	$(CC) $(CFLAGS) -o $@ $<

hard_disk.o : hard_disk.c
	$(CC)  $(CFLAGS)  -o $@ $<

keyboard.o : keyboard.c
	$(CC)  $(CFLAGS)  -o $@ $<
