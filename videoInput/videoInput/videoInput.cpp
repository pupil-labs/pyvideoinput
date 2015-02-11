#include "stdafx.h"
#include "videoInput.h"
#include "MediaFoundation.h"
#include "VideoCaptureDeviceManager.h"

Parametr::Parametr()
{
	CurrentValue = 0;

	Min = 0;
	
	Max = 0;
	
	Step = 0;
	
	Default = 0; 
	
	Flag = 0;
}

videoInput::videoInput(void)
{
	DebugPrintOut::getInstance().printOut(L"\n***** VIDEOINPUT LIBRARY - 2014 (Author: Evgeny Pereguda) *****\n\n");

	ResultCode::Result result = MediaFoundation::getInstance().getInitializationState();

	if(result != ResultCode::OK)
		DebugPrintOut::getInstance().printOut(L"VIDEOINPUT: PROGRAM CANNOT BE USED!!! \n");
}

videoInput::~videoInput(void)
{	
	DebugPrintOut::getInstance().printOut(L"\n***** CLOSE VIDEOINPUT LIBRARY - 2014 *****\n\n");
}

// Get static instance of videoInput
videoInput& videoInput::getInstance()
{
	static videoInput instance;

	return instance;
}

// Get list of the devices which are pluggined to the system
ResultCode::Result videoInput::getListOfDevices(vector<Device> &listOfDevices)
{
	ResultCode::Result result = MediaFoundation::getInstance().getInitializationState();

	if(result != ResultCode::OK)
		goto finish;

	result = VideoCaptureDeviceManager::getInstance().getListOfDevices(listOfDevices);

	if(result != ResultCode::OK)
	{
		DebugPrintOut::getInstance().printOut(L"VIDEOINPUT: List of video devices cannot be enumerated!!!\n");
	}

finish:

	return result;
}

// Get list of the devices which are pluggined to the system
ResultCode::Result videoInput::setupDevice(DeviceSettings deviceSettings, CaptureSettings captureSettings)
{
	ResultCode::Result result = ResultCode::UNKNOWN_ERROR;
				
	result = MediaFoundation::getInstance().getInitializationState();

	if(result != ResultCode::OK)
		goto finish;

	result = VideoCaptureDeviceManager::getInstance().checkSetupedDevice(deviceSettings.symbolicLink);

	if(result != ResultCode::VIDEOCAPTUREDEVICEMANAGER_DEVICEISNOTSETUPED)
		goto finish;

	result = VideoCaptureDeviceManager::getInstance().initAndAddDevice(deviceSettings, captureSettings);

	if(result != ResultCode::OK)
		goto finish;

	result = VideoCaptureDeviceManager::getInstance().startDevice(deviceSettings.symbolicLink);
	
finish:

	if(result != ResultCode::OK)
	{
		DebugPrintOut::getInstance().printOut(L"VIDEOINPUT: Video device cannot be setupped!!!\n");
	}
	else
	{
		DebugPrintOut::getInstance().printOut(L"VIDEOINPUT: Video device is setupped\n");
	}

	return result;
}

// Stop the selected device
ResultCode::Result videoInput::closeDevice(DeviceSettings deviceSettings)
{
	ResultCode::Result result = VideoCaptureDeviceManager::getInstance().closeDevice(deviceSettings.symbolicLink);
	
	if(result != ResultCode::OK)
	{
		DebugPrintOut::getInstance().printOut(L"VIDEOINPUT: Video device is closed\n");
	}
	else
	{
		DebugPrintOut::getInstance().printOut(L"VIDEOINPUT: Video device cannot be closed!!!\n");
	}

	return result;
}

// Get peremeters of videodevice
ResultCode::Result videoInput::getParametrs(CamParametrsSetting &parametrs)
{
	ResultCode::Result result = VideoCaptureDeviceManager::getInstance().checkSetupedDevice(parametrs.symbolicLink);

	if(result != ResultCode::VIDEOCAPTUREDEVICEMANAGER_DEVICEISSETUPED)
		goto finish;


	result = VideoCaptureDeviceManager::getInstance().getParametrs(parametrs);

finish:
	
	if(result != ResultCode::OK)
	{
		DebugPrintOut::getInstance().printOut(L"VIDEOINPUT: Parametrs of the video device are gotten\n");
	}
	else
	{
		DebugPrintOut::getInstance().printOut(L"VIDEOINPUT: Parametrs of the video device cannot be gotten!!!\n");
	}

	return result;
}

// Set peremeters of videodevice
ResultCode::Result videoInput::setParametrs(CamParametrsSetting parametrs)
{	
	ResultCode::Result result = VideoCaptureDeviceManager::getInstance().checkSetupedDevice(parametrs.symbolicLink);

	if(result != ResultCode::VIDEOCAPTUREDEVICEMANAGER_DEVICEISSETUPED)
		goto finish;
	
	result = VideoCaptureDeviceManager::getInstance().setParametrs(parametrs);

finish:
	
	if(result != ResultCode::OK)
	{
		DebugPrintOut::getInstance().printOut(L"VIDEOINPUT: Parametrs of the video device are set\n");
	}
	else
	{
		DebugPrintOut::getInstance().printOut(L"VIDEOINPUT: Parametrs of the video device cannot be set!!!\n");
	}

	return result;
}

// Stop all devices
ResultCode::Result videoInput::closeAllDevices()
{
	ResultCode::Result result = VideoCaptureDeviceManager::getInstance().closeAllDevices();
	
	if(result != ResultCode::OK)
	{
		DebugPrintOut::getInstance().printOut(L"VIDEOINPUT: Video devices are closed\n");
	}
	else
	{
		DebugPrintOut::getInstance().printOut(L"VIDEOINPUT: Video devices cannot be closed!!!\n");
	}

	return result;
}
// Read pixels
ResultCode::Result videoInput::readPixels(ReadSetting readSetting)
{
	ResultCode::Result result = VideoCaptureDeviceManager::getInstance().checkSetupedDevice(readSetting.symbolicLink);

	if(result != ResultCode::VIDEOCAPTUREDEVICEMANAGER_DEVICEISSETUPED)
		goto finish;
	
	result = VideoCaptureDeviceManager::getInstance().readPixels(readSetting);

finish:

	return result;
}

// Setting of the state of outprinting info on consol
ResultCode::Result videoInput::setVerbose(bool state)
{
	DebugPrintOut::getInstance().setVerbose(state);	

	return ResultCode::OK;
}