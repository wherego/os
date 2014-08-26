
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                              main.c
			                                  wzq
							  2013/6/5
  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "const.h"
#include "type.h"
#include "proto.h"
#include "global.h"

#include "init_proto.h"

void init_8259A()
{
  out_byte(PORT_8259A_MAS_CTRL,	        0x11);
  out_byte(PORT_8259A_SLA_CTRL,	        0x11);
  out_byte(PORT_8259A_MAS_CTRLMASK,	0x20);  //IRQ中断0x20
  out_byte(PORT_8259A_SLA_CTRLMASK,	0x28);  //IRQ中断0x28
  out_byte(PORT_8259A_MAS_CTRLMASK,	0x4 );
  out_byte(PORT_8259A_SLA_CTRLMASK,	0x2 );
  out_byte(PORT_8259A_MAS_CTRLMASK,	0x1 );
  out_byte(PORT_8259A_SLA_CTRLMASK,	0x1 );

  out_byte(PORT_8259A_MAS_CTRLMASK,	0xFF);
  out_byte(PORT_8259A_SLA_CTRLMASK,	0xFF);
}

