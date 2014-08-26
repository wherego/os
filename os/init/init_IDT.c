
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                              init_IDT.c
			                            wzq
                                                    2013/5/22
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


#include "const.h"
#include "type.h"
#include "proto.h"
#include "global.h"

#include "init_proto.h"
PRIVATE void set_idt_desc(int vector, u8 desc_attr, int_handler handler);

void init_idt()
{
  disp_str("--init_idt begins--");

  // 全部初始化成中断门(没有陷阱门)
  set_idt_desc(INT_VEC_DividERR_FAULT,       IGate_386|PRES|DPL_KRNL,    //int 0
	       _divide_error);

  set_idt_desc(INT_VEC_Debug_FAULT,          IGate_386|PRES|DPL_KRNL,    //int 1
	       _debug_exception);

  set_idt_desc(INT_VEC_NMI_INTERRUPT,        IGate_386|PRES|DPL_KRNL,    //int 2
	       _nmi);

  set_idt_desc(INT_VEC_Breakpoint_TRAP,      IGate_386|PRES|DPL_KRNL,    //int 3
	       _breakpoint_exception);

  set_idt_desc(INT_VEC_Overflow_TRAP,        IGate_386|PRES|DPL_KRNL,    //int 4
	       _overflow);

  set_idt_desc(INT_VEC_Boundcheck_FAULT,     IGate_386|PRES|DPL_KRNL,    //int 5
	       _bounds_check);

  set_idt_desc(INT_VEC_InvalidOpcode_FAULT,  IGate_386|PRES|DPL_KRNL,    //int 6
	       _inval_opcode);

  set_idt_desc(INT_VEC_DeviceNot_FAULT,      IGate_386|PRES|DPL_KRNL,    //int 7
	       _copr_not_available);

  set_idt_desc(INT_VEC_DoubleFAU_ABORT,      IGate_386|PRES|DPL_KRNL,    //int 8
	       _double_fault);

  set_idt_desc(INT_VEC_CoprocessorSEG_FAULT, IGate_386|PRES|DPL_KRNL,    //int 9
	       _copr_seg_overrun);

  set_idt_desc(INT_VEC_InvalidTSS_FAULT,     IGate_386|PRES|DPL_KRNL,    //int 10
	       _inval_tss);

  set_idt_desc(INT_VEC_SEGNotPresent_FAULT,  IGate_386|PRES|DPL_KRNL,    //int 11
	       _segment_not_present);

  set_idt_desc(INT_VEC_StackSEG_FAULT,       IGate_386|PRES|DPL_KRNL,    //int 12
	       _stack_exception);

  set_idt_desc(INT_VEC_ProtectionERR_FAULT,  IGate_386|PRES|DPL_KRNL,    //int 13
	       _general_protection);

  set_idt_desc(INT_VEC_PageFAU_FAULT,        IGate_386|PRES|DPL_KRNL,    //int 14
	       _page_fault);

  set_idt_desc(INT_VEC_MathERR_FAULT,        IGate_386|PRES|DPL_KRNL,    //int 16
	       _math_error);

  set_idt_desc(INT_VEC_AlignCheck_FAULT,     IGate_386|PRES|DPL_KRNL,    //int 17
	       _align_check);

  set_idt_desc(INT_VEC_MachineCheck_ABORT,   IGate_386|PRES|DPL_KRNL,    //int 18
	       _machine_check);

  set_idt_desc(INT_VEC_SIMD_FAULT,           IGate_386|PRES|DPL_KRNL,    //int 19
	       _SIMD_floatingpoint_exception);


  set_idt_desc(INT_VEC_IRQ0 + 0, IGate_386|PRES|DPL_KRNL, _irq0);            //int 32


  set_idt_desc(INT_VEC_IRQ0 + 1, IGate_386|PRES|DPL_KRNL, _irq1);            //int 33


  set_idt_desc(INT_VEC_IRQ0 + 2, IGate_386|PRES|DPL_KRNL, _irq2);            //int 34


  set_idt_desc(INT_VEC_IRQ0 + 3, IGate_386|PRES|DPL_KRNL, _irq3);            //int 35


  set_idt_desc(INT_VEC_IRQ0 + 4, IGate_386|PRES|DPL_KRNL, _irq4);            //int 36


  set_idt_desc(INT_VEC_IRQ0 + 5, IGate_386|PRES|DPL_KRNL, _irq5);            //int 37


  set_idt_desc(INT_VEC_IRQ0 + 6, IGate_386|PRES|DPL_KRNL, _irq6);            //int 38


  set_idt_desc(INT_VEC_IRQ0 + 7, IGate_386|PRES|DPL_KRNL, _irq7);            //int 39


  set_idt_desc(INT_VEC_IRQ8 + 0, IGate_386|PRES|DPL_KRNL, _irq8);            //int 40


  set_idt_desc(INT_VEC_IRQ8 + 1, IGate_386|PRES|DPL_KRNL, _irq9);            //int 41


  set_idt_desc(INT_VEC_IRQ8 + 2, IGate_386|PRES|DPL_KRNL, _irq10);           //int 42


  set_idt_desc(INT_VEC_IRQ8 + 3, IGate_386|PRES|DPL_KRNL, _irq11);           //int 43


  set_idt_desc(INT_VEC_IRQ8 + 4, IGate_386|PRES|DPL_KRNL, _irq12);           //int 44


  set_idt_desc(INT_VEC_IRQ8 + 5, IGate_386|PRES|DPL_KRNL, _irq13);           //int 45


  set_idt_desc(INT_VEC_IRQ8 + 6, IGate_386|PRES|DPL_KRNL, _irq14);           //int 46


  set_idt_desc(INT_VEC_IRQ8 + 7, IGate_386|PRES|DPL_KRNL, _irq15);           //int 47


  set_idt_desc(INT_VEC_SYSCALL,  IGate_386|PRES|DPL_USER, _sys_call);       //int 0x80，伟大的系统调用
  //设置DPL=DPL_USER，注意啦
}

void set_idt_desc(int vector, u8 desc_attr, int_handler handler)
{
  GATE    *p_gate	= &idt[vector];
  u32	  base	        = (u32)handler;

  p_gate->offset_low	= base & 0xFFFF;
  p_gate->selector	= SELECTOR_KRNL_CS;
  p_gate->dcount	= 0;
  p_gate->attr		= desc_attr;
  p_gate->offset_high	= (base >> 16) & 0xFFFF;
}
