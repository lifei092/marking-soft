#include "global_head.h"
#include "global_param.h"
#include "fun_init.h"

bool buffer_define()
{
    int idx;
	g_pmmap_buf = NULL;
    size_t mmap_size = MMAP_SIZE; //256mbytes
    g_mmap_fd = open("/dev/zero", O_RDWR);
    if(g_mmap_fd < 0)
    {
        printf("file open error!\n");
		return false;
    }
    else
    {
        g_pmmap_buf = (unsigned char *)mmap(NULL, mmap_size, PROT_READ|PROT_WRITE, MAP_SHARED, g_mmap_fd, 0);
        if(MAP_FAILED == g_pmmap_buf)             //th
        {
            printf("buffer set error!\n");
			close(g_mmap_fd);
			return false;
        }
    }
    //inital the parameter and buffer address
    int len_tmp = 0;
    for(idx=0; idx<32; idx++)
    {
       p_orignal[idx] = g_pmmap_buf+len_tmp;
       memset((void *)p_orignal[idx], 0, ORIGNAL_SIZE+1);           //640*480*1.5+2048+1
       len_tmp = len_tmp+ORIGNAL_SIZE+1;
    }
    for(idx=0; idx<32; idx++)
    {
       p_rgb[idx] = g_pmmap_buf+len_tmp;
       memset((void *)p_rgb[idx], 0, RGB_SIZE+1);                   //640*480*3+2048+1
       len_tmp = len_tmp+RGB_SIZE+1;
    }
    for(idx=0; idx<32; idx++)
    {
       p_gray[idx] = g_pmmap_buf+len_tmp;
       memset((void *)p_gray[idx], 0, GRAY_SIZE+1);                 //640*480+2048+1
       len_tmp = len_tmp+GRAY_SIZE+1;
    }
    for(idx=0; idx<32; idx++)
    {
       p_result[idx] = g_pmmap_buf+len_tmp;
       memset((void *)p_result[idx], 0, RESULT_SIZE+1);                 //640*480+2048+1
       len_tmp = len_tmp+RESULT_SIZE+1;
    }

    p_result_debug = g_pmmap_buf+len_tmp;
    memset(p_result_debug, 0, DEBUG_IMAGE_SIZE);                        //640*480*20+2048+1
    len_tmp = len_tmp+DEBUG_IMAGE_SIZE;

    p_camera_buffer = g_pmmap_buf+len_tmp;
    memset(p_camera_buffer, 0, CAMERA_SIZE);                        //2kbytes
    len_tmp = len_tmp+CAMERA_SIZE;

    p_process_buffer = g_pmmap_buf+len_tmp;
    memset(p_process_buffer, 0, IMAMGE_PROCESS_SIZE);                        //2kbytes
    len_tmp = len_tmp+IMAMGE_PROCESS_SIZE;

    p_refer_image = g_pmmap_buf+len_tmp;
    memset(p_refer_image, 0, ORIGNAL_SIZE);                         //640*480*1.5+2048
    len_tmp = len_tmp+ORIGNAL_SIZE;

    p_quick_image = g_pmmap_buf+len_tmp;
    memset(p_quick_image, 0, ORIGNAL_SIZE);                         //640*480*1.5+2048
    len_tmp = len_tmp+ORIGNAL_SIZE;

    p_image0 = g_pmmap_buf+len_tmp;
    memset((void *)p_image0, 0, TABLE_IW);                          //1mbytes--iw12-iw8 table
    len_tmp = len_tmp+TABLE_IW;

    p_buff_tcp = g_pmmap_buf+len_tmp;
    memset(p_buff_tcp, 0, TCP_BUFF_SIZE);                           //8mbytes-----8000
    len_tmp = len_tmp+TCP_BUFF_SIZE;

    p_send = g_pmmap_buf+len_tmp;
    memset(p_send, 0, TABLE_IW);                                    //8000--------------1mbytes
    len_tmp = len_tmp+TABLE_IW;

    p_echo = g_pmmap_buf+len_tmp;
    memset(p_echo, 0, PRO_ECHO_SIZE);                               //current process information--------128bytes
    len_tmp = len_tmp+PRO_ECHO_SIZE;

    p_iw8 = g_pmmap_buf+len_tmp;
    memset(p_iw8, 0, RGB_SIZE*2);                                   //iw8 size ---------640*480
    len_tmp = len_tmp+RGB_SIZE*2;

    p_rgb_0 = g_pmmap_buf+len_tmp;
    memset(p_rgb_0, 0, RGB_SIZE);                                   //iw8 data save buffer----------------640*480*3+2048
    len_tmp = len_tmp+RGB_SIZE;

    p_data8 = g_pmmap_buf+len_tmp;
    memset(p_data8, 0, RGB_SIZE*2);
    len_tmp = len_tmp+RGB_SIZE*2;

    printf("len_tmp is %d\n",len_tmp);

    g_buf_num = 0xff;
    g_work_mode = 0xff;

    g_flag_camera_init = false;
    g_flag_process_init = false;
    g_refer_image = 0xff;
    g_refer_para = 0xff;
    memset(&discri_front, 0, sizeof(discri_front));
    return true;
}

