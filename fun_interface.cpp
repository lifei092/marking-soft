#include "global_head.h"
#include "global_param.h"
#include "fun_public.h"
#include "fun_camera.h"
#include "fun_interface.h"

void *file_write(unsigned char *buff_write, uint32_t length_c)// �������
{
	FILE *stream;   //file of sending
	int write_flag;
	stream = fopen("new_process", "wb");       //open the file only write
	if(stream == NULL)
	{
		printf("new_process open error!\n");
	}
	else
	{
		write_flag = fwrite(buff_write, length_c, 1, stream);
		if(write_flag != 1)
		{
			printf("new_process write error!\n");
			fclose(stream);									//th���ӹر�
		}
		else
		{
			fclose(stream);
			system("sync");
			chmod("new_process", S_IRWXU);
			printf("new_process write over!\n");
		}
	}
	return NULL;
}

//��ȡ��ǰ�����ļ���������
//�����ļ���Ϊ���ҵ�ǰ�����ļ��ڲ������CRC�������ȷ��һ���Զ������������p_param_info
//����ǰ����������뻺���� p_camera_buffer
//����ǰ����������뻺���� p_image_buffer
void *para_load()
{
	FILE *stream;   //file of sending
    uint32_t filesize;//file size
    stream = fopen("central_para", "rb");       //open the file only read
    if(stream < 0)
	{
		printf("ERR: Central file open error! In para_load() function.\n");
	}
    else
    {
		fseek(stream, 0, SEEK_END);  //��ȡ�ļ���С
        filesize = ftell(stream);
        rewind(stream);
        fread(p_param_info, 1, filesize, stream);
        fclose(stream);
    }
    if(filesize > 0)
    {
		uint32_t crc32_temp = 0;
        uint32_t paracrc32 = crc_c32(p_param_info, sizeof(filemiaoshu));
        if(paracrc32 == 0)
        {
			memcpy((unsigned char *)&filemiaoshu, p_param_info, sizeof(filemiaoshu));
            crc32_temp = crc_c32(p_param_info+sizeof(filemiaoshu), filemiaoshu.len);
            struct ExtIPCParaItem current_item;
            if(crc32_temp == filemiaoshu.para_crc)
            {
				g_current_count = filemiaoshu.ext_prog_no;
                uint32_t count_temp = g_current_count*sizeof(current_item)+sizeof(filemiaoshu);
                memcpy(p_camera_buffer, p_param_info+count_temp, 128);
                count_temp = count_temp+128;
                memcpy(p_image_buffer, p_param_info+count_temp, 256);
                count_temp = count_temp+256;
                CameraParamSet();
                ImageParamSet();
                g_flag_camera_init = true;
                g_flag_image_init = true;
            }
        }
    }
    return NULL;
}

//���浱ǰ�����������ļ�
//������Ϻ���µ�ǰ�����ļ����ڴ�
void *para_write1()
{
	uint32_t crc_miaoshu = crc_c32(p_param_info, 256);
    if(crc_miaoshu == 0)
    {
		memcpy((unsigned char *)&filemiaoshu, p_param_info, sizeof(filemiaoshu));
        uint32_t crc_para = crc_c32(p_param_info+sizeof(filemiaoshu), filemiaoshu.len);
        if(crc_para == filemiaoshu.para_crc)
        {
			FILE *stream;   //file of sending
            int write_flag;
            stream = fopen("central_para", "wb");       //open the file only write
            if(stream == NULL)
			{
				printf("ERR: Central_para open error! In para_white1() function.\n");
			}
            else
            {
				write_flag = fwrite(p_param_info, SYS_INFOR_SIZE, 1, stream);
                if(write_flag != 1)
				{
					printf("ERR: Central_para write error! In para_white1() function.\n");
					fclose(stream);							//th���ӹر�
				}
                else
				{
					fflush(stream);
					fsync(fileno(stream));
					fclose(stream);
					printf("MSG: Central_para file write ok!  In para_white1() function.\n");
					//para_load();
					struct ExtIPCParaItem current_item;
					g_current_count = filemiaoshu.ext_prog_no;
                    uint32_t count_temp = g_current_count*sizeof(current_item)+sizeof(filemiaoshu);
                    memcpy(p_camera_buffer, p_param_info+count_temp, 128);
                    count_temp = count_temp+128;
                    memcpy(p_image_buffer, p_param_info+count_temp, 256);
                    count_temp = count_temp+256;
                    CameraParamChange();
                    ImageParamChange();
                    g_flag_camera_init = true;
                    g_flag_image_init = true;
				}
            }
        }
    }
    return NULL;
}

//������ǰ����ţ��������ɲ����ļ�����������д�뵱ǰ�����ļ�
//д������ļ���Ҫ���µ�ǰ�����ļ�
void *para_write()
{
	int write_flag_counter;
    FILE *stream_counter;   //file of sending
    stream_counter = fopen("central_para", "wb");       //open the file only write
    if(stream_counter < 0)
	{
		printf("ERR: Central file open error! In para_write() function.\n");
	}
    else
    {
		filemiaoshu.ext_prog_no = g_current_count;
        filemiaoshu.crc = crc_c32((unsigned char *)&filemiaoshu, 252);
        memcpy(p_param_info, (unsigned char *)&filemiaoshu, 256);
        write_flag_counter = fwrite(p_param_info, SYS_INFOR_SIZE, 1, stream_counter);
        if(write_flag_counter < 0)
		{
			printf("ERR: Central_para write error! In para_write() function.\n");
			fclose(stream_counter);					//th���ӹر�
		}
        else
		{
			fflush(stream_counter);
			fsync(fileno(stream_counter));
			fclose(stream_counter);
			printf("MSG: Central_para file write ok! In para_white() function.\n");
			//para_load();
			struct ExtIPCParaItem current_item;
            g_current_count = filemiaoshu.ext_prog_no;
            uint32_t count_temp = g_current_count*sizeof(current_item)+sizeof(filemiaoshu);
            memcpy(p_camera_buffer, p_param_info+count_temp, 128);
            count_temp = count_temp+128;
            memcpy(p_image_buffer, p_param_info+count_temp, 256);
            count_temp = count_temp+256;
            CameraParamChange();
            ImageParamChange();
            g_flag_camera_init = true;
            g_flag_image_init = true;
		}
    }
    return NULL;
}

void init_lut(uint8_t buffer_gamma, uint16_t buffer_l_limit, uint16_t buffer_h_limit)
{
	union                                                                          //iw12-iw8 table set
	{
		uint32_t d32;
		uint8_t	d8[4];
	}d_u;
	uint16_t gamma;
	uint32_t a,range,llimit,hlimit;
	double r;
	uint32_t* p_des32;
	{
		gamma = (uint16_t)(buffer_gamma);
		llimit = (uint32_t)(buffer_l_limit);
		hlimit = (uint32_t)(buffer_h_limit);
		p_des32 = (uint32_t*)p_image0;
	}
	if(gamma < 80)
		gamma = 80;
	else if(gamma >200)
		gamma = 200;
	r = ((double)gamma)/100;
	if(llimit > 2000)
		llimit = 2000;
	if(hlimit < 2096)
		hlimit = 2096;
	range = hlimit - llimit;
	for(a=0; a<4096; a+=4)
	{
	    uint32_t x;
        for(x=0; x<4; x++)
        {
            double j;
            if(a < llimit)
			{
                d_u.d8[x] = 0;
			}
            else if(a > hlimit)
			{
                d_u.d8[x] = 255;
			}
            else if(range > 0)   //th�����жϣ���֪����Ҫ��
            {
                j = (255*(a-llimit)/range);
                d_u.d8[x] = (uint8_t)(pow(j/255,r)*255);
            }
        }
        *p_des32++ = d_u.d32;
	}
}

void *state_ok(unsigned char *p_send0, uint16_t cmd0) //current receive data is ok!
{

	struct Packhead cmd_e;
	int size_cmd_e = sizeof(cmd_e);
	memset(&cmd_e, 0, size_cmd_e);
	cmd_e.cmd = cmd0;
	cmd_e.crc = crc_c((unsigned char *)&cmd_e, size_cmd_e-1);
	memcpy(p_send0, &cmd_e, size_cmd_e);
	return NULL;
}

void *state_pro_check(unsigned char *p_send0) //current receive data is ok!
{
	struct Packhead cmd_e;
	int size_cmd_e = sizeof(cmd_e);
	memset(&cmd_e, 0, size_cmd_e);
	cmd_e.cmd = 0x2;
	cmd_e.data_sum = PRO_ECHO_SIZE;
	cmd_e.data_crc = crc_c32(p_echo, PRO_ECHO_SIZE);
	cmd_e.crc = crc_c((unsigned char *)&cmd_e, size_cmd_e-1);
	memcpy(p_send0, &cmd_e, size_cmd_e);
	return NULL;
}

void *state_0x20_orignal(unsigned char *p_send0, uint16_t cmd0, uint8_t number_send0) //send current orignal image data
{
	struct Packhead cmd_e;
	int size_cmd_e = sizeof(cmd_e);
	memset(&cmd_e, 0, size_cmd_e);
	cmd_e.cmd = cmd0;
	cmd_e.data_sum = ORIGNAL_SIZE;
	cmd_e.crc = crc_c((unsigned char *)&cmd_e, size_cmd_e-1);
	memcpy(p_send0, &cmd_e, size_cmd_e);
	return NULL;
}

void *state_0x20_rgb(unsigned char *p_send0, uint16_t cmd0, uint8_t number_send0)  //send current rgb image data
{
	struct Packhead cmd_e;
	int size_cmd_e = sizeof(cmd_e);
	memset(&cmd_e, 0, size_cmd_e);
	cmd_e.cmd = cmd0;
	cmd_e.data_sum = RGB_SIZE;
	cmd_e.crc = crc_c((unsigned char *)&cmd_e, size_cmd_e-1);
	memcpy(p_send0, &cmd_e, size_cmd_e);
	return NULL;
}

void *state_0x30(unsigned char *p_send0, uint16_t cmd0)  //no new image
{
	struct Packhead cmd_e;
	int size_cmd_e = sizeof(cmd_e);
	memset(&cmd_e, 0, size_cmd_e);
	cmd_e.cmd = cmd0;
	cmd_e.echo_flag = 1;
	cmd_e.crc = crc_c((unsigned char *)&cmd_e, size_cmd_e-1);
	memcpy(p_send0, &cmd_e, size_cmd_e);
	return NULL;
}

void *state_0x300(unsigned char *p_send0, uint16_t cmd0)   //no new image
{
	struct Packhead cmd_e;
	int size_cmd_e = sizeof(cmd_e);
	memset(&cmd_e, 0, size_cmd_e);
	cmd_e.cmd = cmd0;
	cmd_e.echo_flag = 5;
	cmd_e.crc = crc_c((unsigned char *)&cmd_e,size_cmd_e-1);
	memcpy(p_send0, &cmd_e, size_cmd_e);
	return NULL;
}

void *state_change_number(unsigned char *p_send0)                                   //current receive data is ok!
{
	struct Packhead cmd_e;
    int size_cmd_e = sizeof(cmd_e);
    memset(&cmd_e, 0, size_cmd_e);
    cmd_e.cmd = 0x29;
    cmd_e.crc = crc_c((unsigned char *)&cmd_e, size_cmd_e-1);
    memcpy(p_send0, &cmd_e, size_cmd_e);
    return NULL;
}

void *state_para(unsigned char *p_send0)											//current receive data is ok!
{
	struct Packhead cmd_e;
    int size_cmd_e = sizeof(cmd_e);
    memset(&cmd_e, 0, size_cmd_e);
    cmd_e.cmd = 0x3;
    cmd_e.data_sum = SYS_INFOR_SIZE;
    cmd_e.data_crc = crc_c32((unsigned char *)p_param_info, SYS_INFOR_SIZE);
    cmd_e.crc = crc_c((unsigned char *)&cmd_e, size_cmd_e-1);
    memcpy(p_send0, &cmd_e, size_cmd_e);
    return NULL;
}

void *state_online(unsigned char *p_send0)												//current receive data is ok!
{
	struct Packhead cmd_e;
    int size_cmd_e = sizeof(cmd_e);
    memset(&cmd_e, 0, size_cmd_e);
    cmd_e.data_sum = ONLINE_SIZE;
    cmd_e.data_crc = crc_c32(p_online, ONLINE_SIZE);
    cmd_e.crc = crc_c((unsigned char *)&cmd_e, size_cmd_e-1);
    memcpy(p_send0, &cmd_e, size_cmd_e);
    return NULL;
}

//�����---from interface----8000
void tcp_interface()
{
	int recv_num;                     //receive number
	uint32_t send_num;                //�������ݼ���
	unsigned char *p_buff_temp;       //��������ָ��
	uint32_t count_process = 0;         //������·��ͼ�����
	uint8_t  crc8 = 0;                  //��ͷcrc
	uint32_t crc32 = 0;                 //����crc
	uint8_t number_send = 0;              //�յ��ϴ�ͼ�����ݵ�����ʱ�洢��ǰg_buf_num
	unsigned char *flag_state;        //��ǰ����ͼ�����ݻ�����ָ��
	uint16_t cmd_temp = 0;                //��ǰ���յ�������
	uint32_t send_num_stop = 0;         //�ڼ�⵽��������ʱ��ͣ��ǰ���ͣ���ʱ�ѷ��͵�ͼ���ֽ���

	struct ExtProcessPara para_temp_old;     //����ǰ��Ԥ�������
	struct ExtProcessPara para_temp_new;     //���º��Ԥ�������------���ڱȽϺ����Ԥ������ʹ�õı��

	struct ExtCamPara camera_new;    //����ǰ������Ĳ���
	struct ExtCamPara camera_old;    //���º���������------���ڸ���iw12תiw8�ı�

	struct Packhead head_tcp;       //��ǰtcp���յ���ͷ
	int size_packhead = sizeof(head_tcp);

	struct sockaddr_in sin;         //tcp�������˵�ַ
	struct sockaddr_in pin;         //tcp�ͻ��˵�ַ
	int sock_descriptor;            //�׽ӿ�������

	int temp_sock_descriptor;       //�����ӵ��׽���������
	int temp_sock_descriptor_temp = 0;//�����ӵ��׽���������
	socklen_t address_size;         //�׽��ִ�С

	signal(SIGPIPE, SIG_IGN);        //���̻����SIGPIPE�ź�
	sock_descriptor = socket(AF_INET, SOCK_STREAM, 0);  //�����׽���
	while(sock_descriptor == -1)
	{
		perror("socket error!\n");
	}
	printf("socket ok!\n");

	bzero(&sin, sizeof(sin));         //�׽��ֳ�ʼ��
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(PC_IP);  //current computer ip
	sin.sin_port = htons(INTERFACE_PORT);          //8000

	while(bind(sock_descriptor,(struct sockaddr *)&sin,sizeof(sin)) == -1)
	{
		perror("call to bind interface");
	}
	printf("bind ok!\n");
	//tcp buffer in linux
	int nRecvBufLen = TCP_BUFF_SIZE; //����Ϊ8M
	setsockopt(sock_descriptor, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBufLen, sizeof(int));
	setsockopt(sock_descriptor, SOL_SOCKET, SO_SNDBUF, (const char*)&nRecvBufLen, sizeof(int));

	while(listen(sock_descriptor,100) == -1) //�ڶ˿�sock_descriptor����
	{
		perror("call to listen");
	}
	printf("listen is ok!\n");
	printf("accepting connections \n");
	//��ʼ�ȴ�����
	while(1)
	{
		//���������Ķ˿�sock_descriptor
		if(!((temp_sock_descriptor = accept(sock_descriptor,(struct sockaddr *)&pin, &address_size))== -1) & (pin.sin_addr.s_addr==inet_addr(INTERFACE_IP)))//--------------------------------------------------------notice
		{
			recv_num = recv(temp_sock_descriptor, p_buff_tcp, TCP_BUFF_SIZE, 0);
			if(recv_num > 0)
			{
				crc8 = crc_c(p_buff_tcp, size_packhead);
				if(crc8 == 0)
				{
					memcpy(&cmd_temp, p_buff_tcp, sizeof(cmd_temp));
					if(cmd_temp == 0)								//online
					{
						state_online(p_send);
						memcpy(p_send+size_packhead, p_online, ONLINE_SIZE);
						send_num = send(temp_sock_descriptor, p_send, size_packhead+ONLINE_SIZE, 0);
						if(send_num < 0)
							printf("send_error! 2\n");
					}
					else if(cmd_temp == 0x1)									//�������
					{
						memcpy(&head_tcp, p_buff_tcp, size_packhead);
						p_buff_temp = p_buff_tcp+recv_num;
						count_process = recv_num-size_packhead;
						while(count_process < head_tcp.data_sum)
						{
							recv_num = recv(temp_sock_descriptor,p_buff_temp,TCP_BUFF_SIZE,0);
							if(recv_num > 0)
							{
								p_buff_temp = p_buff_temp+recv_num;
								count_process = count_process+recv_num;
							}
						}
						crc32 = crc_c32(p_buff_tcp+size_packhead, head_tcp.data_sum);
						if(crc32 == head_tcp.data_crc)
						{
							if(head_tcp.data_sum != 0)                                                                      //test the struct process_echo
								file_write(p_buff_tcp+size_packhead+128,head_tcp.data_sum);    //warnning address          //write the process
							state_ok(p_send,cmd_temp);
							send_num = send(temp_sock_descriptor,p_send,size_packhead,0);
							if(send_num < 0)
								printf("send_error 1!\n");
						}
						else
						{
							data_crc_error(p_send,cmd_temp);
							send_num = send(temp_sock_descriptor,p_send,size_packhead,0);
							if(send_num < 0)
								printf("send_error 1!\n");
						}
					}
					else if(cmd_temp == 0x2)			 //������Ϣ��ѯ
					{
						state_pro_check(p_send);
						memcpy(p_send+size_packhead, p_echo, PRO_ECHO_SIZE);
						send_num = send(temp_sock_descriptor,p_send,size_packhead+128,0);
						if(send_num < 0)
							printf("send_error! 2\n");
					}
					else if(cmd_temp == 0x3)						 //����parameter��ѯ
					{
						state_para(p_send);
						memcpy(p_send+size_packhead, p_param_info, SYS_INFOR_SIZE);
						send_num = send(temp_sock_descriptor, p_send, size_packhead, 0);
						if(send_num < 0)
							printf("send_error! 3\n");
					}
					else if(cmd_temp == 0x4)			//send orignal image
					{
						if(g_flag_camera_init & g_flag_image_init & (g_buf_num<0x20))
						{
							number_send = g_buf_num;
							flag_state = p_orignal[number_send];
							if(flag_state[ORIGNAL_SIZE] == 0x10)
							{
								if(number_send != 0)
									number_send = (number_send-1) & 0x1f;
								else
									number_send = 31;
							}
							flag_state = p_orignal[number_send];
							if(flag_state[ORIGNAL_SIZE] == 0x20)
							{
								state_0x20_orignal(p_send, cmd_temp, number_send);
								send_num = send(temp_sock_descriptor, p_send, size_packhead, 0);
								if(send_num < 0)
									printf("send_error! 4\n");
								send_num = 0;
								send_num_stop = 0;
								while(send_num < ORIGNAL_SIZE)
								{
									//if(g_flag_blue_img == false)
									{
										send_num_stop = send(temp_sock_descriptor, flag_state+send_num, ORIGNAL_SIZE-send_num, 0);
										if(send_num_stop > 0)
											send_num = send_num+send_num_stop;
									}
								}
								flag_state[ORIGNAL_SIZE] = 0x30;
							}
							else
							{
								state_0x30(p_send, cmd_temp);                     //no new image
								send_num = send(temp_sock_descriptor, p_send, size_packhead, 0);
								if(send_num < 0)
									printf("send_error! 4\n");
							}
						}
						else
						{
							state_0x300(p_send,cmd_temp);
							send_num = send(temp_sock_descriptor, p_send, size_packhead, 0);
							if(send_num < 0)
								printf("send_error! 4\n");
						}
					}
					else if(cmd_temp == 0x5)									//send rgb image
					{
						if(g_flag_camera_init & g_flag_image_init & (g_buf_num<0x20))
						{
							number_send = g_buf_num;
							flag_state = p_rgb[number_send];
							if(flag_state[RGB_SIZE] == 0x10)
							{
								if(number_send != 0)
									number_send = (number_send-1) & 0x1f;
								else
									number_send = 31;
							}
							flag_state = p_rgb[number_send];
							if(flag_state[RGB_SIZE] == 0x20)
							{                                                  //current image isready
								state_0x20_rgb(p_send, cmd_temp, number_send);
								send_num = send(temp_sock_descriptor, p_send, size_packhead, 0);
								if(send_num < 0)
									printf("send_error! 5\n");
								send_num = 0;
								send_num_stop = 0;
								while(send_num < RGB_SIZE)
								{
									//if(g_flag_blue_img == false)
									{
										send_num_stop = send(temp_sock_descriptor, flag_state+send_num, RGB_SIZE-send_num, 0);
										if(send_num_stop > 0)
											send_num = send_num+send_num_stop;
									}
								}
								flag_state[RGB_SIZE] = 0x30;
							}
							else
							{                                                                  //no new image
								state_0x30(p_send, cmd_temp);
								send_num = send(temp_sock_descriptor,p_send,size_packhead,0);
								if(send_num < 0)
									printf("send_error! 5\n");
							}
						}
						else
						{
							state_0x30(p_send, cmd_temp);
							send_num = send(temp_sock_descriptor,p_send,size_packhead,0);
							if(send_num < 0)
								printf("send_error! 4\n");
						}
					}
					else if(cmd_temp == 0x20)   //�������������
					{
						memcpy(&head_tcp, p_buff_tcp, size_packhead);
						count_process = 0;
						count_process = count_process+recv_num;
						while(count_process < (size_packhead+head_tcp.data_sum))
						{
							recv_num = recv(temp_sock_descriptor,p_buff_tcp+count_process,TCP_BUFF_SIZE,0);
							if(recv_num > 0)
								count_process = count_process+recv_num;
						}
						crc32 = crc_c32(p_buff_tcp+size_packhead,head_tcp.data_sum);
						if(crc32 == head_tcp.data_crc)
						{
							memcpy(&camera_old, p_camera_buffer, sizeof(camera_old));
                            memcpy(p_camera_buffer, p_buff_tcp+size_packhead, sizeof(camera_new));
                            memcpy(&camera_new, p_camera_buffer, sizeof(camera_new));
							if(g_flag_camera_init == false)
							{
                                CameraParamSet();
								init_lut(camera_new.tran12_8_gamma, camera_new.tran12_8_l_limit, camera_new.tran12_8_h_limit);
							}
							else
							{
                                CameraParamChange();
                                if((camera_new.tran12_8_gamma!=camera_old.tran12_8_gamma)|(camera_new.tran12_8_l_limit!=camera_old.tran12_8_l_limit)|(camera_new.tran12_8_h_limit!= camera_old.tran12_8_h_limit))
                                    init_lut(camera_new.tran12_8_gamma,camera_new.tran12_8_l_limit,camera_new.tran12_8_h_limit);
							}
							g_flag_camera_init == true;
							state_ok(p_send, cmd_temp);
							send_num = send(temp_sock_descriptor, p_send, size_packhead,0);
							if(send_num < 0)
								printf("send_error!\n");
						}
						else
						{
							data_crc_error(p_send, cmd_temp);
							send_num = send(temp_sock_descriptor,p_send,size_packhead,0);
							if(send_num < 0)
								printf("send_error!\n");
						}
					}
	            	else if(cmd_temp == 0x21) //ͼ�������������
					{
						memcpy(&head_tcp, p_buff_tcp, size_packhead);
						count_process = 0;
						count_process = count_process+recv_num;
						while(count_process < (size_packhead+head_tcp.data_sum))
						{
							recv_num = recv(temp_sock_descriptor,p_buff_tcp+count_process,TCP_BUFF_SIZE,0);
							if(recv_num > 0)
								count_process = count_process+recv_num;
						}
						crc32 = crc_c32(p_buff_tcp+size_packhead,head_tcp.data_sum);
						if(crc32 == head_tcp.data_crc)
						{
							memcpy(&para_temp_old, p_image_buffer, sizeof(para_temp_old));
							memcpy(p_image_buffer, p_buff_tcp+size_packhead, sizeof(para_temp_old));
							memcpy(&para_temp_new, p_image_buffer, sizeof(para_temp_new));
							if(g_flag_image_init == false)
                            {
                                ImageParamSet();
                            }
                            else
                            {
                                ImageParamChange();
                            }
                            g_flag_image_init = true;
							state_ok(p_send, cmd_temp);
							send_num = send(temp_sock_descriptor,p_send,size_packhead,0);
							if(send_num < 0)
								printf("send_error!\n");
						}
						else
						{
							data_crc_error(p_send, cmd_temp);
							send_num = send(temp_sock_descriptor, p_send, size_packhead,0);
							if(send_num < 0)
								printf("send_error!\n");
						}
					}
					else if(cmd_temp==0x24)//�̱������ͼ����
					{
						memcpy(&head_tcp, p_buff_tcp, size_packhead);
						count_process = 0;
						count_process = count_process+recv_num;
						while(count_process < (size_packhead+head_tcp.data_sum))
						{
							recv_num = recv(temp_sock_descriptor,p_buff_tcp+count_process,TCP_BUFF_SIZE,0);
							if(recv_num > 0)
								count_process = count_process+recv_num;
						}
						crc32 = crc_c32(p_buff_tcp+size_packhead,head_tcp.data_sum);
						if(crc32 == head_tcp.data_crc)
						{
							memcpy(p_quick_image,p_buff_tcp+size_packhead,ORIGNAL_SIZE);
							state_ok(p_send, cmd_temp);
							send_num = send(temp_sock_descriptor,p_send,size_packhead,0);
							if(send_num < 0)
								printf("send_error!\n");
						}
						else
						{
							data_crc_error(p_send,cmd_temp);
							send_num = send(temp_sock_descriptor,p_send,size_packhead,0);
							if(send_num < 0)
								printf("send_error!\n");
						}
					}
					else if(cmd_temp == 0x25)											//�����ļ���������
					{
						count_process = 0;
						memcpy(&head_tcp, p_buff_tcp, size_packhead);
						count_process = count_process+recv_num;
						while(count_process < (size_packhead+head_tcp.data_sum))
						{
							recv_num = recv(temp_sock_descriptor,p_buff_tcp+count_process,MAXSIZE,0);
							if(recv_num > 0)
								count_process = count_process+recv_num;
						}
						crc32 = crc_c32(p_buff_tcp+size_packhead,head_tcp.data_sum);
						if(crc32 == head_tcp.data_crc)
						{
							state_ok(p_send, cmd_temp);
							send_num = send(temp_sock_descriptor,p_send,size_packhead,0);
							if(send_num < 0)
								printf("send_error!\n");
							uint32_t crc_1 = crc_c32(p_buff_tcp+size_packhead,256);
							uint32_t crc_2 = crc_c32(p_buff_tcp+size_packhead+256,SYS_INFOR_SIZE-256);
							uint32_t crc_3;
							memcpy(&crc_3, p_buff_tcp+size_packhead+4, 4);
							if((crc_1==0) & (crc_2==crc_3))
							{
								memcpy(p_param_info, p_buff_tcp+size_packhead, head_tcp.data_sum);
								para_write1();
								g_refer_para = (g_refer_para+1) & 0x1f;
								g_refer_image = (g_refer_image+1) & 0x1f;
							}
						}
						else
						{
							data_crc_error(p_send, cmd_temp);
							send_num = send(temp_sock_descriptor,p_send,size_packhead,0);
							if(send_num < 0)
								printf("send_error!\n");
						}
					}
					else if(cmd_temp == 0x29)										//���ĵ�ǰ�����
					{
						memcpy(&head_tcp, p_buff_tcp, size_packhead);
						g_current_count = head_tcp.ext & 0x1f;
						state_change_number(p_send);
						send_num = send(temp_sock_descriptor,p_send,size_packhead,0);
						if(send_num < 0)
							printf("send_error! 2\n");
						para_write();
						g_refer_para = (g_refer_para+1) & 0x1f;
						g_refer_image = (g_refer_image+1) & 0x1f;
					}
					else													 //cmd����ʶ
					{
						cmd_error(p_send,cmd_temp);
						send_num = send(temp_sock_descriptor,p_send,size_packhead,0);
						if(send_num < 0)
							printf("send_error!\n");
					}
				}
				else
				{
					crc_error(p_send,cmd_temp);
					send_num = send(temp_sock_descriptor,p_send,size_packhead,0);
					if(send_num < 0)
						printf("send_error 1!\n");
				}
			}
			if(temp_sock_descriptor_temp != temp_sock_descriptor)
			{
				close(temp_sock_descriptor_temp);
			}
			temp_sock_descriptor_temp = temp_sock_descriptor;
			memset(p_buff_tcp, 0, sizeof(recv_num));
			p_buff_temp = p_buff_tcp;
			count_process = 0;
			crc8 = 0;
			crc32 = 0;
		}
	}
	close(sock_descriptor);             //th�ر�
}
