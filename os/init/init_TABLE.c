
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            init_TABLE.c
			                              wzq
						      2013/5/21
  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


#include "const.h"
#include "type.h"
#include "proto.h"
#include "global.h"

#include "init_proto.h"

PRIVATE void max_heapity(PROCESS* proc_index_table[],int proc_amt,int parient);

/*======================================================================*
  PUBLIC void init_table()          
  *======================================================================*/
void init_table()
{
  disp_str("--init_table begins--\n");

  /* 初始化process table */
  PROCESS   *p_proc	      = proc_table;
  TASK      *p_task;
  char      *p_task_stack     = task_stacks + STACK_SIZE_TOTAL;
  char      *p_krnl_stack     = krnl_stacks + KRNL_STACK_SIZE * PROC_AMT;
  u16	     selector_ldt     = SELECTOR_LDT_FIRST;

  u16        dpl;    /*DPL*/
  u16        rpl;    /*RPL*/
  int        eflags;
  int        prio;
  int        i;

  /* 初始化tss table */
  memset(tss, 0, sizeof(tss));

  for(i=0;i<PROC_AMT;i++){           /* PROC_AMT=SERV_AMT+USER_AMT */
    if(i < SERV_AMT){              /* server */
      p_task    = server_table + i;
      dpl       = DPL_SERV;          /*DPL1*/
      rpl       = RPL_SERV;          /*RPL1*/
      eflags    = 0x1202;            /* IF=1, IOPL=1, bit 2 is always 1 */
      prio      = 20;
      p_proc->proc_flags  = PROC_READY;        //如果proc_flags的值是0，那么这个进程是可以运行的
                                               //先放到这里，我不打算让他们开机就运行
    }
    else{                           /* user */
      p_task    = user_table + (i - SERV_AMT);
      dpl       = DPL_USER;          /*DPL3*/
      rpl       = RPL_USER;          /*RPL3*/
      eflags    = 0x202;             /* IF=1, bit 2 is always 1 */
      prio      = 20;                          //先让它等于20，实际上要比SERVER小!!!!!!!!!!!!!!!!!!!!!!!
      p_proc->proc_flags  = PROC_READY;        //如果proc_flags的值是0，那么这个进程是可以运行的
                                               //开机就运行,以后再改...................................
    }
    /* 初始化tss[TSS_AMT] */
    tss[i].iobase = sizeof(tss[i]); /* 没有I/O许可位图 */
    tss[i].ldt    = selector_ldt;   //？？？？？？？？？？？？？？？？？？？？？？？？？？？？？

    tss[i].gs	  = (SELECTOR_KRNL_GS  & RPL_MASK & TI_MASK) | TI_GDT | rpl;
    tss[i].fs	  = (SELECTOR_LDT_DATA & RPL_MASK & TI_MASK) | TI_LDT | rpl;
    tss[i].ds	  = (SELECTOR_LDT_DATA & RPL_MASK & TI_MASK) | TI_LDT | rpl;
    tss[i].ss	  = (SELECTOR_LDT_DATA & RPL_MASK & TI_MASK) | TI_LDT | rpl;
    tss[i].cs	  = (SELECTOR_LDT_CODE & RPL_MASK & TI_MASK) | TI_LDT | rpl;
    tss[i].es	  = (SELECTOR_LDT_DATA & RPL_MASK & TI_MASK) | TI_LDT | rpl;

    tss[i].esp    = (u32)p_task_stack;
    tss[i].eflags = eflags;
    tss[i].eip    = (u32)p_task->initial_addr;

    tss[i].cr3    = (u32)PD_BASE_ADDR;

    tss[i].ss0    = SELECTOR_KRNL_DS;
    tss[i].esp0   = (u32)p_krnl_stack;

    /* 初始化proc_table[PROC_AMT] */
    memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KRNL_CS>>3], sizeof(DESCRIPTOR));
    p_proc->ldts[0].attr1 = Code | dpl | PRES;	        // change the DPL 注意：attr1是8位
    memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KRNL_DS>>3], sizeof(DESCRIPTOR));
    p_proc->ldts[1].attr1 = Data_RW | dpl | PRES;       // change the DPL 注意：attr1是8位

    strcpy(p_proc->proc_name, p_task->task_name);       // 进程的名字
    p_proc->proc_id        = i;			        // process id，从0开始，前几个是服务者
    p_proc->proc_ticks     = p_proc->proc_prio = prio;

    p_proc->p_proc_blocked = NULL;

    p_proc->p_que_msg      = NULL;
    p_proc->p_recvfrom     = ANY_TASK;      //我们定义它可以从任何进程接收消息

    proc_index_table[i]    = p_proc;        /*process index table，假设刚开始的时候所有的进程都可以运行*/

    p_task_stack -= p_task->stack_size;
    p_krnl_stack -= KRNL_STACK_SIZE;     //在内存，每个进程的栈大小是一样的。
    p_proc++;
    selector_ldt += 2 << 3;
  }

  /* 现在认为所有的进程开始都是可以运行的，以后若要更改，不要忘来改这里。建立大顶堆 */
  for(i=PROC_AMT/2-1;i>=0;i--)                //貌似这么做，看看结果吧！！！！！！！！！！
    {
      max_heapity(proc_index_table,PROC_AMT,i);
    }
}

PRIVATE void max_heapity(PROCESS* proc_index_table[],int proc_amt,int parient)
{
  int left,right;
  int largest;

  PROCESS* p_proc;

  left = 2*parient+1;
  right= 2*parient+2;

  if((left < proc_amt) && (proc_index_table[left]->proc_ticks > proc_index_table[parient]->proc_ticks))
    {
      largest = left;
    }
  else
    {
      largest = parient;
    }

  if((right < proc_amt) && (proc_index_table[right]->proc_ticks > proc_index_table[largest]->proc_ticks))
    {
      largest = right;
    }

  if (largest != parient)
    {  
      p_proc = proc_index_table[parient];              //这里对不对啊？？？？？？？？？？？？？？？？？
      proc_index_table[parient] = proc_index_table[largest];
      proc_index_table[largest] = p_proc;
      max_heapity(proc_index_table,proc_amt,largest);
    }
}
