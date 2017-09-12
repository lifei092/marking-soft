#ifndef GLOBAL_PARAM_H_INCLUDED
#define GLOBAL_PARAM_H_INCLUDED

#include "global_head.h"


using namespace FlyCapture2;
using namespace std;

GigECamera cam;
Error error;
char ipbuf[3];
char macbuf[2];

int			   g_mmap_fd;					//mmap file
unsigned char *g_pmmap_buf;					//mmap address

unsigned char *p_orignal[32];				//原始图像缓存区，iw12--每个缓存区大小为：640*480*1.5=460800bytes+2k=462848bytes  0-2047--附加信息地址  2048-462847--图像数据地址
											//末端添加一位填写当前缓存区标志 0x10--clear  0x20--ready  0x30--sended
unsigned char *p_rgb[32];					//rgb图像缓存区，每个缓存区大小为:640*480*3=921600bytes+2k = 923648bytes  0-2047--附加信息地址   2048-923647--图像数据地址
											//末端添加一位填写当前缓存区标志 0x10--clear  0x20--ready  0x30--sended
unsigned char *p_gray[32];					//gray图像缓存区，每个缓存区大小为:640*480=307200bytes+2k=309248bytes  0-2047--附加信息地址  2048-309247--图像数据地址
											//末端添加一位填写当前缓存区标志 0x10--clear  0x20--ready  0x30--sended
unsigned char *p_result[32];				//结果图像缓存区，每个缓存区大小为:640*480=307200bytes+2k=309248bytes  0-2047--附加信息地址  2048-309247--图像数据地址
											//末端添加一位填写当前缓存区标志 0x10--clear  0x20--ready  0x30--sended
unsigned char *p_result_debug;			    //debug图像缓存区，每个缓存区大小为:640*480*20=6144000bytes+2k=6146048bytes  0-2047--附加信息地址  2048-6146047--图像数据地址
											//末端添加一位填写当前缓存区标志 0x10--clear  0x20--ready  0x30--sended

unsigned char *p_iw8;						//iw8 data buffer
unsigned char *p_rgb_0;						//iw12-iw8 mid buffer
unsigned char *p_data8;

unsigned char *p_camera_buffer;		     		//相机参数缓存区----128B
unsigned char *p_image_buffer;			    	//处理参数缓存
unsigned char *p_refer_image;				//参考图像缓存区iw12--640*480*1.5=460800bytes + 2048 = 462848
unsigned char *p_quick_image;				//快照图缓存区 iw12---640*480*1.5=460800bytes + 2048 = 462848

unsigned char *p_param_info;					//存储参数缓存指针

unsigned char *p_image0;					//iw12-iw8 table----1m

unsigned char *p_buff_tcp;									//tcp recv buff---8mbytes------8000
unsigned char *p_send;										//tcp send buff-1mbytes--------8000

unsigned char *p_echo;										//程序描述,current save the process miaoshu
unsigned char *p_online;									//current online data

uint8_t        g_refer_image;								//有新的参考图像标识,自增一，取低五位
															//检测到此标识变化后将重新写当前参考图像文件并向前端发送参考图像；
uint8_t		   g_refer_para;								//更新当前中心参数文件标识,自增一，取低五位
															//检测到此标识变化后将重新写当前中心参数文件并向前端发送相机和预处理参数；

uint8_t		   g_buf_num;				    //当前buffer编号  number=numer_ori & 0x1f
uint8_t		   g_buf_cur_num;				//当前触发编号
uint8_t		   g_work_mode;					//工作模式  save the orignal image data from camera、refer image or quick image
uint8_t		   g_buf_num_serial;			//每次中心处理完毕后此标识=g_buf_num，串口部分检测到此标识的变化则发送当前判定结果

bool		   g_flag_camera_init;			//相机初始化标识
bool		   g_flag_image_init;			//图像处理参数初始化标识

uint8_t		   g_current_count;				//当前程序号

struct Packhead
{
    uint16_t cmd;							//命令
    int16_t  echo_flag;						//0,表示当前设置/更新成功，<0，表示当前设置失败
    uint32_t data_sum;						//该次PackData部分的数据长度
    uint32_t data_crc;						//PackData部分的crc，crc常数$88320(0x)
    uint16_t ext;							//cmd的附加信息
    uint8_t  rsv;							//保留,简易附加信息
    uint8_t  crc;							//包头部分crc，8位crc，crc常数$8c
};//16bytes

struct ExtCamPara
{
    uint8_t cam_brightness;					//范围0~25，对应相机设置的取值范围为0~25%；
    uint8_t cam_shutter;					//范围0~255，对应相机shutter0~255us；
    uint8_t cam_gamma;					    //范围0~4，缺省为0；
    uint8_t tran12_8_gamma;					//用作raw12-->raw8变换的gamma，范围80~200；
    int16_t cam_exposure;					//范围-7585~2414，对应相机的-7.585~2.414ev；
    int16_t cam_hue;						//范围-180~180，对应相机-180~180deg，缺省为0；
    uint16_t cam_sharpness;					//范围0~4095，Raw12格式时默认为1024；
    uint16_t cam_saturation;				//范围0~400，对应0~400%，缺省为100%；
    uint16_t cam_gain;						//范围-5753~13034，对应-5.753~13.034db；
    uint16_t cam_wb_red;					//bit[9:0]范围0~1023，缺省642，bit[15] 0 off, 1 on，bit[14] 1 auto；
    uint16_t cam_wb_blue;					//bit[9:0]范围0~1023，缺省642，bit[15] 0 off, 1 on，bit[14] 1 auto；
    uint16_t cam_roi_w;						//范围0~1280，缺省1280且不可更改；
    uint16_t cam_roi_w_offset;				//范围0~1280，缺省0；
    uint16_t cam_roi_h ;					//范围0~960，缺省480且不可更改；
    uint16_t cam_roi_h_offset;				//范围0~960，缺省240；
    uint16_t tran12_8_l_limit;				//用作raw12-->raw8变换的下部起点；
    uint16_t tran12_8_h_limit;				//用作raw12-->raw8变换的上部端点；
    uint16_t rsv1[47];						//保留；
    uint32_t crc32;							//32位crc校验, crc常数$88320
};//128 bytes long

struct Discriptor2K_Label
{
/*raw12图像特征描述32bytes*/
    uint16_t raw12_w;						//前端raw12的宽度，现阶段固定为1280;
    uint16_t raw12_h;						//前端raw12的高度，现阶段固定为480;
    uint16_t wb_red_now;					//当前相机使用的白平衡red系数;
    uint16_t wb_blue_now;				    //当前相机使用的白平衡blue系数;
    uint8_t  raw12_mode;					//raw12数据来源，工作模式等的定义;
											//bit[7:4]：
											//4'b0000表示标准工作模式，图像源来自相机，触发信号来自外部触发器；
											//4'b0???保留；
											//4'b1000表示内部触发模式0，虚拟触发，图像源来自相机；
											//4'b1001表示内部触发模式1，虚拟触发，图像源来自保存的标准图像(参照图)；
											//4'b1010表示内部触发模式2，虚拟触发，图像源来自剪贴板(快照图)；
											//4'b1011--4'b1111保留；
											//byte0[3]保留;
											//byte0[2:0]触发器号0--7；
    uint8_t  raw12_sn;						//序号0--255，各个前端工控机buf编号使用其中的低5bit;
    uint8_t  raw12_cam_no;					//本raw12图像源自几号相机,商标检测1~4号相机
    uint8_t  rsv0[21];
/*rgb全尺寸图像特征描述32bytes*/
    uint16_t rgb_all_w;						//前端rgb全尺寸图像的宽度，现阶段固定为480;
    uint16_t rgb_all_h;						//前端rgb全尺寸图像的高度，现阶段固定为1280;
    uint8_t  rsv1[28];
/* 本图像使用的相机参数128bytes*/
    uint8_t  camera_buffer[128];
/*本图像使用的处理参数256bytes*/
    uint8_t  process_buffer[256];
/*本图像处理结果 256 bytes*/
    uint8_t  result_buffer[256];

/*处理时间标记 64bytes*/
    uint32_t time_ipc1_x;					//前端耗时(接收到串口标志到传输完图像数据),单位us;---768
    uint32_t time_get_raw12;				//raw12数据接收耗时(接收到串口标志到获取完raw12数据),单位us;;---772
    uint32_t time_raw12_to_rgb;				// raw12->rgb耗时,单位us;----776
    uint8_t  rsv2[32];
/*保留*/
    uint8_t  rsv3[1296];					//保留;
    uint32_t crc;
};//2048bytes long

struct ExtProcessPara
{
	//m_nFlagDebug各bit含义：
	//|GlobalDebug |Result |Debug13|Debug12|Debug11|Debug10|Debug9|Debug8|Debug7|Debug6|Debug5|Debug4|Slant |Show|Debug3|Debug2|Debug1|Adjust|OrgPic
	uint32_t m_nFlagDebug;/*OrgPic: 0x0000 0001
						 *Adjust: 0x0000 0003
						 *Debug1: 0x0000 0007
						 *Debug2: 0x0000 000F
						 *Debug3: 0x0000 001F
						 *Show :  0x0000 003F
						 *Slant:  0x0000 007F
						 *Debug4: 0x0000 00FF
						 *Debug5: 0x0000 01FF
						 *Debug6: 0x0000 03FF
						 *Debug7: 0x0000 07FF
						 *Debug8: 0x0000 0FFF
						 *Debug9: 0x0000 1FFF
						 *Debug10:0x0000 3FFF
						 *Debug11:0x0000 7FFF
						 *Debug12:0x0000 FFFF
						 *Debug13:0x0001 FFFF
						 *Result :0x0003 FFFF
						 *GlobalDebug: 0x0007 FFFF
						 */

// Model0: 基本参数设置
	uint8_t  m_nCrop_sw;                //生成形状算子选择开关，1(0001): Rect  2(0010): Ellip  4(0100): Cir  8(1000): Debug
    uint16_t m_nParam1;                 // m_nParam1/2/3/4/5为生成矩形等区域算子的参数
    uint16_t m_nParam2;
    uint16_t m_nParam3;
    uint16_t m_nParam4;
    uint16_t m_nParam5;

	uint8_t m_nFlagAlgorithm;           //算法选择标志位：|m_bSp_sw|m_bFe_sw|m_bSlant_sw|m_bHisto_sw|
                                        //m_bHisto_sw ：白平衡算子使用开关
                                        //m_bSlant_sw : 字符倾斜角度矫正
                                        //m_bFe_sw ： 特征提取算子选择开关
                                        //m_bSp_sw ： 去椒盐噪声算子选择开关

    uint16_t m_nROI_w;                  //字符文本区域的宽度
    uint16_t m_nROI_h;                  //字符文本区域的高度
    uint8_t m_nCol_nums;                //字符行数
    int8_t m_nOrt_min;                  //文本倾斜角度最小值
    int8_t m_nOrt_max;                  //文本倾斜角度最大值
    uint8_t m_nChar_h;                  //字符高度
//Model1: 寻找整个文本区域
	uint8_t m_nMean_sp1;                //去椒盐噪声算子内参数，表示模板大小
    uint8_t m_nMean_img1;               //均值滤波算子内参数，表示模板大小
    uint8_t m_nMin_max_gry1;            //获取灰度图最大最小灰度值算子内参数，表示灰度范围因子
    uint8_t m_nDyn1;                    //动态阈值二值化算子内参数，表示模板大小
    uint8_t m_nDil_h1;                  //膨胀算子内参数，表示模板高度
    uint8_t m_nDil_w1;                  //膨胀算子内参数，表示模板宽度

    uint16_t m_nSlt_area_Min1;          //连通域选择过滤算子内参数，表示面积最小值
    uint16_t m_nSlt_area_Max1;          //连通域选择过滤算子内参数，表示面积最大值
    uint16_t m_nSlt_h_Min1;             //连通域选择过滤算子内参数，表示高度最小值
    uint16_t m_nSlt_h_Max1;             //连通域选择过滤算子内参数，表示高度最大值
    uint16_t m_nSlt_w_Min1;             //连通域选择过滤算子内参数，表示宽度最小值
    uint16_t m_nSlt_w_Max1;             //连通域选择过滤算子内参数，表示宽度最大值
// Model2；将多行字符分成多个单行字符区域
	uint8_t m_nMean_sp2;                //去椒盐噪声算子内参数，表示模板大小
    uint8_t m_nMin_max_gry2;            //获取灰度图最大最小灰度值算子内参数，表示灰度范围因子
    uint8_t m_nMean_img2;               //均值滤波算子内参数，表示模板大小
    uint8_t m_nDot2;                    //动态阈值二值化算子或圆点增强算子内参数，表示模板大小
    uint8_t m_nThresh_min;              //圆点增强算子内参数，表示灰度值范围最小值
    uint8_t m_nThresh_max;              //圆点增强算子内参数，表示灰度值范围最大值

    uint8_t m_nDil_h2;                  //膨胀算子内参数，表示模板高度
    uint8_t m_nDil_w2;                  //膨胀算子内参数，表示模板宽度
    uint8_t m_nDil_shift;               //膨胀算子内参数，表示模板高度增量
    uint8_t m_nEro_w2;                  //腐蚀算子内参数，表示模板宽度
    uint8_t m_nEro_h2;                  //腐蚀算子内参数，表示模板高度

    uint16_t m_nSlt_area_Min2;          //连通域选择过滤算子内参数，表示面积最小值
    uint16_t m_nSlt_area_Max2;          //连通域选择过滤算子内参数，表示面积最大值
    uint16_t m_nSlt_w_Min2;             //连通域选择过滤算子内参数，表示宽度最小值
    uint16_t m_nSlt_w_Max2;             //连通域选择过滤算子内参数，表示宽度最大值
// Model3；处理单个字符以及字符识别
	uint8_t m_nDil_w3_1;                //针对第一行字符的膨胀算子内参数，表示模板宽度
    uint8_t m_nDil_w3_2;                //针对第二行字符的膨胀算子内参数，表示模板宽度
    uint8_t m_nDil_w3_3;                //针对第三行字符的膨胀算子内参数，表示模板宽度
    uint8_t m_nDil_w3_4;                //针对第四行字符的膨胀算子内参数，表示模板宽度
    uint8_t m_nDil_h3_1;                //针对第一行字符的膨胀算子内参数，表示模板高度
    uint8_t m_nDil_h3_2;                //针对第二行字符的膨胀算子内参数，表示模板高度
    uint8_t m_nDil_h3_3;                //针对第三行字符的膨胀算子内参数，表示模板高度
    uint8_t m_nDil_h3_4;                //针对第四行字符的膨胀算子内参数，表示模板高度

    uint8_t m_nThresh_w;                //判断粘连字符的宽度阈值条件
    uint8_t m_nPart_percent;            //区域分割算子内参数，表示分割线偏移大小，默认值为 30
    uint8_t m_nChar_ero_w;              //腐蚀算子内参数，表示模板宽度，默认值为 2
    uint8_t m_nChar_ero_h;              //腐蚀算子内参数，表示模板高度，默认值为 3

    uint16_t m_nSlt_area_Min3;          //连通域选择过滤算子内参数，表示面积最小值
    uint16_t m_nSlt_area_Max3;          //连通域选择过滤算子内参数，表示面积最大值
    uint16_t m_nSlt_h_Min3;             //连通域选择过滤算子内参数，表示高度最小值
    uint16_t m_nSlt_h_Max3;             //连通域选择过滤算子内参数，表示高度最大值
    uint16_t m_nSlt_w_Min3;             //连通域选择过滤算子内参数，表示宽度最小值
    uint16_t m_nSlt_w_Max3;             //连通域选择过滤算子内参数，表示宽度最大值
//保留
	uint8_t  rsv6[160];			        //保留;
    uint32_t crc;				        //32位crc校验, crc常数$88320
};//256 bytes long

struct ExtIPCParaItem
{
    struct ExtCamPara ext_cam_para;							    //相机参数部分，128 bytes long;
    struct ExtProcessPara ext_process_para[4];		            //商标检测前端预处理参数组,256 bytes long;
    uint8_t rsv[21624];
};//98296 bytes long,8bytes alignment

struct ExtParaGroupGeneralInfo
{
    uint32_t len;												//参数组的总长度,不包括本结构体;
    uint32_t para_crc;											//参数组crc，不包括本结构体;
    uint64_t hardwareMAC;										//CIPCx硬件eth0 MAC地址;//暂不使用
    uint64_t softwareID;										//应用软件的id,应用软件会比较自身id和该softwareID;//暂不使用
    uint8_t  ext_prog_no;										//当前使用的瓶型序号;即程序号
    uint8_t  ext_style;											//0:瓶盖检测,其它未定;
    uint8_t  rsv[226];
    uint32_t crc;												//本一般性描述结构体crc;
};//256 bytes long

struct ExtParaGroupGeneralInfo filemiaoshu;

struct Discriptor2K_Label discri_front;		//当前附加数据结构
struct timeval time_serial_recv;			//串口接收时间戳
struct ExtCamPara cam_para_old;                 //相机参数old
struct ExtCamPara cam_para_new;                 //相机参数new

#endif // GLOBAL_PARAM_H_INCLUDED
