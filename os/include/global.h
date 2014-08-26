
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            global.h
						    wzq
                                                    2013/5/21
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*those variables were defined in global.c*/

#ifndef	_GLOBAL_H
#define _GLOBAL_H


#ifndef   EXTERN
#define	  EXTERN   extern
#endif


EXTERN   int	disp_pos;
EXTERN   int	locker;       //掌锁者......
EXTERN   int	sys_ticks;

//该指针指向即将要运行的进程
EXTERN   PROCESS*	p_proc_ready;
EXTERN   int	        proc_ready_amt;
EXTERN   PROCESS*	proc_blocked_que;	/* 阻塞队列指针 */

EXTERN   DESCRIPTOR    *gdt;		        /*GDT  pointer*/
EXTERN   GATE	       *idt;		        /*IDT  pointer*/

#define  TSS_AMT	128
EXTERN   TSS	        tss[TSS_AMT];		/*TSS*/

/* Amount of servers/users/processes */
#define  SERV_AMT		3
#define  USER_AMT		3
#define  PROC_AMT	        SERV_AMT+USER_AMT            /* PROC_AMT=SERV_AMT+USER_AMT */
#define  SERVER_SYS_ID		0                            /*servers/users在process table中的索引*/
#define  SERVER_TTY_ID		1                            /*servers/users在process table中的索引*/
#define  SERVER_HD_ID		2                            /*servers/users在process table中的索引*/
#define  SERVER_FS_ID		3                            /*servers/users在process table中的索引*/
#define  USERA_ID               SERV_AMT
#define  USERB_ID               SERV_AMT+1
#define  USERC_ID               SERV_AMT+2

EXTERN   TASK	   	   server_table[SERV_AMT];		  /*server table*/
EXTERN   TASK	   	   user_table[USER_AMT];		  /*user table*/

EXTERN   PROCESS   	   proc_table[PROC_AMT];		  /*process table*/
EXTERN   PROCESS*   	   proc_index_table[PROC_AMT];		  /*process index  table*/

/* stack size of kernel(目前只有一个内核栈,我想目前够了) */
#define  KRNL_STACK_SIZE	  0x8000
EXTERN   char	   	   krnl_stacks[KRNL_STACK_SIZE * PROC_AMT]; /*kernel stack*/

/* stacks of servers/users（系统）*/
#define  STACK_SIZE_SERV_SYS	  0x8000
#define  STACK_SIZE_SERV_TTY	  0x8000
#define  STACK_SIZE_SERV_HD	  0x8000
#define  STACK_SIZE_SERV_FS	  0x8000
#define  STACK_SIZE_USERA	  0x8000
#define  STACK_SIZE_USERB	  0x8000
#define  STACK_SIZE_USERC	  0x8000
//我想应该把他们分开,以后分！！！！！！！！！！!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define  STACK_SIZE_TOTAL	(STACK_SIZE_SERV_SYS +   \
				 STACK_SIZE_SERV_TTY +   \
				 STACK_SIZE_SERV_HD  +   \
                                 STACK_SIZE_SERV_FS  +   \
                                 STACK_SIZE_USERA +      \
                                 STACK_SIZE_USERB +      \
                                 STACK_SIZE_USERC)
EXTERN   char	   	task_stacks[STACK_SIZE_TOTAL];		/*task stacks*/

/* amount of IRQs */
#define	IRQS_AMT	16
/*IRQ在irq_table中的索引，我们目前用到这几个*/	
#define	IRQ0_CLOCK	 0
#define	IRQ1_KEYBOARD	 1
#define	IRQ2_CASCADE	 2	/* cascade enable for 2nd AT controller */
#define	IRQ3_ETHER	 3	/* default ethernet interrupt vector */
#define	IRQ3_RS232_2	 3	/* RS232 interrupt vector for port 2 */
#define	IRQ4_RS232_1	 4	/* RS232 interrupt vector for port 1 */
#define	IRQ5_XT_WIN	 5	/* XT winchester */
#define	IRQ6_FLOPPY	 6	/* floppy disk */
#define	IRQ7_PRINTER	 7
#define	IRQ14_AT_WIN	14	/* AT winchester */
EXTERN   irq_handler	irq_table[IRQS_AMT];		/*irq  table*/

/* Amount of system call */
#define  SYS_CALL_AMT	 16
/*以下常量是sys_call_table中函数向量.*/
/*注意：在k_const.inc应该有备份*/
#define  NB_sendrecv	  0                             //NB是number的缩写，不是...
EXTERN	 system_call	 sys_call_table[SYS_CALL_AMT];	/*system call table*/

/* 下面的这个数据不应该放在内核 */
#define  MSG_ERR_AMT	 10
EXTERN	 char * msg_err_from_krnl[MSG_ERR_AMT];

#endif
