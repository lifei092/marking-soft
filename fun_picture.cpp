#include "global_head.h"
#include "global_param.h"
#include "fun_public.h"
#include "fun_picture.h"


#define W0 1280
#define H0 480

#define B(x,y) pRGB24[0 + 3 * ((x) + 1280 * (y))]
#define G(x,y) pRGB24[1 + 3 * ((x) + 1280 * (y))]
#define R(x,y) pRGB24[2 + 3 * ((x) + 1280 * (y))]

#define Bay(x,y) p_iw8[(x) + 1280 * (y)]

void bayer_zeros(unsigned char *pBay)
{
	int x,y;
	for(x=0; x<W0; x++)
	{
		for(y=0; y<H0; y++)
		{
			if(Bay(x,y) == 0)
			{
				Bay(x,y) = 1;
			}
		}
	}
}

void bayer_copy(unsigned char *pBay, unsigned char *pRGB24, int x, int y)
{

	G(x + 0, y + 0) = Bay(x + 0, y + 0);
	G(x + 1, y + 1) = Bay(x + 1, y + 1);
	G(x + 0, y + 1) = G(x + 1, y + 0) = ((unsigned long)Bay(x + 0, y + 0) + (unsigned long)Bay(x + 1, y + 1)) / 2;
	R(x + 0, y + 0) = R(x + 1, y + 0) = R(x + 1, y + 1) = R(x + 0, y + 1) = Bay(x + 1, y + 0);
	B(x + 1, y + 1) = B(x + 0, y + 0) = B(x + 0, y + 1) = B(x + 1, y + 0) = Bay(x + 0, y + 1);
}


void bayer_bilinear(unsigned char *pBay, unsigned char *pRGB24, int x, int y)
{
	B(x + 0, y + 0) = ((unsigned long)Bay(x + 0, y + 1) + (unsigned long)Bay(x + 0, y - 1)) / 2;
	G(x + 0, y + 0) = Bay(x + 0, y + 0);
	R(x + 0, y + 0) = ((unsigned long)Bay(x - 1, y + 0) + (unsigned long)Bay(x + 1, y + 0)) / 2;

	B(x + 0, y + 1) = Bay(x + 0, y + 1);
	G(x + 0, y + 1) = ((unsigned long)Bay(x + 0, y + 0) + (unsigned long)Bay(x + 0, y + 2) + (unsigned long)Bay(x - 1, y + 1) + (unsigned long)Bay(x + 1, y + 1)) / 4;
	R(x + 0, y + 1) = ((unsigned long)Bay(x + 1, y + 0) + (unsigned long)Bay(x - 1, y + 0) + (unsigned long)Bay(x + 1, y + 2) + (unsigned long)Bay(x - 1, y + 2)) / 4;

	B(x + 1, y + 0) = ((unsigned long)Bay(x + 0, y + 1) + (unsigned long)Bay(x + 2, y + 1) + (unsigned long)Bay(x + 0, y - 1) + (unsigned long)Bay(x + 2, y - 1)) / 4;
	G(x + 1, y + 0) = ((unsigned long)Bay(x + 0, y + 0) + (unsigned long)Bay(x + 2, y + 0) + (unsigned long)Bay(x + 1, y - 1) + (unsigned long)Bay(x + 1, y + 1)) / 4;
	R(x + 1, y + 0) = Bay(x + 1, y + 0);

	B(x + 1, y + 1) = ((unsigned long)Bay(x + 0, y + 1) + (unsigned long)Bay(x + 2, y + 1)) / 2;
	G(x + 1, y + 1) = Bay(x + 1, y + 1);
	R(x + 1, y + 1) = ((unsigned long)Bay(x + 1, y + 0) + (unsigned long)Bay(x + 1, y + 2)) / 2;
}

void RAWCovert(unsigned char *p_buffer)
{
	int i = 0, j = 0;

	bayer_zeros(p_iw8);
	for (i=0; i<W0; i+=2)
	{
		for (j=0; j<H0; j+=2)
		{
			if (i==0 || j==0 || i==1278 || j==478)
			{
				bayer_copy(p_iw8, p_buffer, i, j);
			}
			else
			{
				bayer_bilinear(p_iw8, p_buffer, i, j);
			}
		}
	}
}

void ImageParamSet()
{

}

void ImageParamChange()
{

}

void image_capture()                      //相机获取图像
{
	Image rawImage;
	Image rgbImage;
	uint8_t temp_buf_num = 0xff;
	uint8_t temp_buf_num_cur = 0xff; //th增加了个临时num
	uint8_t temp_buf_cur_num = 0xff; //th增加了个临时num_cur
	uint8_t temp_work_mode = 0xff; //th增加了个临时work_mode
	unsigned int whitevalue;

	unsigned char *p_ori_temp;
	unsigned char *p_rgb_temp;
	unsigned char *p_discri = (unsigned char *)&discri_front;
	uint16_t red_now;
	uint16_t blue_now;
	struct timeval time_raw12;      //Raw12获取完毕时间
	struct timeval time_rgb_get;    //rgb获取完毕时间

	while(1)
    {
		if(g_flag_camera_init)
		{
			if((g_buf_num!=temp_buf_num) & (g_buf_num<0x20))
			{
				temp_buf_num_cur = g_buf_num;
				temp_buf_cur_num = g_buf_cur_num;
				temp_work_mode = g_work_mode;
				p_ori_temp = p_orignal[temp_buf_num_cur];
				p_rgb_temp = p_rgb[temp_buf_num_cur];

				p_ori_temp[ORIGNAL_SIZE] = 0x10;                     //clear    flag
				p_rgb_temp[RGB_SIZE] = 0x10;                         //clera    flag
				memset(&discri_front, 0, 2048);

				/********get the orignal data ******************/
				discri_front.raw12_w = W;
				discri_front.raw12_h = H;
				discri_front.raw12_mode = temp_work_mode;
				discri_front.raw12_sn = temp_buf_cur_num;
				discri_front.raw12_cam_no = 0x1;
				memcpy(p_discri+128, p_camera_buffer, 128);                 //save the camera parameter              128-191            128-191
				unsigned char* rdata;  // /****orignal data temp buffer pointer*****/
				if(((temp_work_mode & 0xf0)==0) | ((temp_work_mode & 0xf0)==0x80))   //save the orignal image data from camera
				{
					error = cam.RetrieveBuffer( &rawImage );
					if (error != PGRERROR_OK)
                    {
                        PrintError( error );
                        continue;
                    }
					rdata = rawImage.GetData();
					error = cam.ReadRegister(0x80C, &whitevalue);
					if(error != PGRERROR_OK)
					{
						PrintError(error);
						rdata = p_quick_image+2048;
					}
					discri_front.wb_blue_now = (whitevalue & 0x00FFF000)>>12;
					discri_front.wb_red_now  = (whitevalue & 0xFFF);
				}
				else if((temp_work_mode & 0xf0)==0x90)                     //save the orignal image data from refer image
				{
					rdata = p_refer_image+2048;
					memcpy(&red_now, p_refer_image+4, 2);
					memcpy(&blue_now, p_refer_image+6, 2);
					discri_front.wb_blue_now = blue_now;
					discri_front.wb_red_now  = red_now;
				}
				else if((temp_work_mode & 0xf0)==0xa0)                     //save the orignal image data from quick image
				{
					rdata = p_quick_image+2048;
					memcpy(&red_now, p_quick_image+4, 2);
					memcpy(&blue_now, p_quick_image+6, 2);
					discri_front.wb_blue_now = blue_now;
					discri_front.wb_red_now  = red_now;
				}
				else
				{
					continue;                                       //th增加了判断，防止rdata越界等
				}

				memcpy(p_ori_temp+2048, rdata, ORIG_DATA_SIZE);            //save the image data
				gettimeofday(&time_raw12, NULL);
				uint32_t timeraw = (time_raw12.tv_usec-time_serial_recv.tv_usec)+1000000*(time_raw12.tv_sec-time_serial_recv.tv_sec);
				discri_front.time_get_raw12 = timeraw;

				int i,j,f;
				int k = 0;
				for(f=0; f<ORIG_DATA_SIZE; f=f+3)
				{
					p_data8[k] = rdata[f] ;
					p_data8[k+1] = rdata[f+2];
					k = k+2;
				}
				int data8_temp;
				for(f=0; f<RAW8datasize; f++)
				{
					data8_temp = (p_data8[f])<<4;
					p_iw8[f] = p_image0[data8_temp];
				}
				RAWCovert(p_rgb_0);                             //iw8---rgb
				int kk = 2048;
				for(j=0; j<1280; j++)                       //save RGBdata to address
				{
					for(i=0; i<480; i++)
					{
						p_rgb_temp[kk] = p_rgb_0[i*1280*3+j*3];
						p_rgb_temp[kk+1] = p_rgb_0[i*1280*3+j*3+1];
						p_rgb_temp[kk+2] = p_rgb_0[i*1280*3+j*3+2];
						kk = kk+3;
					}
				}
				gettimeofday(&time_rgb_get, NULL);
				discri_front.rgb_all_w = W;
				discri_front.rgb_all_h = H;
				uint32_t timergb = (time_rgb_get.tv_usec-time_raw12.tv_usec)+1000000*(time_rgb_get.tv_sec-time_raw12.tv_sec);
				discri_front.time_raw12_to_rgb = timergb;
				/********get the blue data ******************/
				memcpy(p_discri+256, p_image_buffer, 256);



				temp_buf_num = temp_buf_num_cur;

				g_buf_num_serial = temp_buf_num_cur;
			}
		}
	}
}
