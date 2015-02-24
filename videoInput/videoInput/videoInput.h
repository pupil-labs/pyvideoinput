#pragma once

#include <string>
#include <vector>
#include <guiddef.h>

using namespace std;

// Structure of info 
struct MediaType
{
	unsigned int MF_MT_FRAME_SIZE;

	unsigned int height;

	unsigned int width;
			
	unsigned int MF_MT_YUV_MATRIX;
	
	unsigned int MF_MT_VIDEO_LIGHTING;
	
	unsigned int MF_MT_DEFAULT_STRIDE;
	
	unsigned int MF_MT_VIDEO_CHROMA_SITING;
	
	GUID MF_MT_AM_FORMAT_TYPE;
	
	wstring MF_MT_AM_FORMAT_TYPEName;
	
	unsigned int MF_MT_FIXED_SIZE_SAMPLES;
	
	unsigned int MF_MT_VIDEO_NOMINAL_RANGE;

	float MF_MT_FRAME_RATE_RANGE_MAX;

	float MF_MT_FRAME_RATE;

	float MF_MT_FRAME_RATE_RANGE_MIN;

	float MF_MT_PIXEL_ASPECT_RATIO;
	
	unsigned int MF_MT_ALL_SAMPLES_INDEPENDENT;
	
	unsigned int MF_MT_SAMPLE_SIZE;
	
	unsigned int MF_MT_VIDEO_PRIMARIES;
	
	unsigned int MF_MT_INTERLACE_MODE;

	GUID MF_MT_MAJOR_TYPE;
	
	wstring MF_MT_MAJOR_TYPEName;
	
	GUID MF_MT_SUBTYPE;
	
	wstring MF_MT_SUBTYPEName;	
};

// Stream structure
struct Stream
{
	std::vector<MediaType> listMediaType;
};

// Device info structure
struct Device
{
	wstring friendlyName;

	wstring symbolicName;

	std::vector<Stream> listStream;
};

// Structure for collecting info about one parametr of current video device
struct Parametr
{
	long CurrentValue;

	long Min;
	
	long Max;
	
	long Step;
	
	long Default; 
	
	long Flag;

	Parametr();
};

// Structure for collecting info about 17 parametrs of current video device
struct CamParametrs
{
	    Parametr Brightness;
        Parametr Contrast;
        Parametr Hue;
        Parametr Saturation;
        Parametr Sharpness;
        Parametr Gamma;
        Parametr ColorEnable;
        Parametr WhiteBalance;
        Parametr BacklightCompensation;
        Parametr Gain;


		Parametr Pan;
        Parametr Tilt;
        Parametr Roll;
        Parametr Zoom;
        Parametr Exposure;
        Parametr Iris;
        Parametr Focus;
};


// Structure for defining color format for out data
struct CaptureVideoFormat
{
	enum VideoFormat
	{
		RGB24 = 0,
		RGB32 = 1,
		AYUV = 2
	};
};

// structure of defining of the type callback event
struct StopCallbackEvent
{
	enum CallbackEvent
	{
		STOP = 0,
		CAPTUREDEVICEREMOVED = 1
	};
};

// Interface for processing callback of the emergency stop - removing video device.
class IStopCallback
{
public:
	virtual void Invoke(StopCallbackEvent::CallbackEvent callbackEvent) = 0;
};

// Structure for defining mode of reading pixels
struct ReadMode
{
	enum Read
	{
		ASYNC = 0,
		SYNC = 1
	};
};

// Settings for setupping of video device.
struct DeviceSettings
{
	wstring symbolicLink;

	unsigned int indexStream;

	unsigned int indexMediaType;
	
};

// Settings for setupping mode of capturing raw pixels
struct CaptureSettings
{
	CaptureVideoFormat::VideoFormat videoFormat;

	IStopCallback *pIStopCallback;

	ReadMode::Read readMode;
};

// Structure for controling reading raw pixels.
struct ReadSetting
{
	std::wstring symbolicLink;

	unsigned char *pPixels;

	void setNumpyArray(unsigned char* ndarray, int dim);
};

// Structurre for controling parameters of video device
struct CamParametrsSetting
{
	std::wstring symbolicLink;

	CamParametrs settings;
};

// Structure for defining result code of the working programm
struct ResultCode
{
	enum Result
	{
		OK = 0,
		UNKNOWN_ERROR = 1,
		MEDIA_FOUNDATION_INITIALIZECOM_ERROR = 2,
		MEDIA_FOUNDATION_INITIALIZEMF_ERROR = 3,
		MEDIA_FOUNDATION_SHUTDOWN_ERROR = 4,
		MEDIA_FOUNDATION_ENUMDEVICES_ERROR = 5,
		MEDIA_FOUNDATION_CREATEATTRIBUTE_ERROR = 6,
		MEDIA_FOUNDATION_READFRIENDLYNAME_ERROR = 7,
		MEDIA_FOUNDATION_READSYMBOLICLINK_ERROR = 8,
		MEDIA_FOUNDATION_GETDEVICE_ERROR = 9,
		MEDIA_FOUNDATION_createPresentationDescriptor_ERROR = 10,
		MEDIA_FOUNDATION_GETTHEAMOUNTOFSTREAMS_ERROR = 11,
		MEDIA_FOUNDATION_GETSTREAMDESCRIPTORBYINDEX_ERROR = 12,
		MEDIA_FOUNDATION_ENUMMEDIATYPE_ERROR = 13,
		VIDEOCAPTUREDEVICEMANAGER_GETLISTOFDEVICES_ERROR = 14,
		MEDIA_FOUNDATION_SETSYMBOLICLINK_ERROR = 15,
		MEDIA_FOUNDATION_SETCURRENTMEDIATYPE_ERROR = 16,
		MEDIA_FOUNDATION_GETCURRENTMEDIATYPE_ERROR = 17,
		MEDIA_FOUNDATION_SELECTSTREAM_ERROR = 18,
		MEDIA_FOUNDATION_CREATESESSION_ERROR = 19,
		MEDIA_FOUNDATION_CREATEMEDIATYPE_ERROR = 20,
		MEDIA_FOUNDATION_SETGUID_ERROR = 21,
		MEDIA_FOUNDATION_SETUINT32_ERROR = 22,
		MEDIA_FOUNDATION_CREATESAMPLERGRABBERSINKACTIVE_ERROR = 23,
		MEDIA_FOUNDATION_CREATETOPOLOGY_ERROR = 24,
		MEDIA_FOUNDATION_CREATETOPOLOGYNODE_ERROR = 25,
		MEDIA_FOUNDATION_SETUNKNOWN_ERROR = 26,
		MEDIA_FOUNDATION_SETOBJECT_ERROR = 27,
		MEDIA_FOUNDATION_ADDNODE_ERROR = 28,
		MEDIA_FOUNDATION_CONNECTOUTPUTNODE_ERROR = 29,
		MEDIA_FOUNDATION_SETTOPOLOGY_ERROR = 30,
		MEDIA_FOUNDATION_BEGINGETEVENT_ERROR = 31,
		VIDEOCAPTUREDEVICEMANAGER_DEVICEISSETUPED = 32,
		VIDEOCAPTUREDEVICEMANAGER_DEVICEISNOTSETUPED = 33,
		VIDEOCAPTUREDEVICEMANAGER_DEVICESTART_ERROR = 34,
		VIDEOCAPTUREDEVICE_DEVICESTART_ERROR = 35,
		VIDEOCAPTUREDEVICEMANAGER_DEVICEISNOTSTARTED = 36,
		VIDEOCAPTUREDEVICE_DEVICESTOP_ERROR = 37,
		VIDEOCAPTURESESSION_INIT_ERROR = 38,
		VIDEOCAPTUREDEVICE_DEVICESTOP_WAIT_TIMEOUT = 39,
		VIDEOCAPTUREDEVICE_DEVICESTART_WAIT_TIMEOUT = 40,
		READINGPIXELS_DONE = 41,
		READINGPIXELS_REJECTED = 42,
		READINGPIXELS_MEMORY_ISNOT_ALLOCATED = 43,
		READINGPIXELS_REJECTED_TIMEOUT = 44,
		VIDEOCAPTUREDEVICE_GETPARAMETRS_ERROR = 45,
		VIDEOCAPTUREDEVICE_SETPARAMETRS_ERROR = 46,
		VIDEOCAPTUREDEVICE_GETPARAMETRS_GETVIDEOPROCESSOR_ERROR = 47,
		VIDEOCAPTUREDEVICE_GETPARAMETRS_GETVIDEOCONTROL_ERROR = 48,
		VIDEOCAPTUREDEVICE_SETPARAMETRS_SETVIDEOCONTROL_ERROR = 49,
		VIDEOCAPTUREDEVICE_SETPARAMETRS_SETVIDEOPROCESSOR_ERROR = 50
	};

};

class videoInput
{
public:

	// get static instance of the singleton
	static videoInput& getInstance();

	// filling list of the video devices. 
	ResultCode::Result getListOfDevices(vector<Device> &listOfDevices);

	// setupping selected video device and capture settings.
	ResultCode::Result setupDevice(DeviceSettings deviceSettings, CaptureSettings captureSettings);

	// closing selected video device.
	ResultCode::Result closeDevice(DeviceSettings deviceSettings);

	// closing all setupped video devices
	ResultCode::Result closeAllDevices();

	// reading raw data of pixels
	ResultCode::Result readPixels(ReadSetting readSetting);

	// getting parametrs of video device
	ResultCode::Result getParametrs(CamParametrsSetting &parametrs);

	// setting parametrs of video device
	ResultCode::Result setParametrs(CamParametrsSetting parametrs);

	// Setting of the state of outprinting info on consol
	ResultCode::Result setVerbose(bool state);

private:
	videoInput(void);
	~videoInput(void);
	videoInput(const videoInput&);
	videoInput& operator=(const videoInput&);
};

