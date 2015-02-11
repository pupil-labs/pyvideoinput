#pragma once


#include <map>


#include "videoInput.h"

class VideoCaptureDevice;


class VideoCaptureDeviceManager
{
public:

	static VideoCaptureDeviceManager& getInstance();

	ResultCode::Result getListOfDevices(vector<Device> &listOfDevices);

	ResultCode::Result initAndAddDevice(DeviceSettings deviceSettings, CaptureSettings captureSettings);

	ResultCode::Result checkSetupedDevice(std::wstring symbolicLink);

	ResultCode::Result startDevice(std::wstring symbolicLink);

	ResultCode::Result closeDevice(std::wstring symbolicLink);

	ResultCode::Result closeAllDevices();

	ResultCode::Result readPixels(ReadSetting readSetting);

	ResultCode::Result getParametrs(CamParametrsSetting &parametrs);

	ResultCode::Result setParametrs(CamParametrsSetting parametrs);

private:
	VideoCaptureDeviceManager(void);
	~VideoCaptureDeviceManager(void);
	VideoCaptureDeviceManager(const VideoCaptureDeviceManager&);
	VideoCaptureDeviceManager& operator=(const VideoCaptureDeviceManager&);

	std::map<std::wstring, VideoCaptureDevice*> listSetupedDevices;

};

