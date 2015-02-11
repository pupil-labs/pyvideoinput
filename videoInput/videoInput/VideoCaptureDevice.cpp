#include "stdafx.h"



#include <mfobjects.h>
#include <mfidl.h>
#include <mfapi.h>


#include <Strmif.h>

#include "VideoCaptureDevice.h"
#include "MediaFoundation.h"
#include "ReadWriteBufferFactory.h"



VideoCaptureDevice::VideoCaptureDevice(void)
	:refCount(0)
{
}


VideoCaptureDevice::~VideoCaptureDevice(void)
{
}

ResultCode::Result VideoCaptureDevice::init(IMFMediaSource *pSource, DeviceSettings deviceSettings, 
											CaptureSettings captureSettings, MediaType MT)
{
	ResultCode::Result result = ResultCode::UNKNOWN_ERROR;

	GUID VideoFormat;

	CComPtrCustom<IMFMediaType> pMediaType;

	CComPtrCustom<IMFActivate> pSinkActivate;

	CComPtrCustom<IMFTopology> pTopology;
	
	unsigned int bpp = 0;

	switch (captureSettings.videoFormat)
	{
		case CaptureVideoFormat::RGB32:
			VideoFormat = MFVideoFormat_RGB32;
			bpp = 4;
			break;
		case CaptureVideoFormat::AYUV:
			VideoFormat = MFVideoFormat_AYUV;
			bpp = 4;
			break;
		case CaptureVideoFormat::RGB24:
		default:
			VideoFormat = MFVideoFormat_RGB24;
			bpp = 3;
			break;						
	}

	unsigned int imageSize = MT.MF_MT_FRAME_SIZE * bpp;

	pIReadWriteBuffer = ReadWriteBufferFactory::getInstance().createIReadWriteBuffer(captureSettings.readMode, 
																					 imageSize);
					
	result = MediaFoundation::getInstance().createMediaType(&pMediaType);

	if(result != ResultCode::OK)
	{
		goto finish;
	}

	result = MediaFoundation::getInstance().setGUID(pMediaType, MF_MT_MAJOR_TYPE, MFMediaType_Video);

	if(result != ResultCode::OK)
	{
		goto finish;
	}

	result = MediaFoundation::getInstance().setGUID(pMediaType, MF_MT_SUBTYPE, VideoFormat);

	if(result != ResultCode::OK)
	{
		goto finish;
	}


	pLocalSink = new VideoCaptureSink(pIReadWriteBuffer);
	

	result = MediaFoundation::getInstance().createSampleGrabberSinkActivate(pMediaType, pLocalSink, &pSinkActivate);

	if(result != ResultCode::OK)
	{
		goto finish;
	}
	
	result = MediaFoundation::getInstance().setUINT32(pSinkActivate, MF_SAMPLEGRABBERSINK_IGNORE_CLOCK, TRUE);

	if(result != ResultCode::OK)
	{
		goto finish;
	}

	result = MediaFoundation::getInstance().createTopology(&pTopology);

	if(result != ResultCode::OK)
	{
		goto finish;
	}

	result = createTopology(pSource, deviceSettings.indexStream, pSinkActivate, pTopology);

	if(result != ResultCode::OK)
	{
		goto finish;
	}
	

	result = MediaFoundation::getInstance().createSession(&pSession);

	if(result != ResultCode::OK)
	{
		goto finish;
	}

	pLocalSession = new VideoCaptureSession;

	result = pLocalSession->init(pSession, captureSettings.pIStopCallback);

	if(result != ResultCode::OK)
	{
		goto finish;
	}

	result = MediaFoundation::getInstance().beginGetEvent(pSession, pLocalSession, 0);

	if(result != ResultCode::OK)
	{
		goto finish;
	}
	

	result = MediaFoundation::getInstance().setTopology(pSession, pTopology);

	if(result != ResultCode::OK)
	{
		goto finish;
	}


	pLocalSource = pSource;


finish:

	return result;
}

// Start device
ResultCode::Result VideoCaptureDevice::startDevice()
{
	ResultCode::Result result = ResultCode::VIDEOCAPTUREDEVICE_DEVICESTART_ERROR;
	
    PROPVARIANT var;

    PropVariantInit(&var);

	HRESULT hr = pSession->Start(&GUID_NULL, &var);

	if(FAILED(hr))
		goto finish;
		
	DWORD dwWaitResult = 0;

	dwWaitResult = WaitForSingleObject(pLocalSession->getStartCompleteEvent(), 5000);

	if (dwWaitResult == WAIT_TIMEOUT)
	{		
		DebugPrintOut::getInstance().printOut(L"VIDEO CAPTURE DEVICE: Capturing cannot be started!!!\n");

		result = ResultCode::VIDEOCAPTUREDEVICE_DEVICESTART_WAIT_TIMEOUT;		
	}
	else
	{
		DebugPrintOut::getInstance().printOut(L"VIDEO CAPTURE DEVICE: Capturing is started!!!\n");

		result = ResultCode::OK;
	}

finish:

	return result;
}

// Stop device
ResultCode::Result VideoCaptureDevice::closeDevice()
{
	ResultCode::Result result = ResultCode::VIDEOCAPTUREDEVICE_DEVICESTOP_ERROR;
		
	DWORD dwWaitResult = 0;
	

	HRESULT hr = pSession->Close();

	if(SUCCEEDED(hr))
	{
		dwWaitResult = WaitForSingleObject(pLocalSession->getCloseCompleteEvent(), 5000);

		if (dwWaitResult == WAIT_TIMEOUT)
		{			
			
			DebugPrintOut::getInstance().printOut(L"VIDEO CAPTURE DEVICE: Video device cannot be closed!!!\n");

			result = ResultCode::VIDEOCAPTUREDEVICE_DEVICESTOP_WAIT_TIMEOUT;		
		}
		else
		{
			result = ResultCode::OK;
		}
	}

	pSession->Shutdown();
	
	pLocalSource->Stop();

	pLocalSource->Shutdown();

	pLocalSource.Release();
	
	return result;
}

ResultCode::Result VideoCaptureDevice::readPixels(unsigned char *pPixels)
{
	return pIReadWriteBuffer->readData(pPixels);
}

ResultCode::Result VideoCaptureDevice::getParametrs(CamParametrsSetting &parametrs)
{
	ResultCode::Result result = ResultCode::VIDEOCAPTUREDEVICE_GETPARAMETRS_ERROR;

	if(pLocalSource)
	{
		unsigned int shift = sizeof(Parametr);

		Parametr *pParametr = (Parametr *)(&parametrs.settings);

		CComPtrCustom<IAMVideoProcAmp> pProcAmp;

		HRESULT hr = pLocalSource->QueryInterface(IID_PPV_ARGS(&pProcAmp));

		if (SUCCEEDED(hr))
		{
			for(unsigned int i = 0; i < 10; i++)
			{
				Parametr temp;
					
				hr = pProcAmp->GetRange(VideoProcAmp_Brightness+i, &temp.Min, &temp.Max, &temp.Step, &temp.Default, &temp.Flag);
										
				if (SUCCEEDED(hr))
				{
					long currentValue = temp.Default;

					hr = pProcAmp->Get(VideoProcAmp_Brightness+i, &currentValue, &temp.Flag);

					if (SUCCEEDED(hr))
						temp.CurrentValue = currentValue;

					pParametr[i] = temp;
				}
			}						
		}
		else
		{
			result = ResultCode::VIDEOCAPTUREDEVICE_GETPARAMETRS_GETVIDEOPROCESSOR_ERROR;

			goto finish;
		}

		CComPtrCustom<IAMCameraControl> pProcControl;

		hr = pLocalSource->QueryInterface(IID_PPV_ARGS(&pProcControl));

		if (SUCCEEDED(hr))
		{
			for(unsigned int i = 0; i < 7; i++)
			{
				Parametr temp;
					
				hr = pProcControl->GetRange(CameraControl_Pan+i, &temp.Min, &temp.Max, &temp.Step, &temp.Default, &temp.Flag);
										
				if (SUCCEEDED(hr))
				{
					long currentValue = temp.Default;

					hr = pProcAmp->Get(CameraControl_Pan+i, &currentValue, &temp.Flag);

					temp.CurrentValue = currentValue;
						
					pParametr[10 + i] = temp;
				}
			}
		}
		else
		{
			result = ResultCode::VIDEOCAPTUREDEVICE_GETPARAMETRS_GETVIDEOCONTROL_ERROR;

			goto finish;
		}

		result = ResultCode::OK;
	}

finish:

	if(result != ResultCode::OK)
		DebugPrintOut::getInstance().printOut(L"VIDEO CAPTURE DEVICE: Parametrs of video device cannot be gotten!!!\n");

	return result;
	
}

ResultCode::Result VideoCaptureDevice::setParametrs(CamParametrsSetting parametrs)
{
	ResultCode::Result result = ResultCode::VIDEOCAPTUREDEVICE_SETPARAMETRS_ERROR;

	if(pLocalSource)
	{
		unsigned int shift = sizeof(Parametr);

		Parametr *pParametr = (Parametr *)(&parametrs.settings);

		Parametr *pPrevParametr = (Parametr *)(&prevParametrs);

		CComPtrCustom<IAMVideoProcAmp> pProcAmp;

		HRESULT hr = pLocalSource->QueryInterface(IID_PPV_ARGS(&pProcAmp));

		if (SUCCEEDED(hr))
		{
			for(unsigned int i = 0; i < 10; i++)
			{
				if(pPrevParametr[i].CurrentValue != pParametr[i].CurrentValue || pPrevParametr[i].Flag != pParametr[i].Flag)
					hr = pProcAmp->Set(VideoProcAmp_Brightness + i, pParametr[i].CurrentValue, pParametr[i].Flag);
					
			}
		}
		else
		{
			result = ResultCode::VIDEOCAPTUREDEVICE_SETPARAMETRS_SETVIDEOPROCESSOR_ERROR;

			goto finish;
		}

		CComPtrCustom<IAMCameraControl> pProcControl;

		hr = pLocalSource->QueryInterface(IID_PPV_ARGS(&pProcControl));

		if (SUCCEEDED(hr))
		{
			for(unsigned int i = 0; i < 7; i++)
			{
				if(pPrevParametr[10 + i].CurrentValue != pParametr[10 + i].CurrentValue || pPrevParametr[10 + i].Flag != pParametr[10 + i].Flag)
				hr = pProcControl->Set(CameraControl_Pan+i, pParametr[10 + i].CurrentValue, pParametr[10 + i].Flag);					
			}

		}
		else
		{
			result = ResultCode::VIDEOCAPTUREDEVICE_SETPARAMETRS_SETVIDEOCONTROL_ERROR;

			goto finish;
		}

		result = ResultCode::OK;

		prevParametrs = parametrs.settings;
	}

finish:

	if(result != ResultCode::OK)
		DebugPrintOut::getInstance().printOut(L"VIDEO CAPTURE DEVICE: Parametrs of video device cannot be set!!!\n");

	return result;
	
}

//
// IUnknown methods
//
HRESULT VideoCaptureDevice::QueryInterface(REFIID riid, void** ppv)
{
    HRESULT hr = S_OK;

    if(ppv == NULL)
    {
        return E_POINTER;
    }
	
	if(riid == __uuidof(IUnknown))
    {
        *ppv = static_cast<IUnknown*>(this);
    }
    else
    {
        *ppv = NULL;
        hr = E_NOINTERFACE;
    }

    if(SUCCEEDED(hr))
        AddRef();

    return hr;
}

ULONG VideoCaptureDevice::AddRef(void)
{
    return InterlockedIncrement(&refCount);
}

ULONG VideoCaptureDevice::Release(void)
{
    ULONG uCount = InterlockedDecrement(&refCount);
    if (uCount == 0)
    {
        delete this;
    }
    return uCount;
}

// Create topology
ResultCode::Result VideoCaptureDevice::createTopology(IMFMediaSource *pSource, DWORD streamIndex , IMFActivate *pSinkActivate, IMFTopology *pTopology)
{
	ResultCode::Result result = ResultCode::UNKNOWN_ERROR;
			
	CComPtrCustom<IMFPresentationDescriptor> pPD;
	
	CComPtrCustom<IMFStreamDescriptor> pSD;

	CComPtrCustom<IMFTopologyNode> pNode1;

	CComPtrCustom<IMFTopologyNode> pNode2;

	result = MediaFoundation::getInstance().createPresentationDescriptor(pSource, &pPD);

	if(result != ResultCode::OK)
		goto finish;

	BOOL isSelected;

	result = MediaFoundation::getInstance().getStreamDescriptorByIndex(streamIndex, pPD, isSelected, &pSD);

	if(result != ResultCode::OK)
		goto finish;
	
	result = addSourceNode(pTopology, pSource, pPD, pSD, &pNode1);
	
	if(result != ResultCode::OK)
		goto finish;
	
	result = addSinkNode(pTopology, pSinkActivate, 0, &pNode2);

	if(result != ResultCode::OK)
		goto finish;
	
	result = MediaFoundation::getInstance().connectOutputNode(pNode1, pNode2);
	
finish:
	
	if(result != ResultCode::OK)
		DebugPrintOut::getInstance().printOut(L"VIDEO CAPTURE DEVICE: Topology of video device cannot be created!!!\n");

	return result;
}

// Add node-Source into Topology
ResultCode::Result VideoCaptureDevice::addSourceNode(IMFTopology *pTopology, IMFMediaSource *pSource, IMFPresentationDescriptor *pPD, IMFStreamDescriptor *pSD, IMFTopologyNode **ppNode)
{
	ResultCode::Result result = ResultCode::UNKNOWN_ERROR;

	result = MediaFoundation::getInstance().createTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, ppNode);

	if(result != ResultCode::OK)
		goto finish;

	result = MediaFoundation::getInstance().setUnknown(*ppNode, MF_TOPONODE_SOURCE, pSource);

	if(result != ResultCode::OK)
		goto finish;

	result = MediaFoundation::getInstance().setUnknown(*ppNode, MF_TOPONODE_PRESENTATION_DESCRIPTOR, pPD);

	if(result != ResultCode::OK)
		goto finish;

	result = MediaFoundation::getInstance().setUnknown(*ppNode, MF_TOPONODE_STREAM_DESCRIPTOR, pSD);

	if(result != ResultCode::OK)
		goto finish;

	result = MediaFoundation::getInstance().addNode(pTopology, *ppNode);

finish:
	
	if(result != ResultCode::OK)
		DebugPrintOut::getInstance().printOut(L"VIDEO CAPTURE DEVICE: Source node of video device cannot be added!!!\n");

	return result;
}

// Adding Sink-node into topology
ResultCode::Result VideoCaptureDevice::addSinkNode(IMFTopology *pTopology, IMFActivate *pActivate, DWORD dwId, IMFTopologyNode **ppNode)
{
	ResultCode::Result result = ResultCode::UNKNOWN_ERROR;

	result = MediaFoundation::getInstance().createTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, ppNode);

	if(result != ResultCode::OK)
		goto finish;

	result = MediaFoundation::getInstance().setObject(*ppNode, pActivate);

	if(result != ResultCode::OK)
		goto finish;

	result = MediaFoundation::getInstance().setUINT32(*ppNode, MF_TOPONODE_STREAMID, dwId);

	if(result != ResultCode::OK)
		goto finish;

	result = MediaFoundation::getInstance().setUINT32(*ppNode, MF_TOPONODE_NOSHUTDOWN_ON_REMOVE, FALSE);

	if(result != ResultCode::OK)
		goto finish;

	result = MediaFoundation::getInstance().addNode(pTopology, *ppNode);

finish:
	
	if(result != ResultCode::OK)
		DebugPrintOut::getInstance().printOut(L"VIDEO CAPTURE DEVICE: Sink node of video device cannot be added!!!\n");

	return result;
}