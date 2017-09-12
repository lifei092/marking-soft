#include "global_param.h"
#include "fun_public.h"

// 设置当前网卡、ip、子网掩码
void set_ipaddr(char *ifname, char *Ipaddr, char *mask)
{
	int fd0;
	struct ifreq ifr;
	struct sockaddr_in *sin;

	while((fd0 = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("set_ipaddr socket error");
	}
	memset(&ifr,0,sizeof(ifr));
	strcpy(ifr.ifr_name,ifname);
	sin = (struct sockaddr_in*)&ifr.ifr_addr;
	sin->sin_family = AF_INET;
	//IP地址
	while(inet_aton(Ipaddr, &(sin->sin_addr)) < 0)
	{
		printf("set_ipaddr inet_aton error");
	}
	while(ioctl(fd0,SIOCSIFADDR,&ifr) < 0)
	{
		printf("set_ipaddr ioctl SIOCSIFADDR error");
	}
	//子网掩码
	while(inet_aton(mask,&(sin->sin_addr)) < 0)
	{
		printf("set_ipaddr inet_pton error");
	}
	while(ioctl(fd0, SIOCSIFNETMASK, &ifr) < 0)
	{
		printf("set_ipaddr ioctl SIOCSIFNETMASK error");
	}
	close(fd0);
}

void PrintError(Error error)
{
	error.PrintErrorTrace();
}

uint8_t crc_c(unsigned char *buff,int t)
{
	int i0;
	unsigned char j0;
	unsigned char temp_crc = 0x00;
	for(i0=0; i0<t; i0++)
	{
		temp_crc = temp_crc^buff[i0];
		for(j0=0; j0<8; j0++)
		{
			if(temp_crc & 0x01)
				temp_crc=(temp_crc>>1)^0x8c;
			else
				temp_crc=(temp_crc>>1);
		}
	}
	return temp_crc;
}

uint32_t crc_c32(unsigned char *buff, uint32_t t)
{
	uint32_t genPoly32 = 0xedb88320;//crc 常数
	uint32_t i0;
	int j0;
	uint32_t temp_crc = 0;
	for(i0=0; i0<t; i0++)
	{
		temp_crc ^= (*buff);
		buff ++;
		for(j0=0; j0<8; j0++)
		{
			if(temp_crc & 0x1)
			{
				temp_crc = temp_crc>>1;
				temp_crc ^= genPoly32;
			}
			else
			{
				temp_crc = temp_crc>>1;
			}
		}
	}
	return temp_crc;
}

void cmd_error(unsigned char *p_send0, uint16_t cmd0)
{
	struct Packhead cmd_e;
	int size_cmd_e = sizeof(cmd_e);
	memset(&cmd_e, 0, size_cmd_e);
	cmd_e.cmd = cmd0;
	cmd_e.echo_flag = -3;
	cmd_e.crc = crc_c((unsigned char *)&cmd_e, size_cmd_e-1);
	memcpy(p_send0, &cmd_e, size_cmd_e);
}

void crc_error(unsigned char *p_send0, uint16_t cmd0)
{
	struct Packhead cmd_e;
	int size_cmd_e = sizeof(cmd_e);
	memset(&cmd_e, 0, size_cmd_e);
	cmd_e.cmd = cmd0;
	cmd_e.echo_flag = -2;
	cmd_e.crc = crc_c((unsigned char *)&cmd_e, size_cmd_e-1);
	memcpy(p_send0, &cmd_e, size_cmd_e);
}

void data_crc_error(unsigned char *p_send0, uint16_t cmd0)
{
	struct Packhead cmd_e;
	int size_cmd_e = sizeof(cmd_e);
	memset(&cmd_e, 0, size_cmd_e);
	cmd_e.cmd = cmd0;
	cmd_e.echo_flag = -4;
	cmd_e.crc = crc_c((unsigned char *)&cmd_e, size_cmd_e-1);
	memcpy(p_send0, &cmd_e, size_cmd_e);
}
