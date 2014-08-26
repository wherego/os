
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            const.h
						    wzq
                                                    2013/5/21
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//把它们写在一块，我相当后悔

#ifndef	_CONST_H
#define _CONST_H

/* EXTERN */
#define EXTERN  extern

/* 函数类型 */
#define	PUBLIC			/* PUBLIC is the opposite of PRIVATE */
#define	PRIVATE	static		/* PRIVATE x limits the scope of x */

/* Boolean */
//下面的常量应该作为条件判断
#define	TRUE	1
#define	FALSE	0
//下面的常量应该作为条件判断
#define OK      1
#define NOT_ON  0               //NOT_ON的意思是办不到

/* NULL，用它给指针赋初值 */
#define	NULL	((void *)0)


/* IPC会用到的常量 */
#define SEND		1
#define RECEIVE		2

/* GDT 和 IDT 中描述符的个数 */
#define	 GDT_AMT	256
#define	 IDT_AMT	256

#define  GDT_BASE_ADDR          0x1800
#define  IDT_BASE_ADDR          0x1000
#define  PD_BASE_ADDR           0x0             //page directory


/* GDT 描述符索引 */
#define	INDEX_DUMMY		0	// ┓
#define	INDEX_KRNL_CS		1	// ┣ LOADER 里面已经确定了的.
#define	INDEX_KRNL_DS		2	// ┃
#define	INDEX_VIDEO		3	// ┛
#define	INDEX_TSS		4
#define	INDEX_LDT_FIRST		5

/* GDT 选择子 */
#define	SELECTOR_DUMMY		   0		// ┓
#define	SELECTOR_KRNL_CS	0x08		// ┣ LOADER 里面已经确定了的.
#define	SELECTOR_KRNL_DS	0x10		// ┃
#define	SELECTOR_VIDEO		(0x18+3)	// ┛<-- RPL=3
#define	SELECTOR_KRNL_GS	SELECTOR_VIDEO

#define SELECTOR_LDT_FIRST	0x20
#define	SELECTOR_TSS_FIRST	0x28	        // TSS

/* 中断向量 */
#define	INT_VEC_DividERR_FAULT			0x0
#define	INT_VEC_Debug_FAULT			0x1
#define	INT_VEC_NMI_INTERRUPT			0x2
#define	INT_VEC_Breakpoint_TRAP			0x3
#define	INT_VEC_Overflow_TRAP			0x4
#define	INT_VEC_Boundcheck_FAULT		0x5
#define	INT_VEC_InvalidOpcode_FAULT		0x6
#define	INT_VEC_DeviceNot_FAULT			0x7
#define	INT_VEC_DoubleFAU_ABORT			0x8
#define	INT_VEC_CoprocessorSEG_FAULT		0x9
#define	INT_VEC_InvalidTSS_FAULT		0xA
#define	INT_VEC_SEGNotPresent_FAULT		0xB
#define	INT_VEC_StackSEG_FAULT			0xC
#define	INT_VEC_ProtectionERR_FAULT		0xD
#define	INT_VEC_PageFAU_FAULT			0xE
#define	INT_VEC_Reserved			0xF
#define	INT_VEC_MathERR_FAULT			0x10
#define	INT_VEC_AlignCheck_FAULT		0x11
#define	INT_VEC_MachineCheck_ABORT		0x12
#define	INT_VEC_SIMD_FAULT			0x13
#define	INT_VEC_IRQ0				0x20	/* 8259A中断向量 */
#define	INT_VEC_IRQ8				0x28	/* 8259A中断向量 */
#define	INT_VEC_SYSCALL				0x80	/* system call */


/* 8259A interrupt controller. */
#define PORT_8259A_MAS_CTRL     	0x20 /* I/O port for interrupt controller       <Master> */
#define PORT_8259A_MAS_CTRLMASK 	0x21 /* setting bits in this port disables ints <Master> */
#define PORT_8259A_SLA_CTRL     	0xA0 /* I/O port for second interrupt controller<Slave>  */
#define PORT_8259A_SLA_CTRLMASK 	0xA1 /* setting bits in this port disables ints <Slave>  */
#define PORT_8259A_EOI			0x20


/* 8253/8254 PIT(Programmable Interval Timer). */
#define PORT_8254_COUNTER0         0x40 	/* I/O port for 8254 counter 0 */
#define PORT_8254_COUNTER1         0x41 	/* I/O port for 8254 counter 1 */
#define PORT_8254_COUNTER2         0x42 	/* I/O port for 8254 counter 2 */
#define PORT_8254_MODE_CTRLREG     0x43 	/* I/O port for 8254 mode control register*/
#define TIMER_8254_FREQ     	   1193182L 	/* 1193182Hz */
#define CLOCK_FREQ          	   100      	/* clock freq (100Hz) */


/************************************************************************/
/*                                VGA                                   */
/************************************************************************/
#define	PORT_VGA_CRT_CRTL_ADDR	0x3D4	/* CRT Controller Registers - Addr Register */
#define	PORT_VGA_CRT_CRTL_DATA	0x3D5	/* CRT Controller Registers - Data Register */
#define	INDEX_START_ADDR_HIGH	0xC	/* reg index of video mem start addr (MSB) */
#define	INDEX_START_ADDR_LOW	0xD	/* reg index of video mem start addr (LSB) */
#define	INDEX_CURSOR_HIGH	0xE	/* reg index of cursor position (MSB) */
#define	INDEX_CURSOR_LOW	0xF	/* reg index of cursor position (LSB) */

#define	VGA_MEM_BASE		0xB8000		/* base of color video memory */
#define	VGA_MEM_SIZE		0x8000		/* 32K: B8000H -> BFFFFH */

/* Color */
/*
 * e.g. MAKE_COLOR(BLUE, RED)
 *      MAKE_COLOR(BLACK, RED) | BRIGHT
 *      MAKE_COLOR(BLACK, RED) | BRIGHT | FLASH
 */
#define BLACK   0x0     /* 0000 */
#define WHITE   0x7     /* 0111 */
#define RED     0x4     /* 0100 */
#define GREEN   0x2     /* 0010 */
#define BLUE    0x1     /* 0001 */
#define FLASH   0x80    /* 1000 0000 */
#define BRIGHT  0x08    /* 0000 1000 */
#define MAKE_COLOR(x,y) (x | y) /* MAKE_COLOR(Background,Foreground) */



#endif













