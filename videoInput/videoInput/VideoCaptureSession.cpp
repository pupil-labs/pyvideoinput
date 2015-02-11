#include "stdafx.h"


#include <mfobjects.h>
#include <mfidl.h>
#include <mfapi.h>


#include "VideoCaptureSession.h"


VideoCaptureSession::VideoCaptureSession(void)
	:refCount(0)
{
}


VideoCaptureSession::~VideoCaptureSession(void)
{
}

// Initialization of the Session
ResultCode::Result VideoCaptureSession::init(IMFMediaSession *pSession, IStopCallback *pIStopCallback)
{
	ResultCode::Result result = ResultCode::VIDEOCAPTURESESSION_INIT_ERROR;

	pLocalIStopCallback = pIStopCallback;

	closeCompleteEvent = CreateEvent(NULL,FALSE,FALSE,NULL);

	if(closeCompleteEvent == NULL)
		goto finish;

	startCompleteEvent = CreateEvent(NULL,FALSE,FALSE,NULL);

	if(startCompleteEvent == NULL)
		goto finish;

	pLocalSession = pSession;

	result = ResultCode::OK;

finish:

	return result;
}

HANDLE VideoCaptureSession::getStartCompleteEvent()
{
	return startCompleteEvent;
}

HANDLE VideoCaptureSession::getCloseCompleteEvent()
{
	return closeCompleteEvent;
}

STDMETHODIMP VideoCaptureSession::GetParameters(DWORD *pdwFlags, DWORD *pdwQueue)   
{ 
	return E_NOTIMPL; 
}  

HRESULT VideoCaptureSession::Invoke(IMFAsyncResult* pAsyncResult)
{	
	CComPtrCustom<IMFMediaEvent> pEvent;

    HRESULT hr = S_OK;

	std::lock_guard<std::mutex> lock(critSec);

	if(pAsyncResult == 0)
	{
		hr = E_UNEXPECTED;

		goto finish;
	}

	hr = pLocalSession->EndGetEvent(pAsyncResult, &pEvent);

	MediaEventCode::MediaEvent event = processMediaEvent(pEvent);
	
	if(event != MediaEventCode::CLOSED)
    {
		hr = pLocalSession->BeginGetEvent(this, NULL);           
    }


finish:

	return S_OK;
}

// Processing media events
MediaEventCode::MediaEvent VideoCaptureSession::processMediaEvent(CComPtrCustom<IMFMediaEvent>& pMediaEvent)
{

	MediaEventType eventType;

	HRESULT hr = S_OK;

	HRESULT hrStatus = S_OK; 

	MediaEventCode::MediaEvent result = MediaEventCode::OK;

	UINT32 TopoStatus = MF_TOPOSTATUS_INVALID;

	if(pMediaEvent == NULL)
		return MediaEventCode::NULLPTR;

	hr = pMediaEvent->GetType(&eventType);

	if (FAILED(hr))
	{
		result = MediaEventCode::EXECUTION_ERROR;

		goto finish;
	}
	
	hr = pMediaEvent->GetStatus(&hrStatus);

	if (FAILED(hr))
	{
		result = MediaEventCode::EXECUTION_ERROR;

		goto finish;
	}

	if (FAILED(hrStatus))
	{
		result = MediaEventCode::STATUS_ERROR;

		goto finish;
	}

	if(eventType == MESessionStarted)
	{		
		
		SetEvent(startCompleteEvent);

		goto finish;
	}

	if(eventType == MESessionTopologyStatus)
	{
		hr = pMediaEvent->GetUINT32(MF_EVENT_TOPOLOGY_STATUS, (UINT32*)&TopoStatus);

		if(TopoStatus == MF_TOPOSTATUS_READY)
		{

			PROPVARIANT var;

			PropVariantInit(&var);

			var.vt = VT_EMPTY;
			
			pLocalSession->Start(&GUID_NULL, &var);
		}

		goto finish;
	}

	if (eventType == MESessionStopped)
	{
		result = MediaEventCode::STOPPED;
	}

	if (eventType == MESessionClosed)
	{
		SetEvent(closeCompleteEvent);

		result = MediaEventCode::CLOSED;
	}


	if (eventType == MEVideoCaptureDeviceRemoved)
    {
		if(pLocalIStopCallback != 0)
		{
			pLocalIStopCallback->Invoke(StopCallbackEvent::CAPTUREDEVICEREMOVED);
		}

		result = MediaEventCode::CLOSED;
    }


	
finish:

	return result;
}

//
// IUnknown methods
//
HRESULT VideoCaptureSession::QueryInterface(REFIID riid, void** ppv)
{
    HRESULT hr = S_OK;

    if(ppv == NULL)
    {
        return E_POINTER;
    }

    if(riid == __uuidof(IMFAsyncCallback))
    {
        *ppv = static_cast<IMFAsyncCallback*>(this);
    }
    else if(riid == __uuidof(IUnknown))
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

ULONG VideoCaptureSession::AddRef(void)
{
    return InterlockedIncrement(&refCount);
}

ULONG VideoCaptureSession::Release(void)
{
    ULONG uCount = InterlockedDecrement(&refCount);
    if (uCount == 0)
    {
        delete this;
    }
    return uCount;
}
