/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            scheduler.c
			                            wzq
                                                    2013/6/18
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


#include "const.h"
#include "type.h"
#include "proto.h"
#include "global.h"

#include "kernel_proto.h"

/*======================================================================*
                              schedule
 *======================================================================*
 *在这之前是有一个大顶堆的，当调度的时候，我们只需要把指针指向proc_index_table[0]。
 *但是，需要先维持大顶堆的特性。
 *======================================================================*/
void scheduler()     //我们事先已经建立好了一个大顶堆
{
  int parient;
  int left;
  int right;
  int largest;

  PROCESS* p_proc;

  parient=0;

  while(parient < proc_ready_amt/2)
    {
      left = 2*parient+1;
      right= 2*parient+2;

      if((left < proc_ready_amt) && (proc_index_table[left]->proc_ticks > proc_index_table[parient]->proc_ticks))
	{
	  largest = left;
	}
      else
	{
	  largest = parient;
	}

      if((right < proc_ready_amt) && (proc_index_table[right]->proc_ticks > proc_index_table[largest]->proc_ticks))
	{
	  largest = right;
	}

      if (largest != parient)
	{  
	  p_proc = proc_index_table[parient];
	  proc_index_table[parient] = proc_index_table[largest];
	  proc_index_table[largest] = p_proc;
	  parient = largest;
	}
      else
	{
	  break;
	}
    }

  p_proc_ready = proc_index_table[0];
}
