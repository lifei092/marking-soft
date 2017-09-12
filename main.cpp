
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
	//th note 设置1号相机当前网卡、ip、子网掩码
	char *mask = "255.255.255.0";
	char *name0 = "eth0";
	char *ipaddr0 = CAMERA_IP;
	set_ipaddr(name0, ipaddr0, mask);
	//th note 设置计算机当前网卡、ip、子网掩码
	char *name1 = "eth1";
	char *ipaddr1 = PC_IP;
	set_ipaddr(name1, ipaddr1, mask);
	sleep(1);

	//th优化
	if(!buffer_define())           //buffer initial
	{
		printf("buffer_define error!\n");
		return 0;
	}

	para_load();

	//pthread setting
	pthread_t tids[4]; //线程id
	pthread_attr_t  attr;
	struct sched_param param;
	pthread_attr_init(&attr);
	pthread_attr_setschedpolicy(&attr, SCHED_RR);
	param.sched_priority = 50;
	pthread_attr_setschedparam(&attr, &param);
	int ret0 = pthread_create(&tids[0], &attr, thread0, NULL);//串口接收线程
	if(ret0 < 0)
        printf("pthread 0 is error!\n");
	else
        printf("pthread 0 is ok!\n");

	param.sched_priority = 60;
	pthread_attr_setschedparam(&attr, &param);
	int ret1 = pthread_create(&tids[1], &attr, thread1, NULL);//相机、图像处理线程
	if(ret1 < 0)
        printf("pthread 1 is error!\n");
	else
        printf("pthread 1 is ok!\n");

	param.sched_priority = 60;
	pthread_attr_setschedparam(&attr, &param);
	int ret2 = pthread_create(&tids[2], &attr, thread2, NULL);//人机线程
	if(ret2 < 0)
        printf("pthread 2 is error!\n");
	else
        printf("pthread 2 is ok!\n");

    param.sched_priority = 50;
	pthread_attr_setschedparam(&attr, &param);
	int ret3 = pthread_create(&tids[3], &attr, thread3, NULL);//串口发送线程
	if(ret3 < 0)
        printf("pthread 3 is error!\n");
	else
        printf("pthread 3 is ok!\n");

	pthread_attr_destroy(&attr);
	pthread_exit( NULL );     //等待各个线程退出后，进程才结束，否则进程强制结束，线程处于未终止的状态

	munmap(g_pmmap_buf, MMAP_SIZE);             //th关闭释放
	g_pmmap_buf = NULL;
	close(g_mmap_fd);
	return 0;
}
