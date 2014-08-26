
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                              IDT.c
                                                         wzq
                                                         2013/5/22
  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


#include "const.h"
#include "type.h"
#include "proto.h"
#include "global.h"

#include "kernel_proto.h"

#include "../drivers/include/keyboard.h"

#include "../drivers/include/drv_local.h"

PRIVATE void exception_handler(int err_code);
PRIVATE void inform_int(int server_nr);
/* 我把中断函数都放在了这里，包括IRQ中断，为了避免混乱～～ */
void divide_error()
{
  exception_handler(0);
}
void debug_exception()
{
  exception_handler(1);
}
void nmi()
{
  exception_handler(2);
}
void breakpoint_exception()
{
  exception_handler(3);
}
void overflow()
{
  exception_handler(4);
}
void bounds_check()
{
  exception_handler(5);
}
void inval_opcode()
{
  exception_handler(6);
}
void copr_not_available()
{
  exception_handler(7);
}
void double_fault()
{
  exception_handler(8);
}
void copr_seg_overrun()
{
  exception_handler(9);
}
void inval_tss()
{
  exception_handler(10);
}
void segment_not_present()
{
  exception_handler(11);
}
void stack_exception()
{
  exception_handler(12);
}
void general_protection()
{
  exception_handler(13);
}
void page_fault()
{
  exception_handler(14);
}
/* 没有定义15号中断 */
void math_error()
{
  exception_handler(16);
}
void align_check()
{
  exception_handler(17);
}
void machine_check()
{
  exception_handler(18);
}
void SIMD_floatingpoint_exception()
{
  exception_handler(19);
}

PRIVATE void exception_handler(int err_code)
{
  char * err_msg[20]={"divide error",
		      "single step exception",
		      "NMI",
		      "Breakpoint exception",
		      "Overflow",
		      "BOUND Range Exceeded",
		      "Invalid Opcode (Undefined Opcode)",
		      "Device Not Available (No Math Coprocessor)",
		      "Double Fault",
		      "Coprocessor Segment Overrun (reserved)",
		      "Invalid TSS",
		      "Segment Not Present",
		      "Stack-Segment Fault",
		      "General Protection",
		      "Page Fault",
		      "—  (Intel reserved. Do not use.)",
		      "x87 FPU Floating-Point Error (Math Fault)",
		      "Alignment Check",
		      "Machine Check",
		      "SIMD Floating-Point Exception"
  };
  disp_str(err_msg[err_code]);
}

void irq0(int irq_nb)              //时间中断函数，其中一个调用schedule()的地方
{
  sys_ticks++;

  if (locker != 1) {
    disp_str("!");
    return;
  }
  p_proc_ready->proc_ticks--;

  scheduler();
}

void irq1(int irq_nb)              //键盘中断函数
{
  u8 scan_code = in_byte(PORT_8042_IN_BUF);

  if (kb_in_buf.count < KB_IN_BUF_BYTES) {
    *(kb_in_buf.p_head) = scan_code;
    kb_in_buf.p_head++;

    if (kb_in_buf.p_head == kb_in_buf.buf + KB_IN_BUF_BYTES) {
      kb_in_buf.p_head = kb_in_buf.buf;
    }

    kb_in_buf.count++;
  }
}

void irq2(int irq_nb)
{
  //disp_str("-----\"irq 2\"-----");
}
void irq3(int irq_nb)
{
  disp_str("-----\"irq 3\"-----");
}
void irq4(int irq_nb)
{
  disp_str("-----\"irq 4\"-----");
}
void irq5(int irq_nb)
{
  disp_str("-----\"irq 5\"-----");
}
void irq6(int irq_nb)
{
  disp_str("-----\"irq 6\"-----");
}
void irq7(int irq_nb)
{
  disp_str("-----\"irq 7\"-----");
}
void irq8(int irq_nb)
{
  disp_str("-----\"irq 8\"-----");
}
void irq9(int irq_nb)
{
  disp_str("-----\"irq 9\"-----");
}
void irq10(int irq_nb)
{
  disp_str("-----\"irq 10\"-----");
}
void irq11(int irq_nb)
{
  disp_str("-----\"irq 11\"-----");
}
void irq12(int irq_nb)
{
  disp_str("-----\"irq 12\"-----");
}
void irq13(int irq_nb)
{
  disp_str("-----\"irq 13\"-----");
}
void irq14(int irq_nb)              //硬盘控制器，AT温盘
{
  disp_str("***************");
  /* Interrupts are cleared when the host */
  /*   - reads the Status Register, */
  /*   - issues a reset, or */
  /*   - writes to the Command Register. */

  in_byte(0x1F7);     //REG_STATUS=0x1F7

  inform_int(SERVER_HD_ID);           //目前这一句貌似没有用
}
void irq15(int irq_nb)
{
  disp_str("-----\"irq 15\"-----");
}


/* 某些中断对应一个驱动，我认为驱动一开始是处于休眠状态，等待中断去唤醒 */
/* 以后再改！！！！！！！！！！！！！！！！！！！！！！！ */
PRIVATE void inform_int(int server_nb)
{
  PROCESS* p_proc = proc_table + server_nb;
  p_proc->proc_flags &= ~PROC_WAIT_INT;        //目前这一句貌似没有用
  unblock(p_proc);
}
