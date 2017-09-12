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

unsigned char *p_orignal[32];				//ԭʼͼ�񻺴�����iw12--ÿ����������СΪ��640*480*1.5=460800bytes+2k=462848bytes  0-2047--������Ϣ��ַ  2048-462847--ͼ�����ݵ�ַ
											//ĩ�����һλ��д��ǰ��������־ 0x10--clear  0x20--ready  0x30--sended
unsigned char *p_rgb[32];					//rgbͼ�񻺴�����ÿ����������СΪ:640*480*3=921600bytes+2k = 923648bytes  0-2047--������Ϣ��ַ   2048-923647--ͼ�����ݵ�ַ
											//ĩ�����һλ��д��ǰ��������־ 0x10--clear  0x20--ready  0x30--sended
unsigned char *p_gray[32];					//grayͼ�񻺴�����ÿ����������СΪ:640*480=307200bytes+2k=309248bytes  0-2047--������Ϣ��ַ  2048-309247--ͼ�����ݵ�ַ
											//ĩ�����һλ��д��ǰ��������־ 0x10--clear  0x20--ready  0x30--sended
unsigned char *p_result[32];				//���ͼ�񻺴�����ÿ����������СΪ:640*480=307200bytes+2k=309248bytes  0-2047--������Ϣ��ַ  2048-309247--ͼ�����ݵ�ַ
											//ĩ�����һλ��д��ǰ��������־ 0x10--clear  0x20--ready  0x30--sended
unsigned char *p_result_debug;			    //debugͼ�񻺴�����ÿ����������СΪ:640*480*20=6144000bytes+2k=6146048bytes  0-2047--������Ϣ��ַ  2048-6146047--ͼ�����ݵ�ַ
											//ĩ�����һλ��д��ǰ��������־ 0x10--clear  0x20--ready  0x30--sended

unsigned char *p_iw8;						//iw8 data buffer
unsigned char *p_rgb_0;						//iw12-iw8 mid buffer
unsigned char *p_data8;

unsigned char *p_camera_buffer;		     		//�������������----128B
unsigned char *p_image_buffer;			    	//�����������
unsigned char *p_refer_image;				//�ο�ͼ�񻺴���iw12--640*480*1.5=460800bytes + 2048 = 462848
unsigned char *p_quick_image;				//����ͼ������ iw12---640*480*1.5=460800bytes + 2048 = 462848

unsigned char *p_param_info;					//�洢��������ָ��

unsigned char *p_image0;					//iw12-iw8 table----1m

unsigned char *p_buff_tcp;									//tcp recv buff---8mbytes------8000
unsigned char *p_send;										//tcp send buff-1mbytes--------8000

unsigned char *p_echo;										//��������,current save the process miaoshu
unsigned char *p_online;									//current online data

uint8_t        g_refer_image;								//���µĲο�ͼ���ʶ,����һ��ȡ����λ
															//��⵽�˱�ʶ�仯������д��ǰ�ο�ͼ���ļ�����ǰ�˷��Ͳο�ͼ��
uint8_t		   g_refer_para;								//���µ�ǰ���Ĳ����ļ���ʶ,����һ��ȡ����λ
															//��⵽�˱�ʶ�仯������д��ǰ���Ĳ����ļ�����ǰ�˷��������Ԥ���������

uint8_t		   g_buf_num;				    //��ǰbuffer���  number=numer_ori & 0x1f
uint8_t		   g_buf_cur_num;				//��ǰ�������
uint8_t		   g_work_mode;					//����ģʽ  save the orignal image data from camera��refer image or quick image
uint8_t		   g_buf_num_serial;			//ÿ�����Ĵ�����Ϻ�˱�ʶ=g_buf_num�����ڲ��ּ�⵽�˱�ʶ�ı仯���͵�ǰ�ж����

bool		   g_flag_camera_init;			//�����ʼ����ʶ
bool		   g_flag_image_init;			//ͼ���������ʼ����ʶ

uint8_t		   g_current_count;				//��ǰ�����

struct Packhead
{
    uint16_t cmd;							//����
    int16_t  echo_flag;						//0,��ʾ��ǰ����/���³ɹ���<0����ʾ��ǰ����ʧ��
    uint32_t data_sum;						//�ô�PackData���ֵ����ݳ���
    uint32_t data_crc;						//PackData���ֵ�crc��crc����$88320(0x)
    uint16_t ext;							//cmd�ĸ�����Ϣ
    uint8_t  rsv;							//����,���׸�����Ϣ
    uint8_t  crc;							//��ͷ����crc��8λcrc��crc����$8c
};//16bytes

struct ExtCamPara
{
    uint8_t cam_brightness;					//��Χ0~25����Ӧ������õ�ȡֵ��ΧΪ0~25%��
    uint8_t cam_shutter;					//��Χ0~255����Ӧ���shutter0~255us��
    uint8_t cam_gamma;					    //��Χ0~4��ȱʡΪ0��
    uint8_t tran12_8_gamma;					//����raw12-->raw8�任��gamma����Χ80~200��
    int16_t cam_exposure;					//��Χ-7585~2414����Ӧ�����-7.585~2.414ev��
    int16_t cam_hue;						//��Χ-180~180����Ӧ���-180~180deg��ȱʡΪ0��
    uint16_t cam_sharpness;					//��Χ0~4095��Raw12��ʽʱĬ��Ϊ1024��
    uint16_t cam_saturation;				//��Χ0~400����Ӧ0~400%��ȱʡΪ100%��
    uint16_t cam_gain;						//��Χ-5753~13034����Ӧ-5.753~13.034db��
    uint16_t cam_wb_red;					//bit[9:0]��Χ0~1023��ȱʡ642��bit[15] 0 off, 1 on��bit[14] 1 auto��
    uint16_t cam_wb_blue;					//bit[9:0]��Χ0~1023��ȱʡ642��bit[15] 0 off, 1 on��bit[14] 1 auto��
    uint16_t cam_roi_w;						//��Χ0~1280��ȱʡ1280�Ҳ��ɸ��ģ�
    uint16_t cam_roi_w_offset;				//��Χ0~1280��ȱʡ0��
    uint16_t cam_roi_h ;					//��Χ0~960��ȱʡ480�Ҳ��ɸ��ģ�
    uint16_t cam_roi_h_offset;				//��Χ0~960��ȱʡ240��
    uint16_t tran12_8_l_limit;				//����raw12-->raw8�任���²���㣻
    uint16_t tran12_8_h_limit;				//����raw12-->raw8�任���ϲ��˵㣻
    uint16_t rsv1[47];						//������
    uint32_t crc32;							//32λcrcУ��, crc����$88320
};//128 bytes long

struct Discriptor2K_Label
{
/*raw12ͼ����������32bytes*/
    uint16_t raw12_w;						//ǰ��raw12�Ŀ�ȣ��ֽ׶ι̶�Ϊ1280;
    uint16_t raw12_h;						//ǰ��raw12�ĸ߶ȣ��ֽ׶ι̶�Ϊ480;
    uint16_t wb_red_now;					//��ǰ���ʹ�õİ�ƽ��redϵ��;
    uint16_t wb_blue_now;				    //��ǰ���ʹ�õİ�ƽ��blueϵ��;
    uint8_t  raw12_mode;					//raw12������Դ������ģʽ�ȵĶ���;
											//bit[7:4]��
											//4'b0000��ʾ��׼����ģʽ��ͼ��Դ��������������ź������ⲿ��������
											//4'b0???������
											//4'b1000��ʾ�ڲ�����ģʽ0�����ⴥ����ͼ��Դ���������
											//4'b1001��ʾ�ڲ�����ģʽ1�����ⴥ����ͼ��Դ���Ա���ı�׼ͼ��(����ͼ)��
											//4'b1010��ʾ�ڲ�����ģʽ2�����ⴥ����ͼ��Դ���Լ�����(����ͼ)��
											//4'b1011--4'b1111������
											//byte0[3]����;
											//byte0[2:0]��������0--7��
    uint8_t  raw12_sn;						//���0--255������ǰ�˹��ػ�buf���ʹ�����еĵ�5bit;
    uint8_t  raw12_cam_no;					//��raw12ͼ��Դ�Լ������,�̱���1~4�����
    uint8_t  rsv0[21];
/*rgbȫ�ߴ�ͼ����������32bytes*/
    uint16_t rgb_all_w;						//ǰ��rgbȫ�ߴ�ͼ��Ŀ�ȣ��ֽ׶ι̶�Ϊ480;
    uint16_t rgb_all_h;						//ǰ��rgbȫ�ߴ�ͼ��ĸ߶ȣ��ֽ׶ι̶�Ϊ1280;
    uint8_t  rsv1[28];
/* ��ͼ��ʹ�õ��������128bytes*/
    uint8_t  camera_buffer[128];
/*��ͼ��ʹ�õĴ������256bytes*/
    uint8_t  process_buffer[256];
/*��ͼ������ 256 bytes*/
    uint8_t  result_buffer[256];

/*����ʱ���� 64bytes*/
    uint32_t time_ipc1_x;					//ǰ�˺�ʱ(���յ����ڱ�־��������ͼ������),��λus;---768
    uint32_t time_get_raw12;				//raw12���ݽ��պ�ʱ(���յ����ڱ�־����ȡ��raw12����),��λus;;---772
    uint32_t time_raw12_to_rgb;				// raw12->rgb��ʱ,��λus;----776
    uint8_t  rsv2[32];
/*����*/
    uint8_t  rsv3[1296];					//����;
    uint32_t crc;
};//2048bytes long

struct ExtProcessPara
{
	//m_nFlagDebug��bit���壺
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

// Model0: ������������
	uint8_t  m_nCrop_sw;                //������״����ѡ�񿪹أ�1(0001): Rect  2(0010): Ellip  4(0100): Cir  8(1000): Debug
    uint16_t m_nParam1;                 // m_nParam1/2/3/4/5Ϊ���ɾ��ε��������ӵĲ���
    uint16_t m_nParam2;
    uint16_t m_nParam3;
    uint16_t m_nParam4;
    uint16_t m_nParam5;

	uint8_t m_nFlagAlgorithm;           //�㷨ѡ���־λ��|m_bSp_sw|m_bFe_sw|m_bSlant_sw|m_bHisto_sw|
                                        //m_bHisto_sw ����ƽ������ʹ�ÿ���
                                        //m_bSlant_sw : �ַ���б�ǶȽ���
                                        //m_bFe_sw �� ������ȡ����ѡ�񿪹�
                                        //m_bSp_sw �� ȥ������������ѡ�񿪹�

    uint16_t m_nROI_w;                  //�ַ��ı�����Ŀ��
    uint16_t m_nROI_h;                  //�ַ��ı�����ĸ߶�
    uint8_t m_nCol_nums;                //�ַ�����
    int8_t m_nOrt_min;                  //�ı���б�Ƕ���Сֵ
    int8_t m_nOrt_max;                  //�ı���б�Ƕ����ֵ
    uint8_t m_nChar_h;                  //�ַ��߶�
//Model1: Ѱ�������ı�����
	uint8_t m_nMean_sp1;                //ȥ�������������ڲ�������ʾģ���С
    uint8_t m_nMean_img1;               //��ֵ�˲������ڲ�������ʾģ���С
    uint8_t m_nMin_max_gry1;            //��ȡ�Ҷ�ͼ�����С�Ҷ�ֵ�����ڲ�������ʾ�Ҷȷ�Χ����
    uint8_t m_nDyn1;                    //��̬��ֵ��ֵ�������ڲ�������ʾģ���С
    uint8_t m_nDil_h1;                  //���������ڲ�������ʾģ��߶�
    uint8_t m_nDil_w1;                  //���������ڲ�������ʾģ����

    uint16_t m_nSlt_area_Min1;          //��ͨ��ѡ����������ڲ�������ʾ�����Сֵ
    uint16_t m_nSlt_area_Max1;          //��ͨ��ѡ����������ڲ�������ʾ������ֵ
    uint16_t m_nSlt_h_Min1;             //��ͨ��ѡ����������ڲ�������ʾ�߶���Сֵ
    uint16_t m_nSlt_h_Max1;             //��ͨ��ѡ����������ڲ�������ʾ�߶����ֵ
    uint16_t m_nSlt_w_Min1;             //��ͨ��ѡ����������ڲ�������ʾ�����Сֵ
    uint16_t m_nSlt_w_Max1;             //��ͨ��ѡ����������ڲ�������ʾ������ֵ
// Model2���������ַ��ֳɶ�������ַ�����
	uint8_t m_nMean_sp2;                //ȥ�������������ڲ�������ʾģ���С
    uint8_t m_nMin_max_gry2;            //��ȡ�Ҷ�ͼ�����С�Ҷ�ֵ�����ڲ�������ʾ�Ҷȷ�Χ����
    uint8_t m_nMean_img2;               //��ֵ�˲������ڲ�������ʾģ���С
    uint8_t m_nDot2;                    //��̬��ֵ��ֵ�����ӻ�Բ����ǿ�����ڲ�������ʾģ���С
    uint8_t m_nThresh_min;              //Բ����ǿ�����ڲ�������ʾ�Ҷ�ֵ��Χ��Сֵ
    uint8_t m_nThresh_max;              //Բ����ǿ�����ڲ�������ʾ�Ҷ�ֵ��Χ���ֵ

    uint8_t m_nDil_h2;                  //���������ڲ�������ʾģ��߶�
    uint8_t m_nDil_w2;                  //���������ڲ�������ʾģ����
    uint8_t m_nDil_shift;               //���������ڲ�������ʾģ��߶�����
    uint8_t m_nEro_w2;                  //��ʴ�����ڲ�������ʾģ����
    uint8_t m_nEro_h2;                  //��ʴ�����ڲ�������ʾģ��߶�

    uint16_t m_nSlt_area_Min2;          //��ͨ��ѡ����������ڲ�������ʾ�����Сֵ
    uint16_t m_nSlt_area_Max2;          //��ͨ��ѡ����������ڲ�������ʾ������ֵ
    uint16_t m_nSlt_w_Min2;             //��ͨ��ѡ����������ڲ�������ʾ�����Сֵ
    uint16_t m_nSlt_w_Max2;             //��ͨ��ѡ����������ڲ�������ʾ������ֵ
// Model3���������ַ��Լ��ַ�ʶ��
	uint8_t m_nDil_w3_1;                //��Ե�һ���ַ������������ڲ�������ʾģ����
    uint8_t m_nDil_w3_2;                //��Եڶ����ַ������������ڲ�������ʾģ����
    uint8_t m_nDil_w3_3;                //��Ե������ַ������������ڲ�������ʾģ����
    uint8_t m_nDil_w3_4;                //��Ե������ַ������������ڲ�������ʾģ����
    uint8_t m_nDil_h3_1;                //��Ե�һ���ַ������������ڲ�������ʾģ��߶�
    uint8_t m_nDil_h3_2;                //��Եڶ����ַ������������ڲ�������ʾģ��߶�
    uint8_t m_nDil_h3_3;                //��Ե������ַ������������ڲ�������ʾģ��߶�
    uint8_t m_nDil_h3_4;                //��Ե������ַ������������ڲ�������ʾģ��߶�

    uint8_t m_nThresh_w;                //�ж�ճ���ַ��Ŀ����ֵ����
    uint8_t m_nPart_percent;            //����ָ������ڲ�������ʾ�ָ���ƫ�ƴ�С��Ĭ��ֵΪ 30
    uint8_t m_nChar_ero_w;              //��ʴ�����ڲ�������ʾģ���ȣ�Ĭ��ֵΪ 2
    uint8_t m_nChar_ero_h;              //��ʴ�����ڲ�������ʾģ��߶ȣ�Ĭ��ֵΪ 3

    uint16_t m_nSlt_area_Min3;          //��ͨ��ѡ����������ڲ�������ʾ�����Сֵ
    uint16_t m_nSlt_area_Max3;          //��ͨ��ѡ����������ڲ�������ʾ������ֵ
    uint16_t m_nSlt_h_Min3;             //��ͨ��ѡ����������ڲ�������ʾ�߶���Сֵ
    uint16_t m_nSlt_h_Max3;             //��ͨ��ѡ����������ڲ�������ʾ�߶����ֵ
    uint16_t m_nSlt_w_Min3;             //��ͨ��ѡ����������ڲ�������ʾ�����Сֵ
    uint16_t m_nSlt_w_Max3;             //��ͨ��ѡ����������ڲ�������ʾ������ֵ
//����
	uint8_t  rsv6[160];			        //����;
    uint32_t crc;				        //32λcrcУ��, crc����$88320
};//256 bytes long

struct ExtIPCParaItem
{
    struct ExtCamPara ext_cam_para;							    //����������֣�128 bytes long;
    struct ExtProcessPara ext_process_para[4];		            //�̱���ǰ��Ԥ���������,256 bytes long;
    uint8_t rsv[21624];
};//98296 bytes long,8bytes alignment

struct ExtParaGroupGeneralInfo
{
    uint32_t len;												//��������ܳ���,���������ṹ��;
    uint32_t para_crc;											//������crc�����������ṹ��;
    uint64_t hardwareMAC;										//CIPCxӲ��eth0 MAC��ַ;//�ݲ�ʹ��
    uint64_t softwareID;										//Ӧ�������id,Ӧ�������Ƚ�����id�͸�softwareID;//�ݲ�ʹ��
    uint8_t  ext_prog_no;										//��ǰʹ�õ�ƿ�����;�������
    uint8_t  ext_style;											//0:ƿ�Ǽ��,����δ��;
    uint8_t  rsv[226];
    uint32_t crc;												//��һ���������ṹ��crc;
};//256 bytes long

struct ExtParaGroupGeneralInfo filemiaoshu;

struct Discriptor2K_Label discri_front;		//��ǰ�������ݽṹ
struct timeval time_serial_recv;			//���ڽ���ʱ���
struct ExtCamPara cam_para_old;                 //�������old
struct ExtCamPara cam_para_new;                 //�������new

#endif // GLOBAL_PARAM_H_INCLUDED
