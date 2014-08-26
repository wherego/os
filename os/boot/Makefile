#####################
# Makefile for boot #
#####################
ASM		= nasm
ASMFLAGS	= -I include/

BOOT      = boot.bin
LOADER    = loader.bin
# All Phony Targets
.PHONY : all clean

all : $(BOOT) $(LOADER)

clean :
	rm -f $(BOOT) $(LOADER)

boot.bin : boot.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

loader.bin : loader.asm
	$(ASM) $(ASMFLAGS) -o $@ $<


