
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            server.c
			                                   wzq
							   2013/6/18
  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


#include "const.h"
#include "type.h"
#include "proto.h"
#include "global.h"

#include "kernel_proto.h"

/*======================================================================*
  server_sys
  *======================================================================*/
void server_sys()
{
  /* MESSAGE msg; */
  /* set_msg(&msg); */
  //ID号需要跟具体的功能一致，下面两行表示接收时的ID
  while (1) {
    /* msg.send_id=ANY_TASK; */
    /* //msg.send_id=USERA_ID; */
    /* msg.recv_id=SERVER_SYS_ID; */
    /* //msg.str="myself"; */

    disp_str(" SYS ");
    /* send_recv(RECEIVE, &msg); */
    /* disp_str(" S2 "); */

    /* if(msg.msg_from_krnl != NULL) */
    /*   { */
    /*   	disp_str(msg.msg_from_krnl); */
    /*   } */
    /* else{ */
    /*   disp_str("<"); */
    /*   //disp_str(msg.str); */
    /*   disp_str("> "); */
    /* } */
    milli_delay(500);
  }
}
/*======================================================================*
  server_tty
  *======================================================================*/
void server_tty()
{
  while (1) {
    disp_str(" TTY ");
    milli_delay(500);
  }
}
