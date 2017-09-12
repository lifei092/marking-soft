#include "global_head.h"
#include "global_param.h"
#include "fun_public.h"
#include "fun_serial.h"

#define TRUE 0
#define FALSE -1
int set_speed(int fd, int speed)
{
	int   i;
	int   status;
	struct termios  Opt;
	tcgetattr(fd, &Opt);
	int speed_arr[2] = {B115200, B38400};
	int name_arr[2] = {115200, 38400};

	for ( i=0; i<2; i++)
	{
		if  (speed == name_arr[i])
		{
			tcflush(fd, TCIOFLUSH);
			cfsetispeed(&Opt, speed_arr[i]);
			cfsetospeed(&Opt, speed_arr[i]);
			status = tcsetattr(fd, TCSANOW, &Opt);
			if  (status != 0)
			{
				perror("tcsetattr fd");
				return FALSE;         //th返回false
			}
			tcflush(fd, TCIOFLUSH);
			return TRUE;
		}
	}

	fprintf(stderr,"Unsupported speed\n");
	return FALSE;
}

int set_parity(int fd,int databits,int stopbits,int parity)
{
	struct termios options;
	if( tcgetattr( fd, &options) != 0)
	{
		perror("SetupSerial 1");
		return FALSE;
	}
	options.c_cflag &= ~CSIZE;
	switch (databits) /*设置数据位数*/
	{
	case 7:
		options.c_cflag |= CS7;
		break;
	case 8:
		options.c_cflag |= CS8;
		break;
	default:
		fprintf(stderr,"Unsupported data size\n");
		return FALSE;
	}
	switch (parity)
	{
	case 'n':
	case 'N':
		options.c_cflag &= ~PARENB;   /* Clear parity enable */
		options.c_iflag &= ~INPCK;     /* Enable parity checking */
		break;
	case 'o':
	case 'O':
		options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/
		options.c_iflag |= INPCK;             /* Disnable parity checking */
		break;
	case 'e':
	case 'E':
		options.c_cflag |= PARENB;     /* Enable parity */
		options.c_cflag &= ~PARODD;   /* 转换为偶效验*/
		options.c_iflag |= INPCK;       /* Disnable parity checking */
		break;
	case 'S':
	case 's':  /*as no parity*/
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;break;
	default:
		fprintf(stderr,"Unsupported parity\n");
		return FALSE;
	}
	/* 设置停止位*/
	switch (stopbits)
	{
	case 1:
		options.c_cflag &= ~CSTOPB;
		break;
	case 2:
		options.c_cflag |= CSTOPB;
		break;
	default:
		fprintf(stderr,"Unsupported stop bits\n");
		return FALSE;
	}
	// important
	options.c_cflag |= CLOCAL | CREAD;
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	options.c_oflag &= ~OPOST;
	options.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	/* Set input parity option */
	if (parity != 'n')
	{
		options.c_iflag |= INPCK;
	}
	tcflush(fd, TCIFLUSH);
	//options.c_cc[VTIME] = 10; /* 设置超时*/
	options.c_cc[VMIN] = 0; /* Update the options and do it NOW */
	if (tcsetattr(fd, TCSANOW, &options) != 0)
	{
		perror("SetupSerial 3");
		return FALSE;
	}
	return TRUE;
}

int open_dev(char *dev)
{
	int	fd_temp = open( dev, O_RDWR );         //| O_NOCTTY | O_NDELAY
	if (-1 == fd_temp)
	{
		perror("Can't Open Serial Port");
		return -1;
	}
	else
	{
		return fd_temp;
	}
}

void recive_serial()//recive the serial data
{
    int cnt1;
	int i_t;
	uint8_t crc1;
	uint8_t buff1[3];
	uint8_t buff1_t = 0;
	memset(buff1, 0, sizeof(buff1));
	int count0 = 0;
	struct timeval a0;
	struct timeval a0_temp;
	unsigned long a0_temp1;
	bool begin_flag = false;
	unsigned char rcv_buff[3];         //serial buffer

	int	fd;
	char *dev = "/dev/ttyS0";
	while((fd=open_dev(dev)) == FALSE)
	{
		printf("recive_serial open error!\n");
	}
	while(set_speed(fd,115200) == FALSE)
	{
		printf("recive_serial Set Speed Error\n");
	}
	while(set_parity(fd,8,1,'N') == FALSE)
	{
		printf("recive_serial Set Parity Error\n");
	}
	printf("MSG: /dev/ttyS0 serial inited.\n");

	while(1)
	{
		if(g_flag_camera_init & g_flag_image_init)
		{
			if(begin_flag == false)
			{
				unsigned char buff_temp[255];
				cnt1 = read(fd, buff_temp, sizeof(buff_temp));
				if(cnt1 > 0)
				{
					printf("clear the serial buffer!\n");
				}
				begin_flag = true;
			}
			else
			{
				cnt1 = read(fd, rcv_buff, sizeof(rcv_buff));
				if(cnt1 == -1)
				{
					printf("Read buff error.\n");
				}
				else if(cnt1 > 0)
				{
					if(cnt1 == 3)
					{
						crc1 = crc_c(rcv_buff, 3);
						if(crc1 == 0)
						{
							gettimeofday(&a0, NULL);
							if(count0 == 0)
							{
								g_buf_num = rcv_buff[1] & 0x1f;
								g_buf_cur_num = rcv_buff[1];
								g_work_mode = rcv_buff[0];
								count0 = count0+1;
								gettimeofday(&time_serial_recv, NULL);
								a0_temp = a0;
							}
							else
							{
								a0_temp1 = a0.tv_usec-a0_temp.tv_usec+1000000*(a0.tv_sec-a0_temp.tv_sec);
								if(a0_temp1 > 20000)
								{
									g_buf_num = rcv_buff[1] & 0x1f;
									g_buf_cur_num = rcv_buff[1];
									g_work_mode = rcv_buff[0];
									count0 = count0+1;
									gettimeofday(&time_serial_recv, NULL);
									a0_temp = a0;
								}
							}
						}
						memset(rcv_buff, 0, sizeof(rcv_buff));
						memset(buff1, 0, sizeof(buff1));
						buff1_t = 0;
					}
					else
					{
						for(i_t=0; i_t<cnt1; i_t++)
						{
							buff1[buff1_t+i_t] = rcv_buff[i_t];
						}

						buff1_t = buff1_t+cnt1;
						memset(rcv_buff, 0, sizeof(rcv_buff));
						if(buff1_t == 3)
						{
							crc1 = crc_c(buff1,3);
							if(crc1 == 0)
							{
								gettimeofday(&a0, NULL);
								if(count0 == 0)
								{
									g_buf_num = buff1[1] & 0x1f;
									g_buf_cur_num = buff1[1];
									g_work_mode = buff1[0];
									count0 = count0+1;
									gettimeofday(&time_serial_recv, NULL);
									a0_temp = a0;
								}
								else
								{
									a0_temp1 = a0.tv_usec-a0_temp.tv_usec+1000000*(a0.tv_sec-a0_temp.tv_sec);
									if(a0_temp1 > 20000)
									{
										g_buf_num = buff1[1] & 0x1f;
										g_buf_cur_num = buff1[1];
										g_work_mode = buff1[0];
										count0 = count0+1;
										gettimeofday(&time_serial_recv, NULL);
										a0_temp = a0;
									}
								}
							}
							memset(buff1, 0, sizeof(buff1));
							buff1_t = 0;
						}
					}
				}
				begin_flag = true;
			}
		}
	}
	close(fd);
}

//串口发送模块
void send_serial()
{
	int fd;                            //serial port file
    unsigned char rcv_buff[5];         //serial buffer
    uint8_t result_number = 0xff;
	uint8_t result_number_tmp = 0xff;

    char dev[] = "/dev/ttyS1"; //串口一
	while((fd=open_dev(dev)) == FALSE)
	{
		printf("MSG: /dev/ttyS1 open error!\n");
	}
    while(set_speed(fd,115200) == FALSE)
	{
		printf("MSG: /dev/ttyS1 set speed error!\n");
	}
    while(set_parity(fd,8,1,'N') == FALSE)
	{
		printf("MSG: /dev/ttyS1 set parity error!\n");
	}
    printf("MSG: /dev/ttyS1 serial inited.\n");
    while(1)
    {
		if((result_number != g_buf_num_serial))
        {
			result_number_tmp = g_buf_num_serial;        //th增加 可能没必要
			rcv_buff[0]=  g_work_mode;
            rcv_buff[1] = g_buf_cur_num;
            //rcv_buff[2] = g_number_d0;
			//rcv_buff[3] = g_number_d1;
			rcv_buff[4] = crc_c(rcv_buff,4);
			write(fd, rcv_buff, sizeof(rcv_buff));
            result_number = result_number_tmp;
        }
    }
    close(fd);
}
