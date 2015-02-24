#include "stdafx.h"
#include "MediaFoundation.h"
#include "MF.h"
#include "FormatReader.h"



MediaFoundation::MediaFoundation(void)
{
	state = initializeCOM();

	if (state != ResultCode::OK)
		return;

	state = initializeMF();	
}

MediaFoundation::~MediaFoundation(void)
{
	shutdown();
}

MediaFoundation& MediaFoundation::getInstance()
{
	static MediaFoundation instance;

	return instance;
}

// Initialize COM support
ResultCode::Result MediaFoundation::initializeCOM()
{
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_INITIALIZECOM_ERROR;

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	if (FAILED(hr))
	{
		DebugPrintOut::getInstance().printOut(L"COM: It cannot be initialized!!!\n");
	}
	else
	{
		DebugPrintOut::getInstance().printOut(L"COM: It is initialized!!!\n");

		result = ResultCode::OK;
	}

	return result;
}

// Initialize Media Foundation support
ResultCode::Result MediaFoundation::initializeMF()
{
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_INITIALIZEMF_ERROR;
	
	HRESULT hr = MFStartup(MF_VERSION);

	if (FAILED(hr))
	{
		DebugPrintOut::getInstance().printOut(L"MEDIA FOUNDATION: It cannot be initialized!!!\n");
	}
	else
	{
		DebugPrintOut::getInstance().printOut(L"MEDIA FOUNDATION: It is initialized!!!\n");

		result = ResultCode::OK;
	}

	return result;
}

// Release Media Foundation
ResultCode::Result MediaFoundation::shutdown()
{
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_SHUTDOWN_ERROR;

	HRESULT hr = MFShutdown();

	if (FAILED(hr))
	{
		DebugPrintOut::getInstance().printOut(L"MEDIA FOUNDATION: Resources cannot be released!!!\n");
	}
	else
	{
		DebugPrintOut::getInstance().printOut(L"MEDIA FOUNDATION: Resources are released!!!\n");

		result = ResultCode::OK;
	}
	
	return result;
}

// Get state of initialization
ResultCode::Result MediaFoundation::getInitializationState()
{
	return state;
}

// Enumerate devices
ResultCode::Result MediaFoundation::enumDevices(IMFActivate ***pppDevices, UINT32 &count)
{
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_ENUMDEVICES_ERROR;
	
	CComPtrCustom<IMFAttributes> pAttributes;
	
	HRESULT hr = MFCreateAttributes(&pAttributes, 1);

	if (FAILED(hr))
	{
		DebugPrintOut::getInstance().printOut(L"MEDIA FOUNDATION: Attribute cannot be created!!!\n");

		result = ResultCode::MEDIA_FOUNDATION_CREATEATTRIBUTE_ERROR;

		goto finish;
	}

	hr = pAttributes->SetGUID(
			MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
			MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID
			);

	if (FAILED(hr))
	{
		DebugPrintOut::getInstance().printOut(L"MEDIA FOUNDATION: GUID cannot be set!!!\n");

		result = ResultCode::MEDIA_FOUNDATION_CREATEATTRIBUTE_ERROR;

		goto finish;
	}

	hr = MFEnumDeviceSources(pAttributes, pppDevices, &count);
	
	if (FAILED(hr))
	{
		DebugPrintOut::getInstance().printOut(L"MEDIA FOUNDATION: Devices cannot be enumerated!!!\n");

		goto finish;
	}
	
	DebugPrintOut::getInstance().printOut(L"MEDIA FOUNDATION: Devices are enumerated!!!\n");
	
	result = ResultCode::OK;
		
finish:

	return result;
}

// Read friendlyName of device
ResultCode::Result MediaFoundation::readFriendlyName(IMFActivate *pDevice, std::wstring &friendlyName)
{
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_READFRIENDLYNAME_ERROR;

	wchar_t *pFriendlyName = NULL;

	HRESULT hr = S_OK;

	hr = pDevice->GetAllocatedString(
		MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME,
		&pFriendlyName,
		NULL
		);

	if (FAILED(hr))
	{
		DebugPrintOut::getInstance().printOut(L"MEDIA FOUNDATION: readFriendlyName cannot be executed!!!\n");

		goto finish;
	}
	
	friendlyName = std::wstring(pFriendlyName);

	result = ResultCode::OK;
		
finish:

	if (pFriendlyName)
		CoTaskMemFree(pFriendlyName);

	return result;
}

// Read friendlyName of device
ResultCode::Result MediaFoundation::readSymbolicLink(IMFActivate *pDevice, std::wstring &symbolicLink)
{
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_READSYMBOLICLINK_ERROR;

	wchar_t *pSymbolicLink = NULL;

	HRESULT hr = S_OK;

	hr = pDevice->GetAllocatedString(
			MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK,
			&pSymbolicLink,
			NULL
			);
	

	if (FAILED(hr))
	{
		DebugPrintOut::getInstance().printOut(L"MEDIA FOUNDATION: readSymbolicLink cannot be executed!!!\n");

		goto finish;
	}

	symbolicLink = std::wstring(pSymbolicLink);

	result = ResultCode::OK;

finish:

	if (pSymbolicLink)
		CoTaskMemFree(pSymbolicLink);

	return result;
}

// Get pointer of device
ResultCode::Result MediaFoundation::getSource(IMFActivate *pActivate, IMFMediaSource **ppDevice)
{
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_GETDEVICE_ERROR;

	HRESULT hr = S_OK;

	hr = pActivate->ActivateObject(
		__uuidof(IMFMediaSource),
		(void**)ppDevice
		);
	
	if (FAILED(hr))
		DebugPrintOut::getInstance().printOut(L"MEDIA FOUNDATION: getDevice cannot be executed!!!\n");
	else 
	{
		hr = pActivate->DetachObject();
				
	//	ULONG g = pActivate->Release();

	//	g = (*ppDevice)->AddRef();
						
		result = ResultCode::OK;
	}

	return result;
}

// Get pointer to the 
ResultCode::Result MediaFoundation::getSorceBySymbolicLink(wstring symbolicLink, IMFMediaSource **ppDevice)
{
	ResultCode::Result result = ResultCode::UNKNOWN_ERROR;

	CComPtrCustom<IMFAttributes> pAttributes;

	CComPtrCustom<IMFActivate> pActivate;

	HRESULT hr = MFCreateAttributes(&pAttributes, 1);

	hr = pAttributes->SetGUID(
		MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
		MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID
		);

	hr = pAttributes->SetString(
		MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK,
		symbolicLink.c_str()
		);

			
	if (FAILED(hr))
	{
		DebugPrintOut::getInstance().printOut(L"MEDIA FOUNDATION: symbolic link cannot be set!!!\n");

		result = ResultCode::MEDIA_FOUNDATION_SETSYMBOLICLINK_ERROR;

		goto finish;
	}

	hr = MFCreateDeviceSourceActivate(pAttributes, &pActivate);

	if (FAILED(hr))
	{
		DebugPrintOut::getInstance().printOut(L"MEDIA FOUNDATION: IMFActivate cannot be set!!!\n");

		result = ResultCode::MEDIA_FOUNDATION_SETSYMBOLICLINK_ERROR;

		goto finish;
	}

	result = getSource(pActivate, ppDevice);

finish:
	
	return result;
}

// Get PresentationDescriptor of device
ResultCode::Result MediaFoundation::createPresentationDescriptor(IMFMediaSource *pDevice, IMFPresentationDescriptor **ppPD)
{
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_createPresentationDescriptor_ERROR;

	HRESULT hr = pDevice->CreatePresentationDescriptor(ppPD);

	if (SUCCEEDED(hr))
	{
		result = ResultCode::OK;
	}
	else
	{
		DebugPrintOut::getInstance().printOut(L"MEDIA FOUNDATION: IMFPresentationDescriptor cannot be gotten. getAmountStreams cannot be executed!!!\n");
	}

	return result;
}

// Get the amount of streams in device
ResultCode::Result MediaFoundation::getAmountStreams(IMFPresentationDescriptor * pPD, DWORD &streamCount)
{
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_GETTHEAMOUNTOFSTREAMS_ERROR;

	HRESULT hr = S_OK;

	hr = pPD->GetStreamDescriptorCount(&streamCount);

	if (SUCCEEDED(hr))
	{
		result = ResultCode::OK;
	}
	else
		DebugPrintOut::getInstance().printOut(L"MEDIA FOUNDATION: getAmountStreams cannot be executed!!!\n");
		
	return result;
}

// Get the descriptor of stream by index
ResultCode::Result MediaFoundation::getStreamDescriptorByIndex(DWORD index, IMFPresentationDescriptor *pPD, BOOL &isSelected, IMFStreamDescriptor **pSD)
{
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_GETSTREAMDESCRIPTORBYINDEX_ERROR;
	
	HRESULT hr = pPD->GetStreamDescriptorByIndex(index, &isSelected, pSD);

	if (SUCCEEDED(hr))
	{
		result = ResultCode::OK;
	}
	else
		DebugPrintOut::getInstance().printOut(L"MEDIA FOUNDATION: getStreamDescriptorByIndex cannot be executed!!!\n");

	return result;
}

// Get the list of MediaType of the stream
ResultCode::Result MediaFoundation::enumMediaType(IMFStreamDescriptor *pSD, std::vector<MediaType> &listMediaType)
{
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_ENUMMEDIATYPE_ERROR;

	listMediaType.clear();

	CComPtrCustom<IMFMediaTypeHandler> pHandler;

	CComPtrCustom<IMFMediaType> pType;
	
	HRESULT hr = pSD->GetMediaTypeHandler(&pHandler);

	if (FAILED(hr))
	{
		DebugPrintOut::getInstance().printOut(L"MEDIA FOUNDATION: GetMediaTypeHandler cannot be executed!!!\n");

		goto finish;
	}
	
	DWORD cTypes = 0;

	hr = pHandler->GetMediaTypeCount(&cTypes);

	if (FAILED(hr))
	{
		DebugPrintOut::getInstance().printOut(L"MEDIA FOUNDATION: GetMediaTypeCount cannot be executed!!!\n");

		goto finish;
	}

	for (DWORD i = 0; i < cTypes; i++)
	{
		hr = pHandler->GetMediaTypeByIndex(i, &pType);

		if (FAILED(hr))
		{
			goto finish;
		}

		MediaType MT = FormatReader::getInstance().Read(pType);

		pType.Release();

		listMediaType.push_back(MT);
	}

	result = ResultCode::OK;

finish:

	return result;
}

// Set selection of Stream
ResultCode::Result MediaFoundation::selectStream(DWORD streamDescriptorIndex, IMFPresentationDescriptor *pPD)
{
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_SELECTSTREAM_ERROR;

	HRESULT hr = pPD->SelectStream(streamDescriptorIndex);

	if (FAILED(hr))
	{
		DebugPrintOut::getInstance().printOut(L"MEDIA FOUNDATION: SelectStream cannot be executed!!!\n");
	}
	else
		result = ResultCode::OK;

	return result;
}

// Release selection of Stream
ResultCode::Result MediaFoundation::deselectStream(DWORD streamDescriptorIndex, IMFPresentationDescriptor *pPD)
{
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_SELECTSTREAM_ERROR;

	HRESULT hr = pPD->DeselectStream(streamDescriptorIndex);

	if (FAILED(hr))
	{
		DebugPrintOut::getInstance().printOut(L"MEDIA FOUNDATION: DeselectStream cannot be executed!!!\n");
	}
	else
		result = ResultCode::OK;

	return result;
}

// Get the list of MediaType of the stream
ResultCode::Result MediaFoundation::setCurrentMediaType(IMFStreamDescriptor *pSD, unsigned long  dwFormatIndex)
{
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_SETCURRENTMEDIATYPE_ERROR;

	CComPtrCustom<IMFMediaTypeHandler> pHandler;

	CComPtrCustom<IMFMediaType> pType;

	HRESULT hr = pSD->GetMediaTypeHandler(&pHandler);

	if (FAILED(hr))
	{
		DebugPrintOut::getInstance().printOut(L"MEDIA FOUNDATION: GetMediaTypeHandler cannot be executed!!!\n");

		goto finish;
	}

	hr = pHandler->GetMediaTypeByIndex((DWORD)dwFormatIndex, &pType);

	if (FAILED(hr))
	{
		DebugPrintOut::getInstance().printOut(L"MEDIA FOUNDATION: GetMediaTypeByIndex cannot be executed!!!\n");

		goto finish;
	}

	hr = pHandler->SetCurrentMediaType(pType);

	if (SUCCEEDED(hr))
	{
		result = ResultCode::OK;
	}

finish:

	return result;
}

// Get the list of MediaType of the stream
ResultCode::Result MediaFoundation::getCurrentMediaType(IMFStreamDescriptor *pSD, IMFMediaType **ppMediaType)
{
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_GETCURRENTMEDIATYPE_ERROR;

	CComPtrCustom<IMFMediaTypeHandler> pHandler;

	CComPtrCustom<IMFMediaType> pType;

	HRESULT hr = pSD->GetMediaTypeHandler(&pHandler);

	if (FAILED(hr))
	{
		DebugPrintOut::getInstance().printOut(L"MEDIA FOUNDATION: GetMediaTypeHandler cannot be executed!!!\n");

		goto finish;
	}
	
	hr = pHandler->GetCurrentMediaType((ppMediaType));

	if (SUCCEEDED(hr))
	{
		result = ResultCode::OK;
	}

finish:

	return result;
}

// Get the list of MediaType of the stream
ResultCode::Result MediaFoundation::createSession(IMFMediaSession **ppSession)
{
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_CREATESESSION_ERROR;

	HRESULT hr = MFCreateMediaSession(NULL, ppSession);

	if(SUCCEEDED(hr))
		result = ResultCode::OK;

	return result;
}

// Create MediaType 
ResultCode::Result MediaFoundation::createMediaType(IMFMediaType **ppMediaType)
{
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_CREATEMEDIATYPE_ERROR;

	HRESULT hr = MFCreateMediaType(ppMediaType);

	if(SUCCEEDED(hr))
		result = ResultCode::OK;

	return result;
}

// Set GUID
ResultCode::Result MediaFoundation::setGUID(IMFAttributes *pAttributes, REFGUID guidKey, REFGUID guidValue)
{	
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_SETGUID_ERROR;

	HRESULT hr = pAttributes->SetGUID(guidKey, guidValue);

	if(SUCCEEDED(hr))
		result = ResultCode::OK;

	return result;
}

// Set UINT32 value
ResultCode::Result MediaFoundation::setUINT32(IMFAttributes *pAttributes, REFGUID guidKey, UINT32 value)
{
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_SETUINT32_ERROR;

	HRESULT hr = pAttributes->SetUINT32(guidKey, value);

	if(SUCCEEDED(hr))
		result = ResultCode::OK;

	return result;
}

// Create active sink for binding grabber
ResultCode::Result MediaFoundation::createSampleGrabberSinkActivate(IMFMediaType *pMediaType, 
																	IMFSampleGrabberSinkCallback *pSampleGrabberSinkCallback, 
																	IMFActivate **pActivate)
{
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_CREATESAMPLERGRABBERSINKACTIVE_ERROR;

	HRESULT hr = MFCreateSampleGrabberSinkActivate(pMediaType, pSampleGrabberSinkCallback, pActivate);

	if(SUCCEEDED(hr))
		result = ResultCode::OK;

	return result;
}

// Create pointer on topology
ResultCode::Result MediaFoundation::createTopology(IMFTopology **ppTopology)
{
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_CREATETOPOLOGY_ERROR;

	HRESULT hr = MFCreateTopology(ppTopology);

	if(SUCCEEDED(hr))
		result = ResultCode::OK;

	return result;
}

// Create node of topology
ResultCode::Result MediaFoundation::createTopologyNode(MF_TOPOLOGY_TYPE nodeType, IMFTopologyNode **ppNode)
{	
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_CREATETOPOLOGYNODE_ERROR;

	HRESULT hr = MFCreateTopologyNode(nodeType, ppNode);

	if(SUCCEEDED(hr))
		result = ResultCode::OK;

	return result;
}

// Set pointer by giudKey
ResultCode::Result MediaFoundation::setUnknown(IMFAttributes *pAttributes, REFGUID guidKey, IUnknown *pUnknown)
{
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_SETUNKNOWN_ERROR;

	HRESULT hr = pAttributes->SetUnknown(guidKey, pUnknown);

	if(SUCCEEDED(hr))
		result = ResultCode::OK;

	return result;
}

// Bind object with the node
ResultCode::Result MediaFoundation::setObject(IMFTopologyNode *pTopologyNode, IUnknown *pObject)
{
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_SETOBJECT_ERROR;

	HRESULT hr = pTopologyNode->SetObject(pObject);

	if(SUCCEEDED(hr))
		result = ResultCode::OK;

	return result;
}

// Add node to topology
ResultCode::Result MediaFoundation::addNode(IMFTopology *pTopology, IMFTopologyNode *pTopologyNode)
{
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_ADDNODE_ERROR;

	HRESULT hr = pTopology->AddNode(pTopologyNode);

	if(SUCCEEDED(hr))
		result = ResultCode::OK;

	return result;
}

// Connect two nodes
ResultCode::Result MediaFoundation::connectOutputNode(IMFTopologyNode *pTopologyNode, IMFTopologyNode *pOutputTopologyNode)
{
	
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_CONNECTOUTPUTNODE_ERROR;

	HRESULT hr = pTopologyNode->ConnectOutput(0, pOutputTopologyNode, 0);

	if(SUCCEEDED(hr))
		result = ResultCode::OK;

	return result;
}

// Set topology to the session
ResultCode::Result MediaFoundation::setTopology(IMFMediaSession *pSession, IMFTopology *pTopology)
{
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_SETTOPOLOGY_ERROR;

	HRESULT hr = pSession->SetTopology(0, pTopology);

	if(SUCCEEDED(hr))
		result = ResultCode::OK;

	return result;
}

// Set callback for processing events
ResultCode::Result MediaFoundation::beginGetEvent(IMFMediaEventGenerator *pMediaEventGenerator, IMFAsyncCallback *pCallback, IUnknown *punkState)
{
	ResultCode::Result result = ResultCode::MEDIA_FOUNDATION_BEGINGETEVENT_ERROR;

	HRESULT hr = pMediaEventGenerator->BeginGetEvent(pCallback, punkState);

	if(SUCCEEDED(hr))
		result = ResultCode::OK;

	return result;
}