/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                               fs.c
			                                  wzq
							  2013/9/24
  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


#include "const.h"
#include "type.h"
#include "proto.h"
#include "global.h"

#include "fs_proto.h"

#include "fs.h"
#include "../drivers/include/hard_disk.h"

PRIVATE void init_fs();
PRIVATE void make_fs();

PRIVATE int do_open();
PRIVATE int do_close();
PRIVATE int do_read();
PRIVATE int do_write();

PRIVATE int alloc_imap_bit(int dev);
/*****************************************************************************
 *                                server_fs
 *****************************************************************************

 *****************************************************************************/
void server_fs()
{
  init_fs();

  MESSAGE msg;
  set_msg(&msg);

  while (TRUE) {
    msg.send_id = ANY_TASK;
    msg.recv_id = SERVER_FS_ID;

    send_recv(RECEIVE, &msg);

    switch (msg.type) {
    case OPEN:
      do_open();
      break;
    case CLOSE:
      do_close();
      break;
    case READ:
      do_read();
      break;
    case WRITE:
      do_write();
      break;
    default:
      //以后处理？？？？？？？？？？？？？？？？？？
      break;
    }

    /* 目前这里有一个问题，当发送者需要返回信息信息时，怎么返回，因为这里定义了一个message， */
    /* 接收时会覆盖这个消息。我们现在传递的是指针，可惜我现在还不会分配内存！！！！！！！！！！！！！！ */
    /* send_recv(SEND, &msg); */
    milli_delay(1000);
  }
}

/*****************************************************************************
 *                                init_fs
 *****************************************************************************
 *****************************************************************************/
PRIVATE void init_fs()
{
  make_fs();
  alloc_imap_bit(0);
}

/*****************************************************************************
 *                               make_fs
 *****************************************************************************
 *****************************************************************************/
PRIVATE void make_fs()
{
  MESSAGE fs_msg;
  char    buffer[SECTOR_SIZE];         //缓冲区。
  struct fs_request fs_req;

  int i;

  set_msg(&fs_msg);

  fs_msg.send_id = SERVER_FS_ID;
  fs_msg.recv_id = SERVER_HD_ID;
  fs_msg.flags  |= MSG_BLOCKED;
  fs_msg.type    = DEV_WRITE;
  fs_msg.size    = HD_REQUEST_SIZE;
  fs_msg.pointer = &fs_req;

  /************************/
  /*      super block     */
  /************************/
  struct  super_block sb;

  sb.magic       = MAGIC;             // 魔数
  sb.inodes_amt  = INODES_AMT;        // inodes为4096个，占据一个扇区
  sb.sects_amt   = SECTS_AMT;         // 扇区数量,舍弃了3840个扇区。
  sb.dir_ent_amt = DIR_ENT_AMT;       // 目录条目数量

  sb.imap_sects_amt    = IMAP_SECTS_AMT;      // inode-map占的扇区数量
  sb.smap_sects_amt    = SMAP_SECTS_AMT;      // sector-map占的扇区数量
  sb.inode_sects_amt   = INODE_SECTS_AMT;     // inode占的扇区数量
  sb.dir_ent_sects_amt = DIR_ENT_SECTS_AMT;   // directory entry占的扇区数量

  sb.data_sect_start   = DATA_SEC_START;      // 数据起始扇区，从0开始

  fs_req.dev  = 0;                       //只能先这么写了。目前只有一个device
  fs_req.sec_start = SUPER_BLOCK_START;  //记得是从0开始的！！！
  fs_req.data_size = SUPER_BLOCK_SIZE;
  fs_req.p_data    = &sb;
  fs_req.p_proc_waiting  = &proc_table[SERVER_FS_ID];

  send_recv(SEND, &fs_msg);
  /************************/
  /*       inode map      */
  /************************/
  memset(buffer, 0, SECTOR_SIZE);

  for (i=0; i<(4+2); i++)    //第一个保留，第二个根文件。后四个console,以后修改这里！！！！！！！！！！
    buffer[0] |= 1 << i;

  fs_req.dev  = 0;                     //只能先这么写了。目前只有一个device
  fs_req.sec_start = INODE_MAP_START;  //记得是从0开始的！！！
  fs_req.data_size = SECTOR_SIZE;
  fs_req.p_data    = buffer;
  fs_req.p_proc_waiting = &proc_table[SERVER_FS_ID];

  send_recv(SEND, &fs_msg);
  /************************/
  /*      secter map      */
  /************************/
  memset(buffer, 0, SECTOR_SIZE);

  for (i = 0; i < ROOT_SECTS_AMT; i++)          //八个扇区留给root目录
    buffer[0] |= (1 << i);

  fs_req.dev  = 0;       //只能先这么写了。目前只有一个device
  fs_req.sec_start = SECTOR_MAP_START;  //记得是从0开始的！！！
  fs_req.data_size = SECTOR_SIZE;
  fs_req.p_data    = buffer;
  fs_req.p_proc_waiting = &proc_table[SERVER_FS_ID];

  send_recv(SEND, &fs_msg);

  //初始化剩余的map为0。这样写有点慢，以后改？？？？？？？？？？？？？？？？？？？？？
  //没有作用，为什么呢？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？
  /* memset(buffer, 2, SECTOR_SIZE); */

  /* for (i = 1; i < SMAP_SECTS_AMT; i++) */
  /*   { */
  /*     hd_req_que[free].dev  = 0;       //只能先这么写了。目前只有一个device */
  /*     hd_req_que[free].sec_start = SECTOR_MAP_START + i;  //记得是从0开始的！！！ */
  /*     hd_req_que[free].data_size = SECTOR_SIZE; */
  /*     hd_req_que[free].p_data    = buffer; */
  /*     hd_req_que[free].p_proc_waiting = &proc_table[SERVER_FS_ID]; */

  /*     send_recv(SEND, &fs_msg); */
  /*   } */
  /************************/
  /*       inodes         */
  /************************/
  memset(buffer, 0, SECTOR_SIZE);

  struct inode* p_inode = (struct inode*)buffer;

  p_inode->i_mode       = -1;                 //先这么用，保留.............
  p_inode->i_file_size  =  0;                 //先这么用，保留.............
  p_inode->i_start_sect =  0;                 //先这么用，保留.............
  p_inode->i_sects_amt  =  0;                 //先这么用，保留.............

  /* inode of root目录 */
  p_inode = (struct inode*)(buffer + INODE_SIZE);

  p_inode->i_mode       = DIRECTORY;
  p_inode->i_file_size  = DIR_ENTRY_SIZE * 5; /* 5 files:'.', dev_tty0~3.*/
  p_inode->i_start_sect = DATA_SEC_START;
  p_inode->i_sects_amt  = ROOT_SECTS_AMT;

  /* inode of /dev_tty0~3 */
  for (i = 0; i < 4; i++) {
    p_inode = (struct inode*)(buffer + (INODE_SIZE * (i + 2)));

    p_inode->i_mode       = CHAR_DEV;
    p_inode->i_file_size  = 0;
    p_inode->i_start_sect = 0;
    p_inode->i_sects_amt  = 0;
  }

  fs_req.dev       = 0;                  //只能先这么写了。目前只有一个device
  fs_req.sec_start = INODE_ARRAY_START;  //记得是从0开始的！！！
  fs_req.data_size = SECTOR_SIZE;
  fs_req.p_data    = buffer;
  fs_req.p_proc_waiting = &proc_table[SERVER_FS_ID];

  send_recv(SEND, &fs_msg);
  /************************/
  /*          `/'         */
  /************************/
  memset(buffer, 0, SECTOR_SIZE);
  struct dir_entry * p_dir_entry = (struct dir_entry *)buffer;

  /* 第一项保留 */
  p_dir_entry->inode_nb = 0;
  strcpy(p_dir_entry->file_name, "");

  /* dir entries of '.' */
  (++p_dir_entry)->inode_nb = 1;
  strcpy(p_dir_entry->file_name, ".");

  /* dir entries of '/dev_tty0~3' */
  for (i = 0; i < 4; i++) {
    p_dir_entry++;
    p_dir_entry->inode_nb = i + 2; /* dev_tty0's inode_nr is 2 */
  }

  fs_req.dev       = 0;                //只能先这么写了。目前只有一个device
  fs_req.sec_start = DIR_ENT_START;    //记得是从0开始的！！！
  fs_req.data_size = SECTOR_SIZE;
  fs_req.p_data    = buffer;
  fs_req.p_proc_waiting = &proc_table[SERVER_FS_ID];

  send_recv(SEND, &fs_msg);
}

/*****************************************************************************
 *                               do_open
 *****************************************************************************
 *****************************************************************************/
PRIVATE int do_open()
{
/*   int fd = -1;		/\* return value *\/ */

/*   char pathname[MAX_PATH]; */

/*   /\* get parameters from the message *\/ */
/*   int flags = fs_msg.FLAGS;	/\* access mode *\/ */
/*   int name_len = fs_msg.NAME_LEN;	/\* length of filename *\/ */
/*   int src = fs_msg.source;	/\* caller proc nr. *\/ */
/*   assert(name_len < MAX_PATH); */
/*   phys_copy((void*)va2la(TASK_FS, pathname), */
/* 	    (void*)va2la(src, fs_msg.PATHNAME), */
/* 	    name_len); */
/*   pathname[name_len] = 0; */

/*   /\* find a free slot in PROCESS::filp[] *\/ */
/*   int i; */
/*   for (i = 0; i < NR_FILES; i++) { */
/*     if (pcaller->filp[i] == 0) { */
/*       fd = i; */
/*       break; */
/*     } */
/*   } */
/*   if ((fd < 0) || (fd >= NR_FILES)) */
/*     panic("filp[] is full (PID:%d)", proc2pid(pcaller)); */

/*   /\* find a free slot in f_desc_table[] *\/ */
/*   for (i = 0; i < NR_FILE_DESC; i++) */
/*     if (f_desc_table[i].fd_inode == 0) */
/*       break; */
/*   if (i >= NR_FILE_DESC) */
/*     panic("f_desc_table[] is full (PID:%d)", proc2pid(pcaller)); */

/*   int inode_nr = search_file(pathname); */

/*   struct inode * pin = 0; */
/*   if (flags & O_CREAT) { */
/*     if (inode_nr) { */
/*       printl("file exists.\n"); */
/*       return -1; */
/*     } */
/*     else { */
/*       pin = create_file(pathname, flags); */
/*     } */
/*   } */
/*   else { */
/*     assert(flags & O_RDWR); */

/*     char filename[MAX_PATH]; */
/*     struct inode * dir_inode; */
/*     if (strip_path(filename, pathname, &dir_inode) != 0) */
/*       return -1; */
/*     pin = get_inode(dir_inode->i_dev, inode_nr); */
/*   } */

/*   if (pin) { */
/*     /\* connects proc with file_descriptor *\/ */
/*     pcaller->filp[fd] = &f_desc_table[i]; */

/*     /\* connects file_descriptor with inode *\/ */
/*     f_desc_table[i].fd_inode = pin; */

/*     f_desc_table[i].fd_mode = flags; */
/*     /\* f_desc_table[i].fd_cnt = 1; *\/ */
/*     f_desc_table[i].fd_pos = 0; */

/*     int imode = pin->i_mode & I_TYPE_MASK; */

/*     if (imode == I_CHAR_SPECIAL) { */
/*       MESSAGE driver_msg; */
/*       driver_msg.type = DEV_OPEN; */
/*       int dev = pin->i_start_sect; */
/*       driver_msg.DEVICE = MINOR(dev); */
/*       assert(MAJOR(dev) == 4); */
/*       assert(dd_map[MAJOR(dev)].driver_nr != INVALID_DRIVER); */
/*       send_recv(BOTH, */
/* 		dd_map[MAJOR(dev)].driver_nr, */
/* 		&driver_msg); */
/*     } */
/*     else if (imode == I_DIRECTORY) { */
/*       assert(pin->i_num == ROOT_INODE); */
/*     } */
/*     else { */
/*       assert(pin->i_mode == I_REGULAR); */
/*     } */
/*   } */
/*   else { */
/*     return -1; */
/*   } */

/*   return fd; */
}

/*****************************************************************************
 *                                do_close
 *****************************************************************************
 *****************************************************************************/
PRIVATE int do_close()
{
/*   int fd = fs_msg.FD; */
/*   put_inode(pcaller->filp[fd]->fd_inode); */
/*   pcaller->filp[fd]->fd_inode = 0; */
/*   pcaller->filp[fd] = 0; */

/*   return 0; */
}

/*****************************************************************************
 *                                create_file
 *****************************************************************************
 *****************************************************************************/
PRIVATE struct inode * create_file(char * path, int flags)
{
/*   char filename[MAX_PATH]; */
/*   struct inode * dir_inode; */
/*   if (strip_path(filename, path, &dir_inode) != 0) */
/*     return 0; */

/*   int inode_nr = alloc_imap_bit(dir_inode->i_dev); */
/*   int free_sect_nr = alloc_smap_bit(dir_inode->i_dev, */
/* 				    NR_DEFAULT_FILE_SECTS); */
/*   struct inode *newino = new_inode(dir_inode->i_dev, inode_nr, */
/* 				   free_sect_nr); */

/*   new_dir_entry(dir_inode, newino->i_num, filename); */

/*   return newino; */
}
/*****************************************************************************
 *                                alloc_imap_bit
 *****************************************************************************
 *dev:设备号。
 *返回值：inode号。
 *****************************************************************************/
PRIVATE int alloc_imap_bit(int dev)
{
  /* 这样做很不科学，应该根据super block里的信息确定，以后改!!!!!!!!!!!!!!!!!!!!!!!! */
  MESSAGE fs_msg;
  struct fs_request fs_req;

  int imap_start  = INODE_MAP_START;
  int imap_sects_amt = IMAP_SECTS_AMT;

  const int bytes = imap_sects_amt * SECTOR_SIZE; 

  char buffer[bytes];   //目前，其实就是512...
  int  inode_nb;

  int i, j;
  int flag = FALSE;

  set_msg(&fs_msg);

  /* 设置fs_msg */
  fs_msg.send_id = SERVER_FS_ID;
  fs_msg.recv_id = SERVER_HD_ID;
  fs_msg.flags  |= MSG_BLOCKED;
  fs_msg.type    = DEV_READ;
  fs_msg.size    = HD_REQUEST_SIZE;
  fs_msg.pointer = &fs_req;

  /* 设置fs_rep */
  fs_req.dev       = 0;               //只能先这么写了。目前只有一个device
  fs_req.sec_start = imap_start;      //记得是从0开始的！！！
  fs_req.data_size = imap_sects_amt * SECTOR_SIZE;
  fs_req.p_data    = buffer;
  fs_req.p_proc_waiting = &proc_table[SERVER_FS_ID];

  send_recv(SEND, &fs_msg);

  for (i = 0; i < bytes; i++) {
    if (buffer[i] == 0xFF)
      continue;

    /* skip `1' bits */
    j = 0;
    while (((buffer[i] >> j) & 1) != 0)
      {
	j++;
      }

    /* i: byte index; j: bit index */
    inode_nb = i *8 + j;
    buffer[i] |= (1 << j);

    flag = TRUE;
    break;
  }

  if (flag == TRUE){
    /* 设置fs_msg */
    fs_msg.send_id = SERVER_FS_ID;
    fs_msg.recv_id = SERVER_HD_ID;
    fs_msg.flags  |= MSG_BLOCKED;
    fs_msg.type    = DEV_WRITE;
    fs_msg.size    = HD_REQUEST_SIZE;
    fs_msg.pointer = &fs_req;

    /* 设置fs_rep */
    fs_req.dev       = 0;               //只能先这么写了。目前只有一个device
    fs_req.sec_start = imap_start;      //记得是从0开始的！！！
    fs_req.data_size = imap_sects_amt * SECTOR_SIZE;
    fs_req.p_data    = buffer;
    fs_req.p_proc_waiting = &proc_table[SERVER_FS_ID];

    send_recv(SEND, &fs_msg);

    return inode_nb;
  }
  else{
    return 0;   //第0个保留
  }
}

/*****************************************************************************
 *                                alloc_smap_bit
 *****************************************************************************
 *****************************************************************************/
PRIVATE int alloc_smap_bit(int dev, int nr_sects_to_alloc)
{
/*   /\* int nr_sects_to_alloc = NR_DEFAULT_FILE_SECTS; *\/ */

/*   int i; /\* sector index *\/ */
/*   int j; /\* byte index *\/ */
/*   int k; /\* bit index *\/ */

/*   struct super_block * sb = get_super_block(dev); */

/*   int smap_blk0_nr = 1 + 1 + sb->nr_imap_sects; */
/*   int free_sect_nr = 0; */

/*   for (i = 0; i < sb->nr_smap_sects; i++) { /\* smap_blk0_nr + i : */
/* 					       current sect nr. *\/ */
/*     RD_SECT(dev, smap_blk0_nr + i); */

/*     /\* byte offset in current sect *\/ */
/*     for (j = 0; j < SECTOR_SIZE && nr_sects_to_alloc > 0; j++) { */
/*       k = 0; */
/*       if (!free_sect_nr) { */
/* 	/\* loop until a free bit is found *\/ */
/* 	if (fsbuf[j] == 0xFF) continue; */
/* 	for (; ((fsbuf[j] >> k) & 1) != 0; k++) {} */
/* 	free_sect_nr = (i * SECTOR_SIZE + j) * 8 + */
/* 	  k - 1 + sb->n_1st_sect; */
/*       } */

/*       for (; k < 8; k++) { /\* repeat till enough bits are set *\/ */
/* 	assert(((fsbuf[j] >> k) & 1) == 0); */
/* 	fsbuf[j] |= (1 << k); */
/* 	if (--nr_sects_to_alloc == 0) */
/* 	  break; */
/*       } */
/*     } */

/*     if (free_sect_nr) /\* free bit found, write the bits to smap *\/ */
/*       WR_SECT(dev, smap_blk0_nr + i); */

/*     if (nr_sects_to_alloc == 0) */
/*       break; */
/*   } */

/*   assert(nr_sects_to_alloc == 0); */

/*   return free_sect_nr; */
}

/*****************************************************************************
 *                                new_inode
 *****************************************************************************/
/**
 * Generate a new i-node and write it to disk.
 *
 * @param dev  Home device of the i-node.
 * @param inode_nr  I-node nr.
 * @param start_sect  Start sector of the file pointed by the new i-node.
 *
 * @return  Ptr of the new i-node.
 *****************************************************************************/
PRIVATE struct inode * new_inode(int dev, int inode_nr, int start_sect)
{
/*   struct inode * new_inode = get_inode(dev, inode_nr); */

/*   new_inode->i_mode = I_REGULAR; */
/*   new_inode->i_size = 0; */
/*   new_inode->i_start_sect = start_sect; */
/*   new_inode->i_nr_sects = NR_DEFAULT_FILE_SECTS; */

/*   new_inode->i_dev = dev; */
/*   new_inode->i_cnt = 1; */
/*   new_inode->i_num = inode_nr; */

/*   /\* write to the inode array *\/ */
/*   sync_inode(new_inode); */

/*   return new_inode; */
}

/*****************************************************************************
 *                                new_dir_entry
 *****************************************************************************/
/**
 * Write a new entry into the directory.
 *
 * @param dir_inode  I-node of the directory.
 * @param inode_nr   I-node nr of the new file.
 * @param filename   Filename of the new file.
 *****************************************************************************/
PRIVATE void new_dir_entry(struct inode *dir_inode,int inode_nr,char *filename)
{
/*   /\* write the dir_entry *\/ */
/*   int dir_blk0_nr = dir_inode->i_start_sect; */
/*   int nr_dir_blks = (dir_inode->i_size + SECTOR_SIZE) / SECTOR_SIZE; */
/*   int nr_dir_entries = */
/*     dir_inode->i_size / DIR_ENTRY_SIZE; /\** */
/* 					 * including unused slots */
/* 					 * (the file has been */
/* 					 * deleted but the slot */
/* 					 * is still there) */
/* 					 *\/ */
/*   int m = 0; */
/*   struct dir_entry * pde; */
/*   struct dir_entry * new_de = 0; */

/*   int i, j; */
/*   for (i = 0; i < nr_dir_blks; i++) { */
/*     RD_SECT(dir_inode->i_dev, dir_blk0_nr + i); */

/*     pde = (struct dir_entry *)fsbuf; */
/*     for (j = 0; j < SECTOR_SIZE / DIR_ENTRY_SIZE; j++,pde++) { */
/*       if (++m > nr_dir_entries) */
/* 	break; */

/*       if (pde->inode_nr == 0) { /\* it's a free slot *\/ */
/* 	new_de = pde; */
/* 	break; */
/*       } */
/*     } */
/*     if (m > nr_dir_entries ||/\* all entries have been iterated or *\/ */
/* 	new_de)              /\* free slot is found *\/ */
/*       break; */
/*   } */
/*   if (!new_de) { /\* reached the end of the dir *\/ */
/*     new_de = pde; */
/*     dir_inode->i_size += DIR_ENTRY_SIZE; */
/*   } */
/*   new_de->inode_nr = inode_nr; */
/*   strcpy(new_de->name, filename); */

/*   /\* write dir block -- ROOT dir block *\/ */
/*   WR_SECT(dir_inode->i_dev, dir_blk0_nr + i); */

/*   /\* update dir inode *\/ */
/*   sync_inode(dir_inode); */
}

/*****************************************************************************
 *                                new_dir_entry
 *****************************************************************************
 *****************************************************************************/
PRIVATE int do_read()
{
}

/*****************************************************************************
 *                                new_dir_entry
 *****************************************************************************
 *****************************************************************************/
PRIVATE int do_write()
{
}
