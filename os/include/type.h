
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                type.h
				                          wzq
							  2013/5/21
  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef	_TYPE_H
#define _TYPE_H

typedef	unsigned long long    u64;
typedef	unsigned int	      u32;
typedef	unsigned short	      u16;
typedef	unsigned char	      u8;

typedef	long long    s64;
typedef	int	     s32;
typedef	short	     s16;
typedef	char	     s8;

/* 函数指针 */
typedef	void	(*int_handler)	();            //可能有错！！！！！！！！！！！！！！！！！！！
typedef	void	(*irq_handler)	(int irq_nb);  /* irq_nb=(0,15) */
typedef	void	(*task_func)	();            /* server/user的函数体指针 */

typedef void   *system_call;

/*========================================================================
                              DESCRIPTOR
  ========================================================================*/
/* 描述符类型值说明 */
/* 注意：描述符类型值为16位，以下常量按16位定义。*/
#define	SEG_32			0x4000	/* 32 位段				*/
#define	LIMIT_4K		0x8000	/* 段界限粒度为 4K 字节			*/
#define PRES			0x80    /* 存在					*/
#define	DPL0			0x00	/* DPL = 0				*/
#define	DPL1			0x20	/* DPL = 1				*/
#define	DPL2			0x40	/* DPL = 2				*/
#define	DPL3			0x60	/* DPL = 3				*/
#define	DPL_KRNL		DPL0
#define	DPL_SERV		DPL1
#define	DPL_USER		DPL3
/* 存储段描述符类型值说明 */
#define	Data_R			0x10	/* 只读数据段类型值		*/
#define	Data_RW			0x12	/* 可读写数据段属性值		*/
#define	Data_RWA		0x13	/* 已访问可读写数据段类型值	*/
#define	Code			0x18	/* 只执行代码段属性值		*/
#define	Code_R			0x1A	/* 可执行可读代码段属性值   	*/
#define	Code_Cc			0x1C	/* 只执行一致代码段属性值		*/
#define	Code_CcR		0x1E	/* 可执行可读一致代码段属性值	*/
/* 系统段描述符类型值说明 */
#define	LDT			0x2	/* 局部描述符表段类型值			*/
#define	TaskGate		0x5	/* 任务门类型值				*/
#define	TSS_386			0x9	/* 可用 386 任务状态段类型值		*/
#define	CGate_386		0xC	/* 386 调用门类型值			*/
#define	IGate_386		0xE	/* 386 中断门类型值			*/
#define	TGate_386		0xF	/* 386 陷阱门类型值			*/
/* 存储段/系统段 描述符 */
typedef struct descriptor		/* 共 8 个字节 */
{
  u16	limit1;			/* Limit */
  u16	base_low;		/* Base */
  u8	base_mid;		/* Base */
  /* G(1) D(1) 0(1) AVL(1) Limit2(4) P(1) DPL(2) DT(1) TYPE(4) */
  /* u16	attr2_limit2_attr1; */
  u8	attr1;			/* P(1) DPL(2) DT(1) TYPE(4) */
  u8	attr2_limit2;		/* G(1) D(1) 0(1) AVL(1) LimitHigh(4) */
  u8	base_high;		/* Base */
}DESCRIPTOR;

/*========================================================================
                                GATE
  ========================================================================*/
/* 选择子类型值说明 */
/* 注意：选择子为16位，以下常量按16位定义。*/
#define	RPL_MASK	0xFFFC
#define	RPL0		0
#define	RPL1		1
#define	RPL2		2
#define	RPL3		3
#define	RPL_KRNL	RPL0
#define	RPL_SERV	RPL1
#define	RPL_USER	RPL3
#define	TI_MASK		0xFFFB
#define	TI_GDT		0
#define	TI_LDT		4
/* 门描述符 */
typedef struct gate
{
  u16	offset_low;	/* Offset Low */
  u16	selector;	/* Selector */
  u8	dcount;		/* 该字段只在调用门描述符中有效。如果在利用
			   调用门调用子程序时引起特权级的转换和堆栈
			   的改变，需要将外层堆栈中的参数复制到内层
			   堆栈。该计数字段就是用于说明这种情况
			   发生时，要复制的双字参数的数量。*/
  u8	attr;		/* P(1) DPL(2) DT(1) TYPE(4) */
  u16	offset_high;	/* Offset High */
}GATE;

/*========================================================================
                                MESSAGE
  ========================================================================*/
/* send_id和recv_id */
#ifndef ANY_TASK
#define ANY_TASK	  -1
#endif 
#ifndef NO_TASK
#define NO_TASK	          -2
#endif 

/* flags，当每一个进程发送消息时，内核就给该消息设置级别，目前用到的是SERVER和USER，
   其他的以后备用，阻塞状态是进程自己要求的，进程自己进行设置 */
#define MSG_CLEAR    0x0
#define MSG_LEVEL0   0x1
#define MSG_LEVEL1   0x2
#define MSG_SERVER   MSG_LEVEL1
#define MSG_LEVEL2   0x4
#define MSG_LEVEL3   0x8
#define MSG_USER     MSG_LEVEL3
#define MSG_BLOCKED  0x10       /* 该位表示是否阻塞自己，置位代表阻塞，由进程自己决定 */
#define MSG_REPLY    0x20       /* 该位表示是否需要返回信息，置位代表需要，由进程自己决定 */
/*type*/
enum msgtype {
  /* 消息的类型为空(0) */
  MSG_NO_TYPE,

  /* 驱动用到的消息类型，等待中断，阻塞自己  */
  WAIT_INT,

  /* message type for drivers */
  DEV_OPEN,DEV_CLOSE,DEV_READ,DEV_WRITE,

  /* FS */
  OPEN, CLOSE, READ, WRITE
};
/* 目前内核的消息机制，只用到了前五项 */
typedef struct message{
  int  send_id;             //发送者 ID
  int  recv_id;             //接收者 ID
  u32  flags;               //有32位标志，我们目前用其低5位，低四位为特权级
  struct message* p_next;   //指向消息指针，形成阻塞队列。
  char* msg_from_krnl;      //如果该指针不等于NULL，则存在错误信息，由内核指定。

  int  type;                //消息类型
  u32  size;                //数据的大小
  void* pointer;            //指向数据的指针
}MESSAGE;

/*========================================================================
                                TSS
  ========================================================================*/
typedef struct tss {
  u32	backlink;
  u32	esp0;
  u32	ss0;
  u32	esp1;
  u32	ss1;
  u32	esp2;
  u32	ss2;
  u32	cr3;
  u32	eip;
  u32	eflags;

  u32	eax;
  u32	ecx;
  u32	edx;
  u32	ebx;
  u32	esp;
  u32	ebp;
  u32	esi;
  u32	edi;
  u32	es;
  u32	cs;
  u32	ss;
  u32	ds;
  u32	fs;
  u32	gs;
  u32	ldt;
  u16	trap;
  u16	iobase;	/* I/O位图基址大于或等于TSS段界限，就表示没有I/O许可位图 */
}TSS;

/*========================================================================
                                PROCESS
  ========================================================================*/
/* p_recvfrom */
#ifndef ANY_TASK
#define ANY_TASK	  -1
#endif 
#ifndef NO_TASK
#define NO_TASK	          -2
#endif 

/* proc_flags */
#define PROC_READY         0x0      /* 进程可以运行 */
#define PROC_WAIT_INT     0x01      /* 为驱动而设置，等待中断 */
#define PROC_SENDING      0x02
#define PROC_RECEIVING 	  0x04

/* 每个任务都会有一个单独的LDT */
/* 每个LDT中的描述符个数: */
#define	LDT_AMT			2
#define	SELECTOR_LDT_CODE     0x0
#define	SELECTOR_LDT_DATA    0x08
#define	INDEX_LDT_CODE		0
#define	INDEX_LDT_DATA		1

#define	FILE_OPENED_AMT	       16
/*PROCESS*/
/* 注意：PROCESS前三项顺序和位置目前不能动 */
/* 注意：如果增加变量的话，记得在init_table()中初始化 */
typedef struct process {
  u32         proc_id;              /* 初始化的时候复制，从0开始 */
  DESCRIPTOR  ldts[LDT_AMT];	    /* local descriptors for code and data */
  char        proc_name[32];        /* 进程的名字 */
  int	      proc_ticks;
  int 	      proc_prio;
  int  	      proc_flags;                //如果proc_flags的值是0，那么这个进程是可以运行的
  struct process* p_proc_blocked;        //为阻塞队列而设........
 
  //下面两个变量用于消息机制
  MESSAGE*    p_que_msg;            //进程收到的消息都加到这个队列上.....
  int 	      p_recvfrom;           //该项应该做为条件

  //文件系统
  /* struct file_desc* file_opened[FILE_OPENED_AMT]; */
}PROCESS;

/*========================================================================
                                TASK
  ========================================================================*/
typedef struct task {
  task_func   initial_addr;
  int	      stack_size;
  char	      task_name[32];
}TASK;



#endif












