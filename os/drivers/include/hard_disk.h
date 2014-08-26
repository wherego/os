
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            hard_disk.h
						    wzq
                                                    2013/9/13
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef	_HARD_DISK_H
#define _HARD_DISK_H


/************************************************************************
                         hard disk type
 ************************************************************************/
#define SECTOR_SIZE		0x200        //512
#define	PARTITION_TABLE_OFFSET	0x1BE        //分区表在boot sector中的偏移。

#define REG_DATA	0x1F0		/*读/写时为Data寄存器*/
#define REG_PRECOMP	0x1F1		/*写前预补偿寄存器*/
#define REG_ERROR	REG_PRECOMP	/*读时为Error寄存器*/
/* 	The contents of this register are valid only when the error bit
	(ERR) in the Status Register is set, except at drive power-up or at the
	completion of the drive's internal diagnostics, when the register
	contains a status code.
        When the error bit (ERR) is set, Error Register bits are interpreted as such:
	|  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
	+-----+-----+-----+-----+-----+-----+-----+-----+
	| BRK | UNC |     | IDNF|     | ABRT|TKONF| AMNF|
	+-----+-----+-----+-----+-----+-----+-----+-----+
	   |     |     |     |     |     |     |     |
	   |     |     |     |     |     |     |     `--- 0. Data address mark not found after correct ID field found
	   |     |     |     |     |     |     `--------- 1. Track 0 not found during execution of Recalibrate command
	   |     |     |     |     |     `--------------- 2. Command aborted due to drive status error or invalid command
       	   |     |     |     |     `--------------------- 3. Not used
	   |     |     |     `--------------------------- 4. Requested sector's ID field not found
	   |     |     `--------------------------------- 5. Not used
	   |     `--------------------------------------- 6. Uncorrectable data error encountered
	   `--------------------------------------------- 7. Bad block mark detected in the requested sector's ID field
*/

#define REG_SECTOR_AMT  	0x1F2		/* 扇区数寄存器 */
#define REG_SECTOR	        0x1F3		/* 扇区号寄存器 */
#define REG_LCYL	        0x1F4		/* 柱面号寄存器(低字节) */
#define REG_HCYL	        0x1F5		/* 柱面号寄存器(高字节) */

#define REG_DEVICE	        0x1F6
/*读/写时为DEVICE寄存器*/
/*	Drive | Head | LBA bits 24-27	I/O		*/
/*	|  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
	+-----+-----+-----+-----+-----+-----+-----+-----+
	|  1  |  L  |  1  | DRV | HS3 | HS2 | HS1 | HS0 |
	+-----+-----+-----+-----+-----+-----+-----+-----+
	         |           |   \_____________________/
	         |           |              |
	         |           |              `------------ If L=0, Head Select.
	         |           |                                    These four bits select the head number.
	         |           |                                    HS0 is the least significant.
	         |           |                            If L=1, HS0 through HS3 contain bit 24-27 of the LBA.
	         |           `--------------------------- Drive. 当 DRV=0, 选择drive 0 (master).
	         |                                               当 DRV=1, 选择drive 1 (slave).
	         `--------------------------------------- LBA mode. This bit selects the mode of operation.
	                                                            When L=0, addressing is by 'CHS' mode.
	                                                            When L=1, addressing is by 'LBA' mode.
*/
/* DEVICE register设置:第四位指定master(0)或者slave(1)。第六位指定CHS模式或者LBA模式。
 * 我们目前用CHS模式。
 */
#define	SET_DEVICE_REG(drv,head) (((drv) << 4) | (head & 0xF) | 0xA0)


#define REG_STATUS	        0x1F7
/*读时为Status寄存器*/
/* 	Any pending interrupt is cleared whenever this register is read.
	|  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
	+-----+-----+-----+-----+-----+-----+-----+-----+
	| BSY | DRDY|DF/SE|  #  | DRQ |     |     | ERR |
	+-----+-----+-----+-----+-----+-----+-----+-----+
	   |     |     |     |     |     |     |     |
	   |     |     |     |     |     |     |     `--- 0. Error.(an error occurred)
	   |     |     |     |     |     |     `--------- 1. Obsolete.
	   |     |     |     |     |     `--------------- 2. Obsolete.
	   |     |     |     |     `--------------------- 3. Data Request. (ready to transfer data)
	   |     |     |     `--------------------------- 4. Command dependent. (formerly DSC bit)
	   |     |     `--------------------------------- 5. Device Fault / Stream Error.
	   |     `--------------------------------------- 6. Drive Ready.
	   `--------------------------------------------- 7. Busy. If BSY=1, no other bits in the register are valid.
*/
/* 下面为Status寄存器而定义 */
#define	STATUS_BUSY	  0x80
#define	STATUS_DEVRDY	  0x40
#define	STATUS_DFSE	  0x20
#define	STATUS_DSC	  0x10
#define	STATUS_DATA_REQ	  0x08
/* #define	STATUS_CORR	  0x04 */
/* #define	STATUS_IDX	  0x02 */
#define	STATUS_ERR	  0x01

#define REG_CMD		  REG_STATUS
/* 输出时为Command寄存器 */
/*
	+--------+---------------------------------+-----------------+
	| Command| Command Description             | Parameters Used |
	| Code   |                                 | PC SC SN CY DH  |
	+--------+---------------------------------+-----------------+
	| ECh  @ | Identify Drive                  |             D   |
	| 91h    | Initialize Drive Parameters     |    V        V   |
	| 20h    | Read Sectors With Retry         |    V  V  V  V   |
	| E8h  @ | Write Buffer                    |             D   |
	+--------+---------------------------------+-----------------+

	KEY FOR SYMBOLS IN THE TABLE:
	===========================================-----=========================================================================
	PC    Register 1F1: Write Precompensation	@     These commands are optional and may not be supported by some drives.
	SC    Register 1F2: Sector Count		D     Only DRIVE parameter is valid, HEAD parameter is ignored.
	SN    Register 1F3: Sector Number		D+    Both drives execute this command regardless of the DRIVE parameter.
	CY    Register 1F4+1F5: Cylinder low + high	V     Indicates that the register contains a valid paramterer.
	DH    Register 1F6: Drive / Head
*/
#define ATA_IDENTIFY		0xEC
#define ATA_READ		0x20
#define ATA_WRITE		0x30

#define REG_DEV_CTRL	0x3F6		/*写时为Device Control寄存器*/
/*	|  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
	+-----+-----+-----+-----+-----+-----+-----+-----+
	| HOB |  -  |  -  |  -  |  -  |SRST |-IEN |  0  |
	+-----+-----+-----+-----+-----+-----+-----+-----+
	   |                             |     |
	   |                             |     `--------- Interrupt Enable.
           |                             |                  - IEN=0, and the drive is selected,
	   |                             |                    drive interrupts to the host will be enabled.
	   |                             |                  - IEN=1, or the drive is not selected,
	   |                             |                    drive interrupts to the host will be disabled.
	   |                             `--------------- Software Reset.
	   |                                                - The drive is held reset when RST=1.
	   |                                                  Setting RST=0 re-enables the drive.
	   |                                                - The host must set RST=1 and wait for at least
	   |                                                  5 microsecondsbefore setting RST=0, to ensure
	   |                                                  that the drive recognizes the reset.
	   `--------------------------------------------- HOB (High Order Byte)
                                                            - defined by 48-bit Address feature set.
*/

#define REG_ALT_STATUS	REG_DEV_CTRL	/* 读时为Alternate Status寄存器 */
/*	This register contains the same information as the Status Register.
	The only difference is that reading this register does not imply interrupt acknowledge or clear a pending interrupt.
*/

#define REG_DRV_ADDR	0x3F7		//这个是干什么的？？？？？？？？？？？？？？？？？？？？？？？？？？

/* 命令格式
      首先发送6字节的参数，最后发送1字节的命令码。不管什么命令都需要完整输出这7个字节的命令块，
   依次写入端口0x1f1~0x1f7。这点和软驱不同。 */
struct hd_cmd {
  u8	precomp;
  u8	sec_amt;      //扇区数目，0代表256。
  u8	sec;          //起始扇区，从1开始。
  u8	lycl;
  u8	hycl;
  u8	dev;          //device register
  u8	cmd;
};

/* main drive struct, one entry per drive */
struct hd_info
{
  int	dev_heads;       //硬盘磁头数
  int	track_secs;      //每磁道扇区数
  int	cylinders;       //柱面数
};

/************************************************************************
                         hard disk type
 ************************************************************************/
#define DEV_AMT    2
struct hd_info     hd_info[DEV_AMT];    //定义两个

#endif
