
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            global.c
						    wzq
                                                    2013/6/7
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*注意:下面定义的变量应该在global.h和k_const.inc中声明。*/
//这里面没有函数，为什么不是.h文件呢，因为变量在这里定义并且赋初值，我们不能让它们定义两次


#include "const.h"
#include "type.h"
#include "proto.h"
#include "global.h"

int   disp_pos  =  0;
int   locker    =  0;
int   sys_ticks =  0;

/*process pointer*/
PROCESS       *p_proc_ready;

/* process ready amount  */
int	      proc_ready_amt   = PROC_AMT;

/* 阻塞队列指针 */
PROCESS       *proc_blocked_que = NULL;	/* 阻塞队列指针 */

/*GDT  pointer*/
DESCRIPTOR    *gdt = (DESCRIPTOR *)GDT_BASE_ADDR;

/*IDT  pointer*/
GATE	      *idt = (GATE *)IDT_BASE_ADDR;

/*TSS*/
TSS	      tss[TSS_AMT];

/*server table*/
TASK	      server_table[SERV_AMT] = {{server_sys, STACK_SIZE_SERV_SYS, "server_sys"},
					{server_tty, STACK_SIZE_SERV_TTY, "server_tty"},
					{server_hd , STACK_SIZE_SERV_HD , "server_hd" },
					{server_fs , STACK_SIZE_SERV_FS , "server_fs" }};


/*task table*/
TASK	      user_table[USER_AMT] = {{TaskA, STACK_SIZE_USERA, "TaskA"},
                                      {TaskB, STACK_SIZE_USERB, "TaskB"},
				      {TaskC, STACK_SIZE_USERC, "TaskC"}};

/*process table*/
PROCESS	      proc_table[PROC_AMT];                     /*process table*/
PROCESS*      proc_index_table[PROC_AMT];		/*process index  table*/

/*kernel stack*/
char	      krnl_stacks[KRNL_STACK_SIZE * PROC_AMT];
/*task stacks*/
char	      task_stacks[STACK_SIZE_TOTAL];

/*irq table:这里已经赋值，我们需要做的就是中断使能*/
irq_handler   irq_table[IRQS_AMT] = {irq0,irq1,irq2 ,irq3 ,irq4 ,irq5 ,irq6 ,irq7,
                                     irq8,irq9,irq10,irq11,irq12,irq13,irq14,irq15};

/*system call table，以后就是在这里增加系统调用函数*/
system_call   sys_call_table[SYS_CALL_AMT] = {sys_sendrecv};

/*system call table，以后就是在这里增加系统调用函数*/
char * msg_err_from_krnl[MSG_ERR_AMT]={"Value of the function in question.",
				       "Value of the sender ID in question.",
				       "Value of the recver ID in question.",
				       "Recver do not receive your message.",
				       "There is no message in your queue.",
				       "You are sending the same message.",
				       "",
				       "",
				       "",
				       ""
};
