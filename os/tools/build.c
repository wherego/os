
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                              build.c
                                                         wzq
                                                         2013/9/29
  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/fs.h>
#include <unistd.h>
#include <fcntl.h>

#define LOADER_SECTS   16      /* loader占据 16个扇区 */
#define KERNEL_SECTS  128      /* kernel占据128个扇区 */

void die(char * str)
{
  fprintf(stderr,"%s\n",str);
  exit(1);
}

int main(int argc,char* argv[])
{
  int  i, j, c, id;
  char buf[512];

/* 参数是三个，分别是../boot/boot.bin ../boot/loader.bin ./kernel.bin */
  if (argc != 4)
    die("Usage: ./build ../boot/boot.bin ../boot/loader.bin ./kernel.bin > Image");

/* 初始化 */
  j = sizeof(buf);
  for (i=0; i<j; i++)
    buf[i]=0;

/* 以下读取boot.bin boot为纯二进制文件 */
  if ((id=open(argv[1],O_RDONLY,0)) < 0)
    die("Unable to open 'boot.bin'");

  c = read(id,buf,sizeof(buf));

  if (c != 512)
    die("Boot block must be exactly 512 bytes");

  if ((*(unsigned short *)(buf+510)) != 0xAA55)
    die("Boot block hasn't got boot flag (0xAA55)");

  c = write(1, buf, 512);

  if (c != 512)
    die("Write boot failed");

  fprintf(stderr,"BOOT:%7d bytes have been written.\n", c);

  close (id);

/* 以下读取loader.bin loader为纯二进制文件 */
  if ((id=open(argv[2],O_RDONLY,0))<0)
    die("Unable to open 'loader.bin'");

  for (i=0; (c=read(id,buf,sizeof(buf)))>0; i+=c)
    {
      if (write(1,buf,c)!=c)
  	die("Write loader failed");
    }

  if (i > LOADER_SECTS*512)
    die("Loader exceeds LOADER_SECTS sectors");

  c = 512 - i%512;

  for (j=0; j<sizeof(buf); j++)
    buf[j]=0;

  if (write(1,buf,c)!=c)
    die("Write loader failed");

  c += i;
  c /= 512;
  c = LOADER_SECTS - c;

  for (j=0 ; j<c; j++)
    {
      if (write(1, buf, sizeof(buf)) != sizeof(buf))
  	die("Write loader failed");
    }

  fprintf(stderr,"LOADER:%5d bytes have been written.\n", i);

  close (id);

/* 以下读取kernel.bin */
  if ((id=open(argv[3],O_RDONLY,0))<0)
    die("Unable to open 'kernel.bin'");

  for (i=0; (c=read(id,buf,sizeof(buf)))>0; i+=c)
    {
      if (write(1,buf,c)!=c)
  	die("Write kernel failed");
    }

  if (i > KERNEL_SECTS*512)
    die("Kernel exceeds KERNEL_SECTS sectors");

  c = 512 - i%512;

  for (j=0; j<sizeof(buf); j++)
    buf[j]=0;

  if (write(1,buf,c)!=c)
    die("Write kernel failed");

  c += i;
  c /= 512;
  c = LOADER_SECTS - c;

  for (j=0 ; j<c; j++)
    {
      if (write(1, buf, sizeof(buf)) != sizeof(buf))
  	die("Write kernel failed");
    }

  fprintf(stderr,"KERNEL:%5d bytes have been written.\n", i);

  close (id);

  return 0;
}
