
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                               load.inc
; 						      wzq
;                                                     2013/5/21
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


;软盘特征
SecPerTrk		equ	 18		; 每磁道扇区数
NumHeads		equ	 2		; 磁头数(面数)
BytsPerSec		equ	 512		; 每扇区字节数
DrvNum			equ	 0		; 中断 13 的驱动器号

;loader和kernel占据扇区大小和扇区偏移
LOADER_SECTS		equ	 16     	; loader占据16个扇区
LOADER_SECTS_Offset	equ	 1
KERNEL_SECTS  		equ 	 128		; kernel占据128个扇区 
KERNEL_SECTS__Offset  	equ 	 17

;loader和kernel加载到内存中的位置
BaseOfLoader		equ   	 09000h		     ; LOADER.BIN 被加载到的位置 ---- 段地址
OffsetOfLoader		equ	 0100h		     ; LOADER.BIN 被加载到的位置 ---- 偏移地址
BaseOfLoaderPhyAddr	equ	 BaseOfLoader * 10h  ; LOADER.BIN 被加载到的位置 ---- 物理地址

BaseOfKernel    	equ   	 01000h	      	     ; KERNEL.BIN 被加载到的位置 ---- 段地址
OffsetOfKernel  	equ	 0h	  	     ; KERNEL.BIN 被加载到的位置 ---- 偏移地址
BaseOfKernelPhyAddr	equ	 BaseOfKernel * 10h  ; KERNEL.BIN 被加载到的位置 ---- 物理地址

;页目录与页表在内存中的位置
PageDirBase		equ	 0		; 页目录开始地址: 0
PageTblBase		equ	 100000h	; 页表开始地址:   1M

;GDT TABLE和IDT TABLE在内存中的位置
GDT_AMT				equ   	 256
IDT_AMT				equ   	 256
BaseOfIDTtable			equ   	 0100h		     ; IDT TABLE ---- 段地址
BaseOfGDTtable			equ   	 0180h		     ; GDT TABLE ---- 段地址
BaseOfIDTtablePhyAddr		equ   	 01000h		     ; IDT TABLE ---- 物理地址
BaseOfGDTtablePhyAddr		equ   	 01800h		     ; GDT TABLE ---- 物理地址


;内存大小
MemSize:		equ	 2000000h	;内存大小 32M

