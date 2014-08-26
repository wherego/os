
######################
# Makefile for tools #
######################


#boot占据1个扇区
BOOT_SECTS      =    1
#loader占据12个扇区
LOADER_SECTS    =   16
#kernel占据128个扇区
KERNEL_SECTS    =  128

#BOOT_SECTS+LOADER_SECTS+KERNEL_SECTS
SECTS_IN_ALL    =  145

CC		=  gcc
CFLAGS		= -m32

TARGET		= Image
TOOLS		= build printdir
# All Phony Targets
.PHONY : all clean Image bin

# Default starting position
all : $(TARGET)

clean :
	rm -f $(TARGET) $(TOOLS) bochsout.txt kernel.elf kernel.bin

Image : kernel.bin $(TOOLS)
	./build ../boot/boot.bin ../boot/loader.bin ./kernel.bin > Image
	sync
	dd if=Image of=floppy.img bs=512 count=$(SECTS_IN_ALL) conv=notrunc

kernel.bin :
	objcopy -I elf32-i386 -O binary -S ./kernel.elf kernel.bin

build : build.c
	$(CC) $(CFLAGS) -o $@ $<

printdir : printdir.c
	$(CC) $(CFLAGS) -o $@ $<
