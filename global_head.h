#ifndef GLOBAL_HEAD_H_INCLUDED
#define GLOBAL_HEAD_H_INCLUDED

#include "stdio.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "unistd.h"
#include "errno.h"
#include "string.h"
#include "stdlib.h"
#include "sys/time.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "pthread.h"
#include "sys/mman.h"
#include "termios.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>
#include "stdbool.h"
#include "signal.h"

#include <sys/ioctl.h>
#include <net/if.h>
#include <net/route.h>

#include "FlyCapture2.h"
#include "iostream"
#include "math.h"

#define MMAP_SIZE      268435456					  //映射内存大小 256mbytes
#define ONLINE_SIZE         1024						//online data size 1024
#define RGB_SIZE       923648                         //640*480*3+2048

#define TCP_BUFF_SIZE  8388608                          //8*1024*1024 8M
#define TABLE_IW       1048576                         //1*1024*1024 1M

#define DEBUG_IMAGE_SIZE       6146048                         //640*480*20+2048

#define RAW8datasize   307200                         //640*480
#define ORIGNAL_SIZE   462848                         //640*480*1.5+2048
#define ORIG_DATA_SIZE 460800                           //640*480*1.5

#define GRAY_SIZE 309248                           //640*480+2048=
#define RESULT_SIZE 309248                           //640*480+2048=

#define CAMERA_SIZE    2048

#define IMAMGE_PROCESS_SIZE    2048

#define PRO_ECHO_SIZE  128

#define SYS_INFOR_SIZE      3*1024*1024					//current parameter size----3mbytes

#define MAXSIZE TCP_BUFF_SIZE

#define H			   1280							  // 定义要转换图像行数
#define W			   480							  // 定义要转换图像列数

#define INTERFACE_IP   "192.168.1.40"                 //人机 ip
#define PC_IP	       "192.168.1.30"				  //中心PC ip
#define CAMERA_IP      "192.168.0.20"                 //camera ip
#define INTERFACE_PORT 8000							  //from interface

#endif //GLOBAL_HEAD_H_INCLUDED
