
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                           drv_local.h
						    wzq
                                                    2013/10/1
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef	_DRV_LOCAL_H
#define _DRV_LOCAL_H

/*========================================================================
                         KEYBOARD_INPUT_BUFFER
  ========================================================================*/
/* Keyboard structure, 1 per console. */
#define	KB_IN_BUF_BYTES	32		/* 缓冲区大小 */
typedef struct keyboard_input_buffer {
  char*	p_head;			/* 指向缓冲区中下一个空闲位置 */
  char*	p_tail;			/* 指向键盘任务应处理的字节 */
  int	count;			/* 缓冲区中共有多少字节 */
  char	buf[KB_IN_BUF_BYTES];	/* 缓冲区 */
}KEYBOARD_INPUT_BUFFER;

EXTERN	 KEYBOARD_INPUT_BUFFER	 kb_in_buf;	/* Keyboard input buffer */

/* keyboard map */
#define  SCAN_CODES_AMT	 0x80
#define  MAP_COLS	    3
EXTERN	 u32 		 keyboardmap[SCAN_CODES_AMT * MAP_COLS];	/*keyboard map*/


#endif
