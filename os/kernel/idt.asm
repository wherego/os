
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                               idt.asm
;						      wzq
;                                                     2013/6/6
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;; 这里定义的是一些中断入口函数，调用相应的中断函数，在IDT.c中。之后，返回到这里,调用restart。
;; 中断发生时，会从levelx（x=1，2，3）跳到level0，esp的值变成TSS中预设好的值...

%include "../boot/include/load.inc"
%include "k_const.inc"

; 导入函数
extern	divide_error
extern	debug_exception
extern	nmi
extern	breakpoint_exception
extern	overflow
extern	bounds_check
extern	inval_opcode
extern	copr_not_available
extern	double_fault
extern	copr_seg_overrun
extern	inval_tss
extern	segment_not_present
extern	stack_exception
extern	general_protection
extern	page_fault
extern	math_error
extern	align_check
extern	machine_check
extern	SIMD_floatingpoint_exception

; 导出函数
global	_divide_error
global	_debug_exception
global	_nmi
global	_breakpoint_exception
global	_overflow
global	_bounds_check
global	_inval_opcode
global	_copr_not_available
global	_double_fault
global	_copr_seg_overrun
global	_inval_tss
global	_segment_not_present
global	_stack_exception
global	_general_protection
global	_page_fault
global	_math_error
global	_align_check
global	_machine_check
global	_SIMD_floatingpoint_exception

global	_irq0
global	_irq1
global	_irq2
global	_irq3
global	_irq4
global	_irq5
global	_irq6
global	_irq7
global	_irq8
global	_irq9
global	_irq10
global	_irq11
global	_irq12
global	_irq13
global	_irq14
global	_irq15

global	_sys_call

[SECTION .text]
; ====================================================================================
; 中断和异常 -- 异常
; ====================================================================================
_divide_error:
	push	0xFFFFFFFF	; no err code
	push	0		; vector_no	= 0
	call	divide_error
	add	esp, 4*2	; 让栈顶指向 EIP，堆栈中从顶向下依次是：EIP、CS、EFLAGS
	iretd			;

_debug_exception:
	push	0xFFFFFFFF	; no err code
	push	1		; vector_no	= 1
	call	debug_exception
	add	esp, 4*2	; 让栈顶指向 EIP，堆栈中从顶向下依次是：EIP、CS、EFLAGS
	iretd			;

_nmi:
	push	0xFFFFFFFF	; no err code
	push	2		; vector_no	= 2
	call	nmi
	add	esp, 4*2	; 让栈顶指向 EIP，堆栈中从顶向下依次是：EIP、CS、EFLAGS
	iretd			;

_breakpoint_exception:
	push	0xFFFFFFFF	; no err code
	push	3		; vector_no	= 3
	call	breakpoint_exception
	add	esp, 4*2	; 让栈顶指向 EIP，堆栈中从顶向下依次是：EIP、CS、EFLAGS
	iretd			;

_overflow:
	push	0xFFFFFFFF	; no err code
	push	4		; vector_no	= 4
	call	overflow
	add	esp, 4*2	; 让栈顶指向 EIP，堆栈中从顶向下依次是：EIP、CS、EFLAGS
	iretd			;

_bounds_check:
	push	0xFFFFFFFF	; no err code
	push	5		; vector_no	= 5
	call	bounds_check
	add	esp, 4*2	; 让栈顶指向 EIP，堆栈中从顶向下依次是：EIP、CS、EFLAGS
	iretd			;

_inval_opcode:
	push	0xFFFFFFFF	; no err code
	push	6		; vector_no	= 6
	call	inval_opcode
	add	esp, 4*2	; 让栈顶指向 EIP，堆栈中从顶向下依次是：EIP、CS、EFLAGS
	iretd			;

_copr_not_available:
	push	0xFFFFFFFF	; no err code
	push	7		; vector_no	= 7
	call	copr_not_available
	add	esp, 4*2	; 让栈顶指向 EIP，堆栈中从顶向下依次是：EIP、CS、EFLAGS
	iretd			;

_double_fault:
	push	8		; vector_no	= 8
	call	double_fault
	add	esp, 4*2	; 让栈顶指向 EIP，堆栈中从顶向下依次是：EIP、CS、EFLAGS
	iretd			;

_copr_seg_overrun:
	push	0xFFFFFFFF	; no err code
	push	9		; vector_no	= 9
	call	copr_seg_overrun
	add	esp, 4*2	; 让栈顶指向 EIP，堆栈中从顶向下依次是：EIP、CS、EFLAGS
	iretd			;

_inval_tss:
	push	10		; vector_no	= A
	call	inval_tss
	add	esp, 4*2	; 让栈顶指向 EIP，堆栈中从顶向下依次是：EIP、CS、EFLAGS
	iretd			;

_segment_not_present:
	push	11		; vector_no	= B
	call	segment_not_present
	add	esp, 4*2	; 让栈顶指向 EIP，堆栈中从顶向下依次是：EIP、CS、EFLAGS
	iretd			;

_stack_exception:
	push	12		; vector_no	= C
	call	stack_exception
	add	esp, 4*2	; 让栈顶指向 EIP，堆栈中从顶向下依次是：EIP、CS、EFLAGS
	iretd			;

_general_protection:
	push	13		; vector_no	= D
	call	general_protection
	add	esp, 4*2	; 让栈顶指向 EIP，堆栈中从顶向下依次是：EIP、CS、EFLAGS
	iretd			;

_page_fault:
	push	14		; vector_no	= E
	call	page_fault
	add	esp, 4*2	; 让栈顶指向 EIP，堆栈中从顶向下依次是：EIP、CS、EFLAGS
	iretd			;

_math_error:
	push	0xFFFFFFFF	; no err code
	push	16		; vector_no	= 10h
	call	math_error
	add	esp, 4*2	; 让栈顶指向 EIP，堆栈中从顶向下依次是：EIP、CS、EFLAGS
	iretd			;

_align_check:
	push	17		; vector_no	= 11h
	call	align_check
	add	esp, 4*2	; 让栈顶指向 EIP，堆栈中从顶向下依次是：EIP、CS、EFLAGS
	iretd			;

_machine_check:
	push	0xFFFFFFFF	; no err code
	push	18		; vector_no	= 12h
	call	machine_check
	add	esp, 4*2	; 让栈顶指向 EIP，堆栈中从顶向下依次是：EIP、CS、EFLAGS
	iretd			;

_SIMD_floatingpoint_exception:
	push	0xFFFFFFFF	; no err code
	push	19		; vector_no	= 13h
	call	SIMD_floatingpoint_exception
	add	esp, 4*2	; 让栈顶指向 EIP，堆栈中从顶向下依次是：EIP、CS、EFLAGS
	iretd			;
; ====================================================================================
;                              IRQ 
; 中断和异常 -- 硬件中断
; 8259A连接INTR，为可屏蔽中断,貌似没有ERROR CODE。
; ====================================================================================
%macro	int_irq_master	1
        cmp     dword [ss:locker], 0
        jne     LABEL_RET

        inc     dword [ss:locker]

	in	al, PORT_8259A_MAS_CTRLMASK	; \
	or	al, (1 << %1)			;  | 屏蔽当前中断
	out	PORT_8259A_MAS_CTRLMASK, al	; /
	
	mov	al, PORT_8259A_EOI		; \ 置EOI位
	out	PORT_8259A_MAS_CTRL, al		; /
	 
	sti				; CPU在响应中断的过程中会自动关中断，
                                	; 这句之后就允许响应新的中断 

	push	%1
	call	[ss:irq_table + 4 * %1]	;中断处理程序
	pop	ecx

	cli			        ; 关中断

	in	al, PORT_8259A_MAS_CTRLMASK	; \
	and	al, ~(1 << %1)			;  | 恢复接受当前中断
	out	PORT_8259A_MAS_CTRLMASK, al	; /

	jmp     restart
%endmacro
; ---------------------------------------------------------------------------------

ALIGN	16
_irq0:		; Interrupt routine for irq0  (clock)
 	int_irq_master	0

ALIGN	16
_irq1:		; Interrupt routine for irq 1 (keyboard)
	int_irq_master	1

ALIGN	16
_irq2:		; Interrupt routine for irq 2 (cascade!)
	int_irq_master	2

ALIGN	16
_irq3:		; Interrupt routine for irq 3 (second serial)
	int_irq_master	3

ALIGN	16
_irq4:		; Interrupt routine for irq 4 (first serial)
	int_irq_master	4

ALIGN	16
_irq5:		; Interrupt routine for irq 5 (XT winchester)
	int_irq_master	5

ALIGN	16
_irq6:		; Interrupt routine for irq 6 (floppy)
	int_irq_master	6

ALIGN	16
_irq7:		; Interrupt routine for irq 7 (printer)
	int_irq_master	7

; ---------------------------------------------------------------------------------
%macro	int_irq_slave 	1
        cmp     dword [ss:locker], 0
        jne     LABEL_RET

        inc     dword [ss:locker]

	in	al, PORT_8259A_SLA_CTRLMASK	; \
	or	al, (1 << (%1 - 8))		;  | 屏蔽当前中断
	out	PORT_8259A_SLA_CTRLMASK, al	; /

	mov	al, PORT_8259A_EOI
	out	PORT_8259A_MAS_CTRL, al		; 置master EOI位
	out	PORT_8259A_SLA_CTRL, al		; 置slave EOI位

	sti				; CPU在响应中断的过程中会自动关中断，
	                                ; 这句之后就允许响应新的中断

	push	%1			; \
	call	[ss:irq_table + 4 * %1]	;  | 中断处理程序
	pop	ecx			; /

	cli			        ; 关中断

	in	al, PORT_8259A_SLA_CTRLMASK	; \
	and	al, ~(1 << (%1 - 8))		;  | 恢复接受当前中断
	out	PORT_8259A_SLA_CTRLMASK, al	; /

	jmp     restart
%endmacro
; ---------------------------------------------------------------------------------

ALIGN	16
_irq8:		; Interrupt routine for irq 8 (realtime clock).
        int_irq_slave	8

ALIGN	16
_irq9:		; Interrupt routine for irq 9 (irq 2 redirected)
        int_irq_slave	9

ALIGN	16
_irq10:		; Interrupt routine for irq 10
        int_irq_slave	10

ALIGN	16
_irq11:		; Interrupt routine for irq 11
        int_irq_slave	11

ALIGN	16
_irq12:		; Interrupt routine for irq 12
        int_irq_slave	12

ALIGN	16
_irq13:		; Interrupt routine for irq 13 (FPU exception)
        int_irq_slave	13

ALIGN	16
_irq14:		; Interrupt routine for irq 14 (AT winchester)
        int_irq_slave	14

ALIGN	16
_irq15:		; Interrupt routine for irq 15
        int_irq_slave	15

; ====================================================================================
;                                 void _sys_call();
;大名鼎鼎的系统调用
;该函数的中断号是 INT_VEC_SYSCALL（0x80）
; ====================================================================================
_sys_call:
        sti        		;开中断

	;; 注意，我们现在的系统调用有三个参数，EAS存放系统调用函数的索引。
	;; 第一个参数是：进程指针，第二个参数是：FUNCTION，第三个是：消息指针
	push	ecx
	push	ebx
	push	dword [ss:p_proc_ready]
        call    [ss:sys_call_table + eax * 4]
	add	esp, 4 * 3

        cli			;关中断

	iretd
; =================================================================================
;				    restart
; =================================================================================
restart:
	xor	eax, eax
	
	mov	ebx, [ss:p_proc_ready]
	mov	eax, [ss:ebx + EXCUR_PROC_ID]
	mov	bx , 16
	mul	bx
	mov	ebx, SELECTOR_TSS_FIRST
	add	eax, ebx

	mov	bx, [ss:SEL_OF_GDT]

	cmp	ax, bx
	jz	LABEL_RET

	mov	[ss:SEL_OF_GDT], ax
	mov	ebx, ADDR_OFFSET
	dec	dword [ss:locker]

	jmp 	far  [ss:ebx]

LABEL_RET:
	iretd

[SECTION .data32]
ALIGN	32
ADDR_OFFSET:	dd	0
SEL_OF_GDT:	dw	0x28
