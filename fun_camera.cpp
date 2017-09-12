#include "global_head.h"
#include "global_param.h"
#include "fun_public.h"
#include "fun_camera.h"


/******************************************/
char *intochar(int a)                  //ip
{
	int b,c;
	if(a < 10)
	{
		ipbuf[0] = a + '0';
		ipbuf[1] = NULL;
		ipbuf[2] = NULL;
	}
	else if(a < 100)
	{
		b = a/10;
		ipbuf[0] = b + '0';
		ipbuf[1] = (a - b*10) + '0';
		ipbuf[2] = NULL;
	}
	else //if(a<1000)
	{
		b = a/100;
		c = (a - b*100)/10;
		ipbuf[0] = b + '0';
		ipbuf[1] = c + '0';
		ipbuf[2] = (a - (b*100+c*10)) + '0';
	}
	//ipbuf[3] = NULL;					 //th不能随便乱改未定义的数组吧
	return ipbuf;
}

string IPAddressToString(IPAddress ipAddress)
{
	string Ipstring;
	Ipstring = "";
	Ipstring += intochar(ipAddress.octets[0]);
	Ipstring += ".";
	Ipstring += intochar(ipAddress.octets[1]);
	Ipstring += ".";
	Ipstring += intochar(ipAddress.octets[2]);
	Ipstring += ".";
	Ipstring += intochar(ipAddress.octets[3]);

	return Ipstring;
}

char *intoHex(int aa)              //mac
{
	if(aa/16 < 10)
		macbuf[0] = aa/16 + '0';
	else
		macbuf[0] = aa/16 - 10 + 'A';
	if(aa%16 < 10)
		macbuf[1] = aa%16 + '0';
	else
		macbuf[1] = aa%16 - 10 + 'A';
	//macbuf[2] = NULL;							//th不能随便乱改未定义的数组吧
	return macbuf;
}

string MACAddressToString(MACAddress mAddress)
{
	string Mstring;
	Mstring = "";
	Mstring += intoHex(mAddress.octets[0]);
	Mstring += ":";
	Mstring += intoHex(mAddress.octets[1]);
	Mstring += ":";
	Mstring += intoHex(mAddress.octets[2]);
	Mstring += ":";
	Mstring += intoHex(mAddress.octets[3]);
	Mstring += ":";
	Mstring += intoHex(mAddress.octets[4]);
	Mstring += ":";
	Mstring += intoHex(mAddress.octets[5]);
	return Mstring;
}
/*****************************************/

/*****************************************/
void BrightnessSet()
{
	Property prop;
	prop.type = BRIGHTNESS;
	prop.absControl = true;
	prop.absValue = para_new.cam_brightness;
	error = cam.SetProperty(&prop);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
	}
}

void ExposureSet()
{
	Property prop;
	prop.type = AUTO_EXPOSURE;
	prop.onOff = true;
	prop.autoManualMode = false;
	prop.absControl = true;
	prop.absValue = (para_new.cam_exposure)*0.001;//4,5
	error = cam.SetProperty(&prop);
	if(error != PGRERROR_OK)
	{
		PrintError(error);
	}
}

void SharpnessSet()
{
	Property prop;
	prop.type = SHARPNESS;
	prop.onOff = true;
	prop.autoManualMode = false;
	prop.onePush = false;
	prop.absControl = false;
	prop.valueA = para_new.cam_sharpness;//8,9
	error = cam.SetProperty(&prop);
	if(error != PGRERROR_OK)
	{
		PrintError(error);
	}
}

void HueSet()
{
	Property prop;
	prop.type = HUE;
	prop.onOff = true;
	prop.autoManualMode = false;
	prop.onePush = false;
	prop.absControl = false;
	prop.absValue = para_new.cam_hue;//6,7
	error = cam.SetProperty(&prop);
	if(error != PGRERROR_OK)
	{
		PrintError(error);
	}
}

void SaturationSet()
{
	Property prop;
	prop.type = SATURATION;
	prop.onOff = true;
	prop.autoManualMode = false;
	prop.absControl = true;
	prop.absValue = para_new.cam_saturation;//10,11
	error = cam.SetProperty(&prop);
	if(error != PGRERROR_OK)
	{
		PrintError(error);
	}
}

void GammaSet()
{
	Property prop;
	prop.type = GAMMA;
	prop.onOff = true;
	prop.autoManualMode = false;
	prop.absControl = true;
	prop.absValue = para_new.cam_gamma;
	error = cam.SetProperty(&prop);
	if(error != PGRERROR_OK)
	{
		PrintError(error);
	}
}

void ShutterSet()
{
	Property prop;
	prop.type = SHUTTER;
	prop.onOff = true;
	prop.autoManualMode = false;
	prop.absControl = true;
	prop.absValue = (para_new.cam_shutter)*0.001;
	error = cam.SetProperty(&prop);
	if(error != PGRERROR_OK)
	{
		PrintError(error);
	}
}

void GainSet()
{
	Property prop;
	prop.type = GAIN;
	prop.autoManualMode = false;
	prop.absControl = true;
	prop.absValue = (para_new.cam_gain)*0.001;//12,13
	error = cam.SetProperty(&prop);
	if(error != PGRERROR_OK)
	{
		PrintError(error);
	}
}

void WhiteBalanceSet()
{
	Property prop;
	prop.type = WHITE_BALANCE;
	if(((para_new.cam_wb_red) & 0x8000) == 0x8000)
		prop.onOff = true;
	else
		prop.onOff = false;
	if(((para_new.cam_wb_red) & 0x4000) == 0x4000)
		prop.autoManualMode = true;
	else
		prop.autoManualMode = false;
	prop.absControl = false;
	prop.valueA = para_new.cam_wb_red & 0x3ff;
	prop.valueB = para_new.cam_wb_blue & 0x3ff;
	error = cam.SetProperty(&prop);
	if(error != PGRERROR_OK)
	{
		PrintError(error);
	}
}
/************************************/


/************************************/
int PINSet()                     //th增加返回判断
{
	error = cam.WriteRegister(0x11F8, 0x40000000);
	if(error != PGRERROR_OK)
	{
		PrintError(error);
		//return -1;
	}
	return 0;
}

int SetGPIO0()                //th增加返回判断
{
	error = cam.WriteRegister(0x1110, 0x80000000);
	if(error != PGRERROR_OK)
	{
		PrintError(error);
		//return -1;
	}
	return 0;
}

int SetGPIO1()				//th增加返回判断
{
	unsigned int gvalue = 0;
	error = cam.ReadRegister(0x1120, &gvalue);
	if(error != PGRERROR_OK)
	{
		PrintError(error);
		//return -1;
	}
	gvalue |= 0x80080000;
	error = cam.WriteRegister(0x1120, gvalue);
	if(error != PGRERROR_OK)
	{
		PrintError(error);
		//return -1;
	}
	return 0;
}

int GetROISet()					//th增加返回判断
{
	error = cam.StopCapture();
	if(error != PGRERROR_OK)
	{
		PrintError(error);
		//return -1;            //th不知道是否需要增加返回？
	}

	GigEImageSettings imageSettings;
	imageSettings.offsetX = para_new.cam_roi_w_offset;//20,21
	imageSettings.offsetY = para_new.cam_roi_h_offset; //24,25
	imageSettings.height = para_new.cam_roi_h;//22,23
	imageSettings.width = para_new.cam_roi_w;//18,19
	imageSettings.pixelFormat = PIXEL_FORMAT_RAW12;

	error = cam.SetGigEImageSettings(&imageSettings);
	if(error != PGRERROR_OK)
	{
		PrintError(error);
		//return -1;
	}

	error = cam.StartCapture();
	if(error != PGRERROR_OK)
	{
		PrintError(error);
		//return -1;
	}
	return 0;
}

int TriggerModeSet()								//th增加返回判断
{
	TriggerMode triggerMode;
	triggerMode.onOff = true;
	triggerMode.mode = 0;
	triggerMode.parameter = 0;
	triggerMode.source = 0;
	error = cam.SetTriggerMode(&triggerMode);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		//return -1;
	}
	return 0;
}

int StrobeSet()										//th增加返回判断
{
	StrobeControl strobeset;
	strobeset.source = 1;
	strobeset.onOff = true;
	strobeset.delay = 0;
	strobeset.duration = 0;
	error = cam.SetStrobe(&strobeset);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		//return -1;
	}
	return 0;
}

int EmbInfomation()									//th增加返回判断
{
	EmbeddedImageInfo embeddedInfo;
	embeddedInfo.timestamp.onOff = false;
	embeddedInfo.brightness.onOff = false;
	embeddedInfo.exposure.onOff = false;
	embeddedInfo.frameCounter.onOff = false;
	embeddedInfo.gain.onOff = false;
	embeddedInfo.GPIOPinState.onOff = false;
	embeddedInfo.ROIPosition.onOff = false;
	embeddedInfo.shutter.onOff = false;
	embeddedInfo.strobePattern.onOff = false;
	embeddedInfo.whiteBalance.onOff = false;
	error = cam.SetEmbeddedImageInfo(&embeddedInfo);
	if(error != PGRERROR_OK)
	{
		PrintError(error);
		//return -1;
	}
	return 0;
}
/************************************/



int CheckPowerOnCamera(void)   //th增加返回判断
{
	// Power on the camera
	const unsigned int k_cameraPower = 0x610;
	const unsigned int k_powerVal = 0x80000000;
	unsigned int regVal = 0;
	unsigned int retries = 10;

	error = cam.WriteRegister(k_cameraPower, k_powerVal);
	if(error != PGRERROR_OK)
	{
		printf("power-up is not success\n");
		//return -1;
	}

	do
	{
		//delay
		error = cam.ReadRegister(k_cameraPower, &regVal);
		if(error == PGRERROR_TIMEOUT)
		{
			PrintError(error);
			//return -1;
		}
		else if(error != PGRERROR_OK)
		{
			PrintError(error);
			//return -1;
		}
		retries --;
	}while ((regVal & k_powerVal) == 0 && retries > 0);
	return 0;
}

void PrintCameraInfo(CameraInfo* pCamInfo)
{
    cout << endl;
	cout << "*** CAMERA INFORMATION ***" << endl;
	cout << "Camera model:" << pCamInfo->modelName << endl;
	cout << "IP Address:"<<IPAddressToString(pCamInfo->ipAddress)<<endl;
	cout<< "MAC Address:"<< MACAddressToString(pCamInfo->macAddress)<<endl;
}

int Mode_init()           //th增加返回判断
{
	PINSet();
	SetGPIO0();
	SetGPIO1();
	GetROISet();
	TriggerModeSet(); //in
	StrobeSet(); //out
	EmbInfomation(); //no timestamp,parameter info in image
	return 0;
}

int EnableBuffer()				   //th增加返回判断
{
	unsigned int bval;
	error = cam.ReadRegister(0x634, &bval);
	if(error != PGRERROR_OK)
	{
		PrintError(error);
	}

	bval |= 0x80000000;
	error = cam.WriteRegister(0x634, bval);
	if(error != PGRERROR_OK)
	{
		printf("Error writing enable buffer");
	}
	return 0;
}

int DisableIsochronous()			//th增加返回判断
{
	error = cam.WriteRegister(0x614, 0);
	if(error != PGRERROR_OK)
	{
		PrintError(error);
	}
	return 0;
}

bool PollForTriggerReady()
{
	const unsigned int k_softwareTrigger = 0x62C;
	unsigned int regVal = 0;
	do
	{
		error = cam.ReadRegister(k_softwareTrigger, &regVal);
		if(error != PGRERROR_OK)
		{
			PrintError(error);
			return false;
		}

	} while( (regVal >> 31) != 0 );

	return true;
}

int RunSingleCamera(PGRGuid guid)
{
	printf("Connecting to camera...\n");
	while(cam.IsConnected() == 0)
	{
		error = cam.Connect(&guid);
		if(error != PGRERROR_OK)
		{
			PrintError(error);
		}
	}
	//camera power
	if(CheckPowerOnCamera() < 0)                 //th增加返回判断
	{
		printf("CheckPowerOnCamera is error!\n");
		//return -1;
	}

	//camera information
	CameraInfo camInfo;
	error = cam.GetCameraInfo(&camInfo);
	if(error != PGRERROR_OK)
	{
		PrintError(error);
		//return -1;
	}
	PrintCameraInfo(&camInfo);
	//Restoretofactory();
	if(Mode_init() < 0)
	{
		printf("Mode_init is error!\n");
		//return -1;
	}


	// Check for external trigger support
	TriggerModeInfo triggerModeInfo;
	error = cam.GetTriggerModeInfo(&triggerModeInfo);
	if(error != PGRERROR_OK)
	{
		PrintError(error);
		//return -1;
	}
	if(triggerModeInfo.present != true)
	{
		printf("Camera does not support external trigger! Exiting...");
		return -1;
	}


	//enable buffer,enable transmit
	if(EnableBuffer() < 0)
	{
		printf("EnableBuffer is error!\n");
		//return -1;
	}

	if(DisableIsochronous() < 0)
	{
		printf("DisableIsochronous is error!\n");
		//return -1;
	}

	FC2Config fc2config;
	fc2config.grabMode = DROP_FRAMES;//GRAB_TIMEOUT_FORCE_32BITS
	//fc2config.grabTimeout = 500;
	error = cam.SetConfiguration(&fc2config);
	if(error != PGRERROR_OK)
	{
		PrintError(error);
		//return -1;
	}

	// savetochannel();
	// Poll to ensure camera is ready
	bool retVal = PollForTriggerReady();
	if(!retVal)
	{
		printf("\nError polling for trigger ready!\n");
		return -1;
	}
}

int camera_connect()
{
	BusManager busMgr;
    bool camera_link = false;
	while(camera_link == false)
	{
		CameraInfo camInfo[1];
		unsigned int numCamInfo = 0;
		error = BusManager::DiscoverGigECameras(camInfo, &numCamInfo);
		if (error != PGRERROR_OK)
		{
			PrintError(error);
		}
        cout << "Number of cameras discovered: " << numCamInfo << endl;
		cout << "Camera IP Address:"<<IPAddressToString(camInfo[0].ipAddress)<<endl;
		cout << "Camera MAC Address:"<< MACAddressToString(camInfo[0].macAddress)<<endl;

		if (numCamInfo < 1)
        {
            camera_link = true;
        }
	}

	int idx = 0;
    PGRGuid guid;
	error = busMgr.GetCameraFromIndex(idx, &guid);
    if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}
    InterfaceType interfaceType;
	error = busMgr.GetInterfaceTypeFromGuid(&guid, &interfaceType);
	if (error != PGRERROR_OK)
	{
		PrintError(error);
		return -1;
	}
    if (interfaceType == INTERFACE_GIGE)
    {
        if(RunSingleCamera(guid) < 0)
        {
            return -1;                      //th也加了判断
        }
    }
    else
    {
        printf("interfaceType is error\n");
        //return -1;                         th考虑加上,待测试
    }
	return 0;
}

/*********************************************/
void GetROISet0()
{
	GigEImageSettings imageSettings;
	imageSettings.offsetX = para_new.cam_roi_w_offset;
	imageSettings.offsetY = para_new.cam_roi_h_offset;

	error = cam.SetGigEImageSettings(&imageSettings);
	if(error != PGRERROR_OK)
	{
		PrintError(error);
		return;
	}
}

void ParaSet()
{
	memcpy(&para_new,(p_camera_buffer+1024),sizeof(para_new));
	memcpy(&para_old,(p_camera_buffer+1024),sizeof(para_old));
	BrightnessSet();
	ShutterSet();
	GammaSet();
	ExposureSet();
	HueSet();
	SharpnessSet();
	SaturationSet();
	GainSet();
	WhiteBalanceSet();
	GetROISet();
}

void ParaChange()
{
	memcpy(&para_new,p_camera_buffer,sizeof(para_new));

	if(para_new.cam_brightness != para_old.cam_brightness)
		BrightnessSet();

	if(para_new.cam_shutter != para_old.cam_shutter)
		ShutterSet();

	if(para_new.cam_gamma != para_old.cam_gamma)
		GammaSet();

	if(para_new.cam_exposure != para_old.cam_exposure)
		ExposureSet();

	if(para_new.cam_hue != para_old.cam_hue)
		HueSet();

	if(para_new.cam_sharpness != para_old.cam_sharpness)
		SharpnessSet();

	if(para_new.cam_saturation != para_old.cam_saturation)
		SaturationSet();

	if(para_new.cam_gain != para_old.cam_gain)
		GainSet();

	if(para_new.cam_wb_red != para_old.cam_wb_red)
		WhiteBalanceSet();

	if((para_new.cam_roi_h_offset) != (para_old.cam_roi_h_offset))
	{
		error = cam.StopCapture();
		if (error != PGRERROR_OK)
			PrintError(error);
		GetROISet0();
		error = cam.StartCapture();
		if (error != PGRERROR_OK)
			PrintError(error);
	}
}
/*****************************************/
