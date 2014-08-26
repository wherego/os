
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                               init.asm
; 						      wzq
;                                                     2013/5/21
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


%include "k_const.inc"

; 导入函数
extern	init_gdt
extern	init_idt
extern  init_8259A
extern  init_8254
extern	init_table

; 导出函数
global _start

[section .text]
; ====================================================================================
;                                   _start
; ====================================================================================
_start:
	mov	esp, KRNL_StackTop

	call	init_gdt
	call	init_idt
	call    init_8259A      ;初始化8259A
	call	init_8254       ;初始化时钟
	call	init_table

	jmp	SELECTOR_KRNL_CS:init_end

; 让第一个进程开始运行
init_end:
	xor	eax, eax

	mov	ax,  SELECTOR_TSS_FIRST
	ltr	ax

	mov	ax,  SELECTOR_LDT_FIRST
	lldt	ax

	mov	eax, [proc_index_table]
	mov	[p_proc_ready], eax

	mov	ax,  [tss + EXCUR_TSS_SS]
	push	eax
	mov	eax, [tss + EXCUR_TSS_ESP]
	push	eax
	mov	eax, [tss + EXCUR_TSS_EFLAGS]
	push	eax
	mov	ax,  [tss + EXCUR_TSS_CS]
	push	eax
	mov	eax, [tss + EXCUR_TSS_EIP]
	push	eax

	xor	eax, eax
	mov	ax,  [tss + EXCUR_TSS_DS]
	push	eax
	mov	ax,  [tss + EXCUR_TSS_ES]
	push	eax
	mov	ax,  [tss + EXCUR_TSS_FS]
	push	eax
	mov	ax,  [tss + EXCUR_TSS_GS]
	push	eax

	pop	gs
	pop	fs
	pop	es
	pop	ds

	iretd