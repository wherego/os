
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            lib_proto.h
						    wzq
                                                    2013/6/6
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef	_LIB_PROTO_H
#define _LIB_PROTO_H


//k_lib.asm
void disable_irq(int irq_nb);   //(irq_nb=0,...,15)
void enable_irq(int irq_nb);    //(irq_nb=0,...,15)
void disable_int();
void enable_int();

//K_LIB.c
int   sys_get_ticks();
void  int_to_str(char * str, int num);
void  disp_int(int input);
void  milli_delay(int milli_sec);
void  delay(int time);

//port.asm
void out_byte(u16 port, u8 value);
u8   in_byte(u16 port);
void io_delay();
void port_read(u16 port, void* buf, int size);
void port_write(u16 port, void* buf, int size);

//disp.asm
void disp_str(char * str);
void disp_return();

//string.asm
char*  strcpy(char* p_dst, char* p_src);   //返回Destination



#endif
