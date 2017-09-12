
#include "global_head.h"
#include "global_param.h"
#include "fun_init.h"
#include "fun_public.h"
#include "fun_serial.h"
#include "fun_camera.h"
#include "fun_picture.h"
#include "fun_interface.h"


void *thread0(void* args)
{
    recive_serial();
	return NULL;
}

void *thread1(void* args)
{
	camera_connect();
	image_capture();
	return NULL;
}

void *thread2(void* args)
{
    tcp_interface();
	return NULL;
}

void *thread3(void* args)
{
    send_serial();
	return NULL;
}

int main()
{
	sleep(10);
	//th note ����1�������ǰ������ip����������
	char *mask = "255.255.255.0";
	char *name0 = "eth0";
	char *ipaddr0 = CAMERA_IP;
	set_ipaddr(name0, ipaddr0, mask);
	//th note ���ü������ǰ������ip����������
	char *name1 = "eth1";
	char *ipaddr1 = PC_IP;
	set_ipaddr(name1, ipaddr1, mask);
	sleep(1);

	//th�Ż�
	if(!buffer_define())           //buffer initial
	{
		printf("buffer_define error!\n");
		return 0;
	}

	para_load();

	//pthread setting
	pthread_t tids[4]; //�߳�id
	pthread_attr_t  attr;
	struct sched_param param;
	pthread_attr_init(&attr);
	pthread_attr_setschedpolicy(&attr, SCHED_RR);
	param.sched_priority = 50;
	pthread_attr_setschedparam(&attr, &param);
	int ret0 = pthread_create(&tids[0], &attr, thread0, NULL);//���ڽ����߳�
	if(ret0 < 0)
        printf("pthread 0 is error!\n");
	else
        printf("pthread 0 is ok!\n");

	param.sched_priority = 60;
	pthread_attr_setschedparam(&attr, &param);
	int ret1 = pthread_create(&tids[1], &attr, thread1, NULL);//�����ͼ�����߳�
	if(ret1 < 0)
        printf("pthread 1 is error!\n");
	else
        printf("pthread 1 is ok!\n");

	param.sched_priority = 60;
	pthread_attr_setschedparam(&attr, &param);
	int ret2 = pthread_create(&tids[2], &attr, thread2, NULL);//�˻��߳�
	if(ret2 < 0)
        printf("pthread 2 is error!\n");
	else
        printf("pthread 2 is ok!\n");

    param.sched_priority = 50;
	pthread_attr_setschedparam(&attr, &param);
	int ret3 = pthread_create(&tids[3], &attr, thread3, NULL);//���ڷ����߳�
	if(ret3 < 0)
        printf("pthread 3 is error!\n");
	else
        printf("pthread 3 is ok!\n");

	pthread_attr_destroy(&attr);
	pthread_exit( NULL );     //�ȴ������߳��˳��󣬽��̲Ž������������ǿ�ƽ������̴߳���δ��ֹ��״̬

	munmap(g_pmmap_buf, MMAP_SIZE);             //th�ر��ͷ�
	g_pmmap_buf = NULL;
	close(g_mmap_fd);
	return 0;
}
