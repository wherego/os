
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                user.c
				                          wzq
							  2013/6/5
  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


#include "const.h"
#include "type.h"
#include "proto.h"
#include "global.h"

#include "kernel_proto.h"

/*======================================================================*
  TaskA
  *======================================================================*/
void TaskA()
{
  /* MESSAGE msg; */
  /* set_msg(&msg); */

  /* //ID号需要跟具体的功能一致，下面两行表示接收时的ID */
  /* msg.send_id=USERA_ID; */
  /* msg.recv_id=SERVER_SYS_ID; */
  //msg.flags |= MSG_BLOCKED;
  //msg.str="hello";

  while (1) {
    disp_str("A ");
    /* send_recv(SEND, &msg); */
    /* msg.msg_from_krnl = NULL; */

    milli_delay(1000);
  }
}

/*======================================================================*
  TaskB
  *======================================================================*/
void TaskB()
{
  /* MESSAGE msg; */
  /* set_msg(&msg); */

  /* //ID号需要跟具体的功能一致，下面两行表示接收时的ID */
  /* msg.send_id=USERB_ID; */
  /* msg.recv_id=SERVER_SYS_ID; */
  /* msg.flags |= MSG_BLOCKED; */
  //msg.str="soso";

  while (1) {
    disp_str("B ");
    /* send_recv(SEND, &msg); */
    /* disp_str("B2"); */
    /* msg.msg_from_krnl = NULL; */

    milli_delay(1000);
  }
}

/*======================================================================*
  TaskC 先用这个server调试hard disk，硬盘驱动程序
  *======================================================================*/
void TaskC()
{
  while (1) {
    disp_str("C ");
    milli_delay(1000);
  }
}
