
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            kernel_proto.h
						    wzq
                                                    2013/6/6
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef	_KERNEL_PROTO_H
#define _KERNEL_PROTO_H


//idt.asm
void _divide_error();
void _debug_exception();
void _nmi();
void _breakpoint_exception();
void _overflow();
void _bounds_check();
void _inval_opcode();
void _copr_not_available();
void _double_fault();
void _copr_seg_overrun();
void _inval_tss();
void _segment_not_present();
void _stack_exception();
void _general_protection();
void _page_fault();
void _math_error();
void _align_check();
void _machine_check();
void _SIMD_floatingpoint_exception();

void    _irq0();
void    _irq1();
void    _irq2();
void    _irq3();
void    _irq4();
void    _irq5();
void    _irq6();
void    _irq7();
void    _irq8();
void    _irq9();
void    _irq10();
void    _irq11();
void    _irq12();
void    _irq13();
void    _irq14();
void    _irq15();

void _sys_call();

//IDT.c
void 	divide_error();
void 	debug_exception();
void 	nmi();
void 	breakpoint_exception();
void 	overflow();
void 	bounds_check();
void 	inval_opcode();
void 	copr_not_available();
void 	double_fault();
void 	copr_seg_overrun();
void 	inval_tss();
void 	segment_not_present();
void 	stack_exception();
void 	general_protection();
void 	page_fault();
void 	math_error();
void 	align_check();
void 	machine_check();
void 	SIMD_floatingpoint_exception();

void    irq0();
void    irq1();
void    irq2();
void    irq3();
void    irq4();
void    irq5();
void    irq6();
void    irq7();
void    irq8();
void    irq9();
void    irq10();
void    irq11();
void    irq12();
void    irq13();
void    irq14();
void    irq15();

//IPC.c
void send_recv(int function, MESSAGE* msg);
void sys_sendrecv(PROCESS* p_proc, int function, MESSAGE* p_msg);
void msg_send(MESSAGE* p_msg);
void msg_recv(MESSAGE* p_msg);
void block(PROCESS* p_proc);
void unblock(PROCESS* p_proc);
int  deadlock(int src, int dest);
void set_msg(MESSAGE* p_msg);

//schedule.c
void scheduler();

//server.c
void server_sys();
void server_tty();

//syscall.asm
int  _sendrecv(int function, MESSAGE* msg);

//user.c
void TaskA();
void TaskB();
void TaskC();



#endif
