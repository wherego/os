
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            fs.h
						    wzq
                                                    2013/9/24
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


#ifndef	_FS_H
#define _FS_H

/************************************************************************
                          fs type
 ************************************************************************/
#define	 MAGIC	            0x7866

/* 以下是根据生成的虚拟硬盘，自己造参数，纯手工.......... */
#define  INODES_AMT          4096
#define  SECTS_AMT        1044480
#define  DIR_ENT_AMT         4096

#define  IMAP_SECTS_AMT          1
#define  SMAP_SECTS_AMT        255
#define  INODE_SECTS_AMT       128
#define  DIR_ENT_SECTS_AMT     128

#define  BOOT_SEC_START          0
#define  SUPER_BLOCK_START       1
#define  INODE_MAP_START         2
#define  SECTOR_MAP_START        3
#define  INODE_ARRAY_START     258
#define  DIR_ENT_START         386

#define  DATA_SEC_START        514

/* 以下定义是特制的.......... */
#define  ROOT_SECTS_AMT          8        //根文件占据的扇区数目
//################
//super_block
//################
struct super_block {
  int	magic;		    // 魔数

  u32	inodes_amt;	    // inodes 数量
  u32	sects_amt;	    // 扇区数量
  u32	dir_ent_amt;        // 目录条目数量

  u32	imap_sects_amt;	    // inode-map占的扇区数量
  u32	smap_sects_amt;	    // sector-map占的扇区数量
  u32	inode_sects_amt;    // inode占的扇区数量
  u32	dir_ent_sects_amt;  // directory entry占的扇区数量

  u32	data_sect_start;    //数据起始扇区
};

#define SUPER_BLOCK_SIZE sizeof(struct super_block)

//################
//inode      16个字节
//################
/*type*/
enum inode_type {
  /* directory */
  DIRECTORY,

  /* dev_tty*/
  CHAR_DEV
};

struct inode {
  int 	i_mode;		//访问模式
  u32	i_file_size;	//文件大小
  u32	i_start_sect;	//数据的第一个扇区
  u32	i_sects_amt;	//占据的扇区数目
};

#define INODE_SIZE sizeof(struct inode)

//################
//dir_entry  16个字节
//################
#define	MAX_FILENAME_LEN	12

struct dir_entry {
  u32	inode_nb;		        // inode number
  char	file_name[MAX_FILENAME_LEN];	// 文件名
};

#define	DIR_ENTRY_SIZE	sizeof(struct dir_entry)

//################
//file_desc
//################
struct file_desc {
  int 		fd_mode;	/**< R or W */
  u32		fd_pos;		/**< Current position for R/W. */
  struct inode*	fd_p_inode;	/**< Ptr to the i-node */
};

#define	FILE_DESC_SIZE	sizeof(struct file_desc)

//################
//hd_request
//################
struct fs_request {
  int dev;		               /* device=-1 if no request */
  int errors;                          /* 操作时产生的错误次数 */
  u32 sec_start;                       /* 起始扇区,注意：从0开始！！！ */
                                       /* 目前我们有1044480+514个扇区，32位目前够用... */
  int   data_size;                     /* 数据字节大小 */
  char* p_data;                        /* 数据缓冲区 */

  struct process*    p_proc_waiting;   /* 指向等待的进程 */
  struct hd_request* p_next;           /* 指向下一个请求,这是一个队列,为了实现电梯算法而留着 */
};

#define	HD_REQUEST_SIZE	sizeof(struct fs_request)

#endif
