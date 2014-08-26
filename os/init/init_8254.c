
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            init_8254.c
			                            wzq
                                                    2013/6/9
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


#include "const.h"
#include "type.h"
#include "proto.h"
#include "global.h"

#include "init_proto.h"

/*======================================================================*
  init_8254
  *======================================================================*/
void init_8254()
{
  disp_str("--init_8254 begins--");
  out_byte(PORT_8254_MODE_CTRLREG,0x34);
  out_byte(PORT_8254_COUNTER0,(u8) (TIMER_8254_FREQ/CLOCK_FREQ) );
  out_byte(PORT_8254_COUNTER0,(u8) ((TIMER_8254_FREQ/CLOCK_FREQ)>>8) );

  enable_irq(IRQ0_CLOCK);     /* 让8259A可以接收时钟中断 */
}
