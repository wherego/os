
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                               k_const.inc
; 						      wzq
;                                                     2013/6/8
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


;PROCESS was defined in type.c.
;以下常量是PROCESS中元素的偏移
PROC_BASE		equ	0
EXCUR_PROC_ID		equ	PROC_BASE

;TSS was defined in type.c.
;以下常量是TSS中元素的偏移
TSS_BASE		equ	0
EXCUR_TSS_EIP		equ	TSS_BASE + 4*8
EXCUR_TSS_EFLAGS	equ	TSS_BASE + 4*9
EXCUR_TSS_ESP		equ	TSS_BASE + 4*14
EXCUR_TSS_ES		equ	TSS_BASE + 4*18
EXCUR_TSS_CS		equ	TSS_BASE + 4*19
EXCUR_TSS_SS		equ	TSS_BASE + 4*20
EXCUR_TSS_DS		equ	TSS_BASE + 4*21
EXCUR_TSS_FS		equ	TSS_BASE + 4*22
EXCUR_TSS_GS		equ	TSS_BASE + 4*23
EXCUR_TSS_LDT		equ	TSS_BASE + 4*24
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;those variables were defined in global.c
extern		disp_pos
extern		locker
extern		sys_ticks
extern		p_proc_ready
extern		proc_blocked_que

extern		tss
extern		kernel_stack
extern		task_stacks
extern		serer_table
extern		user_table

extern		proc_table
extern		proc_index_table
extern		irq_table	;irq  table
extern		sys_call_table	;system call table
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;以下常量必须与const.h/global.h/type.h保持一致
SELECTOR_DUMMY		equ		   0
SELECTOR_KRNL_CS	equ		0x08
SELECTOR_KRNL_DS	equ		0x10
SELECTOR_VIDEO		equ		(0x18+3)
SELECTOR_KRNL_GS	equ		SELECTOR_VIDEO
SELECTOR_LDT_FIRST	equ		0x20
SELECTOR_TSS_FIRST	equ		0x28

;中断向量
INT_VEC_SYSCALL		equ		0x80

;8259A interrupt controller ports.
PORT_8259A_MAS_CTRL     	equ	0x20   ; I/O port for interrupt controller         <Master>
PORT_8259A_MAS_CTRLMASK 	equ	0x21   ; setting bits in this port disables ints   <Master>
PORT_8259A_SLA_CTRL     	equ	0xA0   ; I/O port for second interrupt controller  <Slave>
PORT_8259A_SLA_CTRLMASK 	equ	0xA1   ; setting bits in this port disables ints   <Slave>
PORT_8259A_EOI			equ	0x20

;KRNL_StackTop 先这么写，以后改吧。
KRNL_StackTop		equ	0x10000


;sys_call_table was defined in global.c.
;以下常量是sys_call_table中函数向量
;注意：在global.h应该有备份
NB_sendrecv		equ	0
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



