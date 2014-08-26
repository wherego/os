#####################################################
#              open source OS--wzq                  #
#                                      wzq          #
#                                      2013/5/21    #
#####################################################

###################
# Makefile of wzq #
###################

ENTRYPOINT	= 0x10000

# Programs, flags, etc.
#目前使用的是nasm,它编译产生的是32位的目标代码
ASM		= nasm
LD		= ld
CC		= gcc

ASMFLAGS	= -I include/ -f elf
LDFLAGS		= -s -m elf_i386 -Ttext $(ENTRYPOINT)
CFLAGS		= -m32 -I include/ -c -fno-builtin -fno-stack-protector

BOOT	        = boot/boot.bin boot/loader.bin
KERNEL	        = ./tools/kernel.elf

OBJS_init	= init/init.o init/init_TABLE.o init/init_GDT.o init/init_IDT.o    \
                  init/init_8259A.o init/init_8254.o init/global.o

OBJS_kernel	= kernel/idt.o kernel/restart.o kernel/IDT.o kernel/IPC.o          \
                  kernel/scheduler.o kernel/syscall.o                              \
		  kernel/server.o  kernel/user.o

OBJS_drivers	= drivers/drv_local.o drivers/hard_disk.o drivers/keyboard.o

OBJS_lib	= lib/k_lib.o lib/K_LIB.o lib/string.o lib/port.o lib/disp.o

OBJS_mm 	= mm/memory.o

OBJS_fs 	= fs/fs.o

OBJS		= $(OBJS_init) $(OBJS_kernel) $(OBJS_drivers) $(OBJS_lib) $(OBJS_mm) \
                  $(OBJS_fs)

# All Phony Targets
.PHONY : all clean image simulate

# Default starting position
all :   clean image

clean :
	cd boot;    make clean
	cd drivers; make clean
	cd fs;      make clean
	cd init;    make clean
	cd kernel;  make clean
	cd lib;     make clean
	cd mm;      make clean
	cd tools;   make clean

	rm -f *~
	cd boot;    	    rm -f *~
	cd boot/include;    rm -f *~
	cd drivers; 	    rm -f *~
	cd drivers/include; rm -f *~
	cd fs;      	    rm -f *~
	cd fs/include;      rm -f *~
	cd include; 	    rm -f *~
	cd init;    	    rm -f *~
	cd init/include;    rm -f *~
	cd kernel;  	    rm -f *~
	cd kernel/include;  rm -f *~
	cd lib;             rm -f *~
	cd lib/include;     rm -f *~
	cd mm;              rm -f *~
	cd mm/include;      rm -f *~
	cd tools;           rm -f *~

image : $(BOOT) $(KERNEL)
	cd tools;   make

simulate :
	cd tools;   bochs

#BOOT文件夹包含生成的boot.bin和loader.bin
$(BOOT) :
	cd boot;    make

#tools目录包含生成的kernel.bin
$(KERNEL) : $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

$(OBJS_init) : 
	cd init;    make

$(OBJS_kernel) : 
	cd kernel;  make

$(OBJS_drivers) : 
	cd drivers; make

$(OBJS_lib) : 
	cd lib;     make

$(OBJS_mm) : 
	cd mm;      make

$(OBJS_fs) : 
	cd fs;      make


