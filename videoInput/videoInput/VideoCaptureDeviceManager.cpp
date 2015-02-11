#include "stdafx.h"


#include <mfobjects.h>
#include <mfidl.h>


#include "VideoCaptureDeviceManager.h"
#include "VideoCaptureDevice.h"
#include "MediaFoundation.h"
#include "FormatReader.h"
#include "ComMassivPtr.h"


VideoCaptureDeviceManager::VideoCaptureDeviceManager(void)
{
}

VideoCaptureDeviceManager::~VideoCaptureDeviceManager(void)
{
	closeAllDevices();
}

VideoCaptureDeviceManager& VideoCaptureDeviceManager::getInstance()
{
	static VideoCaptureDeviceManager instance;

	return instance;
}

// Get list of devices
ResultCode::Result VideoCaptureDeviceManager::getListOfDevices(vector<Device> &listOfDevices)
{
	listOfDevices.clear();
	
	using namespace std;
			
	CComMassivPtr<IMFActivate> ppDevices;

	ResultCode::Result result = MediaFoundation::getInstance().enumDevices(ppDevices.getPtrMassivPtr(), *ppDevices.setSizeMassivPtr());

	if(result != ResultCode::OK)
		goto finish;

	for(UINT32 sourceIndex = 0; sourceIndex < ppDevices.getSizeMassivPtr(); sourceIndex++)
	{
		Device device;

		CComPtrCustom<IMFMediaSource> pDevice;

		result = MediaFoundation::getInstance().readFriendlyName(ppDevices[sourceIndex], device.friendlyName);

		if(result != ResultCode::OK)
			continue;

		result = MediaFoundation::getInstance().readSymbolicLink(ppDevices[sourceIndex], device.symbolicName);

		if(result != ResultCode::OK)
			continue;
		
		result = MediaFoundation::getInstance().getSource(ppDevices[sourceIndex], &pDevice);

		
		if(result != ResultCode::OK)
			continue;
		
		CComPtrCustom<IMFPresentationDescriptor> pPD;

		result = MediaFoundation::getInstance().createPresentationDescriptor(pDevice, &pPD);

		if(result != ResultCode::OK)
			goto shutdownpDevice;

		DWORD streamCount = 0;

		result = MediaFoundation::getInstance().getAmountStreams(pPD, streamCount);

		if(result != ResultCode::OK)
			goto shutdownpDevice;

		for(DWORD streamIndex = 0; streamIndex < streamCount; streamIndex++)
		{	
			Stream stream;

			CComPtrCustom<IMFStreamDescriptor> pSD;

			BOOL isSelected;

			result = MediaFoundation::getInstance().getStreamDescriptorByIndex(0, pPD, isSelected, &pSD);

			if(result != ResultCode::OK)
				goto shutdownpDevice;

			result = MediaFoundation::getInstance().enumMediaType(pSD, stream.listMediaType);

			if(result != ResultCode::OK)
				goto shutdownpDevice;

			if(stream.listMediaType.empty())
				continue;

			device.listStream.push_back(stream);
		}
		
shutdownpDevice:		

		if(pDevice)
			pDevice->Shutdown();
						
		if(device.listStream.empty())
			continue;

		listOfDevices.push_back(device);
	}
					
finish:	
	
	return result;
}

// Initialization of device
ResultCode::Result VideoCaptureDeviceManager::initAndAddDevice(DeviceSettings deviceSettings, CaptureSettings captureSettings)
{
	ResultCode::Result result = ResultCode::UNKNOWN_ERROR;
		
	CComPtrCustom<IMFMediaSource> pSource;

	CComPtrCustom<IMFPresentationDescriptor> pPD;
	
	CComPtrCustom<IMFStreamDescriptor> pSD;

	CComPtrCustom<IMFMediaType> pMediaType;

	MediaType MT;

	BOOL isSelected;

	result = MediaFoundation::getInstance().getSorceBySymbolicLink(deviceSettings.symbolicLink, &pSource);

	if(result != ResultCode::OK)
		goto finish;
	
	result = MediaFoundation::getInstance().createPresentationDescriptor(pSource, &pPD);

	if(result != ResultCode::OK)
		goto finish;

	DWORD streamCount = 0;

	result = MediaFoundation::getInstance().getAmountStreams(pPD, streamCount);

	if(result != ResultCode::OK)
		goto finish;

	for(DWORD streamIndex = 0; streamIndex < streamCount; streamIndex++)
	{
		result = MediaFoundation::getInstance().deselectStream(streamIndex, pPD);

		if(result != ResultCode::OK)
			goto finish;
	}
		
	result = MediaFoundation::getInstance().selectStream(deviceSettings.indexStream, pPD);

	if(result != ResultCode::OK)
		goto finish;	

	result = MediaFoundation::getInstance().getStreamDescriptorByIndex(deviceSettings.indexStream, pPD, isSelected, &pSD);

	if(result != ResultCode::OK)
		goto finish;
	
	result = MediaFoundation::getInstance().setCurrentMediaType(pSD, deviceSettings.indexMediaType);

	if(result != ResultCode::OK)
		goto finish;
	
	result = MediaFoundation::getInstance().getCurrentMediaType(pSD, &pMediaType);

	if(result != ResultCode::OK)
		goto finish;

	MT = FormatReader::getInstance().Read(pMediaType);

	VideoCaptureDevice *pDevice = new VideoCaptureDevice;

	result = pDevice->init(pSource, deviceSettings, captureSettings, MT);

	if(result != ResultCode::OK)
	{
		delete pDevice;

		goto finish;
	}

	listSetupedDevices[deviceSettings.symbolicLink] = pDevice;

finish:

	return result;
}

// Check that device is setuped
ResultCode::Result VideoCaptureDeviceManager::checkSetupedDevice(std::wstring symbolicLink)
{
	ResultCode::Result result = ResultCode::VIDEOCAPTUREDEVICEMANAGER_DEVICEISNOTSETUPED;

	std::map<std::wstring, VideoCaptureDevice*>::iterator itr = listSetupedDevices.find(symbolicLink);

	if(itr != listSetupedDevices.end())
		return ResultCode::VIDEOCAPTUREDEVICEMANAGER_DEVICEISSETUPED;

	return result;
}

// Start device
ResultCode::Result VideoCaptureDeviceManager::startDevice(std::wstring symbolicLink)
{
	ResultCode::Result result = ResultCode::VIDEOCAPTUREDEVICEMANAGER_DEVICESTART_ERROR;

	std::map<std::wstring, VideoCaptureDevice*>::iterator itr = listSetupedDevices.find(symbolicLink);

	if(itr == listSetupedDevices.end())
		goto finish;

	result = (*itr).second->startDevice();
	

finish:

	return result;
}

// Stop device
ResultCode::Result VideoCaptureDeviceManager::closeDevice(std::wstring symbolicLink)
{	
	ResultCode::Result result = ResultCode::VIDEOCAPTUREDEVICEMANAGER_DEVICEISNOTSTARTED;

	std::map<std::wstring, VideoCaptureDevice*>::iterator itr = listSetupedDevices.find(symbolicLink);

	if(itr == listSetupedDevices.end())
		goto finish;

	result = (*itr).second->closeDevice();

	delete (*itr).second;

	listSetupedDevices.erase(itr);	

finish:

	return result;
}

// Stop all devices
ResultCode::Result VideoCaptureDeviceManager::closeAllDevices()
{	
	ResultCode::Result result = ResultCode::OK;

	std::map<std::wstring, VideoCaptureDevice*>::iterator itr = listSetupedDevices.begin();

	for(; itr != listSetupedDevices.end(); itr++)
	{		
		ResultCode::Result resultStop = (*itr).second->closeDevice();

		if(resultStop != ResultCode::OK)
			result = resultStop;

		delete (*itr).second;
	}

	listSetupedDevices.clear();
	
	return result;
}


// Get peremeters of videodevice
ResultCode::Result VideoCaptureDeviceManager::getParametrs(CamParametrsSetting &parametrs)
{
	ResultCode::Result result = ResultCode::VIDEOCAPTUREDEVICEMANAGER_DEVICEISNOTSETUPED;

	std::map<std::wstring, VideoCaptureDevice*>::iterator itr = listSetupedDevices.find(parametrs.symbolicLink);

	if(itr == listSetupedDevices.end())
		goto finish;

	result = (*itr).second->getParametrs(parametrs);

finish:

	return result;
}

ResultCode::Result VideoCaptureDeviceManager::setParametrs(CamParametrsSetting parametrs)
{	
	ResultCode::Result result = ResultCode::VIDEOCAPTUREDEVICEMANAGER_DEVICEISNOTSETUPED;

	std::map<std::wstring, VideoCaptureDevice*>::iterator itr = listSetupedDevices.find(parametrs.symbolicLink);

	if(itr == listSetupedDevices.end())
		goto finish;

	result = (*itr).second->setParametrs(parametrs);

finish:

	return result;
}

ResultCode::Result VideoCaptureDeviceManager::readPixels(ReadSetting readSetting)
{
	ResultCode::Result result = ResultCode::READINGPIXELS_MEMORY_ISNOT_ALLOCATED;

	std::map<std::wstring, VideoCaptureDevice*>::iterator itr = listSetupedDevices.find(readSetting.symbolicLink);

	if(itr == listSetupedDevices.end())
		goto finish;

	result = (*itr).second->readPixels(readSetting.pPixels);

finish:

	return result;
}