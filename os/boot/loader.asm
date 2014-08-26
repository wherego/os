
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                               loader.asm
;                                                     wzq
;                                                     2013/5/21
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


%include	"load.inc"
%include	"pm.inc"

org  OffsetOfLoader

	jmp	LABEL_START

; GDT
;                                                  段基址     段界限,属性
LABEL_GDT:			Descriptor             0,        0, 0					   ; 空描述符
LABEL_DESC_CODE:		Descriptor             0,  0fffffh, DA_CR  | DA_DPL0 | DA_32 | DA_LIMIT_4K ; 0 ~ 4G
LABEL_DESC_DATA:		Descriptor             0,  0fffffh, DA_DRW | DA_DPL0 | DA_32 | DA_LIMIT_4K ; 0 ~ 4G
LABEL_DESC_VIDEO:		Descriptor	 0B8000h,   0ffffh, DA_DRW | DA_DPL3			   ; 显存首地址

GdtLen		equ	GDT_AMT * 8
GdtPtr		dw	GdtLen - 1				; 段界限
		dd	BaseOfGDTtablePhyAddr			; 基地址
; GDT 选择
SelectorCode	equ	LABEL_DESC_CODE	 - LABEL_GDT
SelectorData	equ	LABEL_DESC_DATA	 - LABEL_GDT
SelectorVideo	equ	LABEL_DESC_VIDEO - LABEL_GDT + SA_RPL3

; IDT
IdtLen		equ	IDT_AMT * 8
IdtPtr		dw	IdtLen - 1				; 段界限
		dd	BaseOfIDTtablePhyAddr			; 基地址

; 栈
BaseOfStack	equ	OffsetOfLoader

; 程序开始地址 
LABEL_START:
	mov	ax, cs
	mov	ds, ax
	mov	es, ax
	mov	ss, ax
	mov	sp, BaseOfStack

	mov	dh, 0			; "Load kernel"
	call	DispStrRealMode		; 显示字符串

 	mov	ax, BaseOfKernel        ; 加载内核
 	mov	es, ax
 	mov 	bx, OffsetOfKernel
 	mov	dx, KERNEL_SECTS
 	mov 	ax, KERNEL_SECTS__Offset
LOAD_KERNEL:
 	push	ax
 	push	dx
 	push	bx

 	mov	cl, 1
 	call	ReadSector

	pop	bx
 	pop 	dx
 	pop 	ax

 	add 	bx, 512
 	inc	ax
 	dec	dx
 	jnz	LOAD_KERNEL
KERNEL_LOADED:
	call	KillMotor		; 关闭软驱马达

	mov	dh, 1			; "Ready."
	call	DispStrRealMode		; 显示字符串

; 下面准备跳入保护模式 -------------------------------------------

; 加载 GDTR
LOAD_GDTR:
	mov	ax, BaseOfGDTtable        ; 加载内核
 	mov	es, ax
 	push	GdtLen
 	push	LABEL_GDT
 	push	0

 	call	MemCpy

	add 	sp, 6

	lgdt	[GdtPtr]

; 加载 IDTR
LOAD_IDTR:
	lidt	[IdtPtr]

; 关中断
	cli

; 打开地址线A20
	in	al, 92h
	or	al, 00000010b
	out	92h, al

; 准备切换到保护模式
	mov	eax, cr0
	or	eax, 1
	mov	cr0, eax

; 进入保护模式
	jmp	dword SelectorCode:(BaseOfLoaderPhyAddr+LABEL_PM_START)

;============================================================================
;字符串，为简化代码, 下面每个字符串的长度均为 MessageLength
MessageLength		equ	11
LoadMessage:		db	"Load kernel"
Message1		db	"Ready.     "
;============================================================================

;----------------------------------------------------------------------------
; 函数名: DispStrRealMode
;----------------------------------------------------------------------------
; 运行环境:
;	实模式（保护模式下显示字符串由函数 DispStr 完成）
; 作用:
;	显示一个字符串, 函数开始时 dh 中应该是字符串序号(0-based)
DispStrRealMode:
	mov	ax, MessageLength
	mul	dh
	add	ax, LoadMessage
	mov	bp, ax			; ┓
	mov	ax, ds			; ┣ ES:BP = 串地址
	mov	es, ax			; ┛
	mov	cx, MessageLength	; CX = 串长度
	mov	ax, 01301h		; AH = 13,  AL = 01h
	mov	bx, 0007h		; 页号为0(BH = 0) 黑底白字(BL = 07h)
	mov	dl, 0
	add	dh, 3			; 从第 3 行往下显示
	int	10h			; int 10h
	ret
;----------------------------------------------------------------------------
; 函数名: ReadSector
;----------------------------------------------------------------------------
; 作用:
;	从序号(Directory Entry 中的 Sector 号)为 ax 的 Sector 开始(第一个扇区从0记起), 将 cl 个 Sector 读入 es:bx 中
ReadSector:
	; -----------------------------------------------------------------------
	; 怎样由扇区号求扇区在磁盘中的位置 (扇区号 -> 柱面号, 起始扇区, 磁头号)
	; -----------------------------------------------------------------------
	; 设扇区号为 x
	;                          ┌ 柱面号 = y >> 1
	;       x           ┌ 商 y ┤
	; -------------- => ┤      └ 磁头号 = y & 1
	;  每磁道扇区数     │
	;                   └ 余 z => 起始扇区号 = z + 1
	push	bp
	mov	bp, sp
	sub	esp, 2			; 辟出两个字节的堆栈区域保存要读的扇区数: byte [bp-2]

	mov	byte [bp-2], cl
	push	bx			; 保存 bx
	mov	bl, SecPerTrk        	; bl: 除数
	div	bl			; y 在 al 中, z 在 ah 中
	inc	ah			; z ++
	mov	cl, ah			; cl <- 起始扇区号
	mov	dh, al			; dh <- y
	shr	al, 1			; y >> 1 (其实是 y/BPB_NumHeads, 这里BPB_NumHeads=2)
	mov	ch, al			; ch <- 柱面号
	and	dh, 1			; dh & 1 = 磁头号
	pop	bx			; 恢复 bx
	; 至此, "柱面号, 起始扇区, 磁头号" 全部得到 ^^^^^^^^^^^^^^^^^^^^^^^^
	mov	dl, DrvNum  		; 驱动器号 (0 表示 A 盘)
.GoOnReading:
	mov	ah, 2			; 读
	mov	al, byte [bp-2]		; 读 al 个扇区
	int	13h
	jc	.GoOnReading		; 如果读取错误 CF 会被置为 1, 这时就不停地读, 直到正确为止

	add	esp, 2
	pop	bp

	ret

;----------------------------------------------------------------------------
; 函数名: KillMotor
;----------------------------------------------------------------------------
; 作用:
;	关闭软驱马达
KillMotor:
	push	dx
	mov	dx, 03F2h
	mov	al, 0
	out	dx, al
	pop	dx
	ret
;----------------------------------------------------------------------------

; ------------------------------------------------------------------------
; 内存拷贝，仿 memcpy
; ------------------------------------------------------------------------
; void MemCpy(void* es:Dest, void* ds:Src, int Size);
; ------------------------------------------------------------------------
MemCpy:
	push	bp
	mov	bp, sp

	push	si
	push	di

	mov	di, [bp + 4]	; Destination
	mov	si, [bp + 6]	; Source
	mov	cx, [bp + 8]	; Counter
.1:
	cmp	cx, 0		; 判断计数器
	jz	.2		; 计数器为零时跳出

	mov	al, [ds:si]		; ┓
	inc	si			; ┃
					; ┣ 逐字节移动
	mov	byte [es:di], al	; ┃
	inc	di			; ┛

	dec	cx		; 计数器减一
	jmp	.1		; 循环
.2:
	pop	di
	pop	si

	mov	sp, bp
	pop	bp

	ret			; 函数结束，返回
; MemCpy 结束-------------------------------------------------------------


; 从此以后的代码在保护模式下执行 ----------------------------------------------------
; 32 位代码段. 由实模式跳入 ---------------------------------------------------------
[SECTION .s32]
ALIGN	32
[BITS	32]

LABEL_PM_START:
	mov	ax, SelectorVideo
	mov	gs, ax
	mov	ax, SelectorData
	mov	ds, ax
	mov	es, ax
	mov	fs, ax
	mov	ss, ax
	mov	esp, TopOfStack

	call	SetupPaging

	mov	ax, SelectorData
	mov	ds, ax
	mov	es, ax
	mov	fs, ax
	mov	ss, ax

	jmp	SelectorCode:BaseOfKernelPhyAddr	; 进入内核!!!

; 启动分页机制 --------------------------------------------------------------
SetupPaging:
	; 根据内存大小计算应初始化多少PDE以及多少页表
	xor	edx, edx
	mov	eax, MemSize
	mov	ebx, 400000h	; 400000h = 4M = 4096 * 1024, 一个页表对应的内存大小
	div	ebx
	mov	ecx, eax	; 此时 ecx 为页表的个数，也即 PDE 应该的个数
	test	edx, edx
	jz	.no_remainder
	inc	ecx		; 如果余数不为 0 就需增加一个页表
.no_remainder:
	push	ecx		; 暂存页表个数

	; 为简化处理, 所有线性地址对应相等的物理地址. 并且不考虑内存空洞.

	; 首先初始化页目录
	mov	ax, SelectorData
	mov	es, ax
	mov	edi, PageDirBase	; 此段首地址为 PageDirBase
	xor	eax, eax
	mov	eax, PageTblBase | PG_P | PG_RWW | PG_USU
.1:
	stosd
	add	eax, 4096		; 为了简化, 所有页表在内存中是连续的.
	loop	.1

	; 再初始化所有页表
	pop	eax			; 页表个数
	mov	ebx, 1024		; 每个页表 1024 个 PTE
	mul	ebx
	mov	ecx, eax		; PTE个数 = 页表个数 * 1024
	mov	edi, PageTblBase	; 此段首地址为 PageTblBase
	xor	eax, eax
	mov	eax, PG_P | PG_RWW | PG_USU
.2:
	stosd
	add	eax, 4096		; 每一页指向 4K 的空间
	loop	.2

	mov	eax, PageDirBase
	mov	cr3, eax
	mov	eax, cr0
	or	eax, 80000000h
	mov	cr0, eax
	jmp	short .3
.3:
	nop

	ret
; 分页机制启动完毕 ----------------------------------------------------------


; SECTION .data32
[SECTION .data32]
ALIGN	32

LABEL_DATA32:
StackSpace:	times	1000h	db	0	; 堆栈
TopOfStack	equ	BaseOfLoaderPhyAddr + $	; 栈顶
; SECTION .data32

