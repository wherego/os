
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            init_proto.h
						    wzq
                                                    2013/6/6
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef	_INIT_PROTO_H
#define _INIT_PROTO_H


//global.c
//这里面没有函数

// init.asm
void _start();

//init_8254.c
void init_8254();

//init_8259A.c
void init_8259A();

//init_GDT.c
void init_gdt();

//init_IDT.c
void init_idt();

//init_TABLE.c
void init_table();


#endif
