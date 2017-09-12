#ifndef FUN_PUBLIC_H_INCLUDED
#define FUN_PUBLIC_H_INCLUDED

#include "global_head.h"

using namespace FlyCapture2;

void set_ipaddr(char *ifname, char *Ipaddr, char *mask);

void PrintError(Error error);

uint8_t crc_c(unsigned char *buff,int t);

uint32_t crc_c32(unsigned char *buff, uint32_t t);

void cmd_error(unsigned char *p_send0, uint16_t cmd0);

void crc_error(unsigned char *p_send0, uint16_t cmd0);

void data_crc_error(unsigned char *p_send0, uint16_t cmd0);

#endif  //FUN_PUBLIC_H_INCLUDED


