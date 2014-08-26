
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                              init_GDT.c
			                            wzq
                                                    2013/6/18
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


#include "const.h"
#include "type.h"
#include "proto.h"
#include "global.h"

#include "init_proto.h"

/* 虚拟地址 → 线性地址 */
#define vir2lin(base, vir)	((u32)(((u32)base) + (u32)(vir)))

PRIVATE u32  seg_base(u16 selector);
PRIVATE void set_desc(DESCRIPTOR *p_desc,u32 base,u32 limit,u16 desc_attr);

void init_gdt()
{
  disp_str("\n\n\n\n\n"
	   "--init_gdt begins--");

  /* 填充GDT中的TSS和LDT描述符 */
  int     i;
  u16	  selector_ldt = SELECTOR_LDT_FIRST;
  u16	  selector_tss = SELECTOR_TSS_FIRST;

  for(i=0;i<PROC_AMT;i++){                    //PROC_AMT=SERV_AMT+USER_AMT
    set_desc(&gdt[selector_ldt>>3],
	     vir2lin(seg_base(SELECTOR_KRNL_DS), proc_table[i].ldts),
	     LDT_AMT * sizeof(DESCRIPTOR) - 1,
	     LDT | PRES | DPL_KRNL);

    set_desc(&gdt[selector_tss>>3],
	     vir2lin(seg_base(SELECTOR_KRNL_DS), &tss[i]),
	     sizeof(tss[i]) - 1,
	     TSS_386 | PRES | DPL_KRNL);

    selector_ldt += 2 << 3;
    selector_tss += 2 << 3;
  }
}

/*======================================================================*
                          init_descriptor

  初始化段描述符
  *======================================================================*/
PRIVATE void set_desc(DESCRIPTOR *p_desc,u32 base,u32 limit,u16 desc_attr)
{
  p_desc->limit1   	= limit & 0x0FFFF;
  p_desc->base_low	= base  & 0x0FFFF;
  p_desc->base_mid	= (base >> 16) & 0x0FF;
  /* G(1) D(1) 0(1) AVL(1) LimitHigh(4) P(1) DPL(2) DT(1) TYPE(4) */
  //p_desc->attr2_limit2_attr1= (desc_attr & 0xF0FF) | ((limit>>8) & 0xF00);
  p_desc->attr1		= desc_attr & 0xFF;
  p_desc->attr2_limit2    = (desc_attr>>8) & 0xF0 | ((limit>>16) & 0x0F);
  p_desc->base_high	= (base >> 24) & 0x0FF;
}

/*======================================================================*
                             seg_base

  由段名求绝对地址
  *======================================================================*/
PRIVATE u32 seg_base(u16 selector)
{
  DESCRIPTOR* p = &gdt[selector >> 3];
  return (p->base_high<<24 | p->base_mid<<16 | p->base_low);
}
