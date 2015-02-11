#pragma once


#include <mfobjects.h>
#include <mutex>


#include "videoInput.h"
#include "ComPtrCustom.h"


struct IMFMediaSession;

struct MediaEventCode
{
	enum MediaEvent
	{
		NULLPTR = 0,
		CLOSED = 1,
		OK = 2,
		STATUS_ERROR = 3,
		EXECUTION_ERROR = 4,
		STOPPED = 5
	};
};

class VideoCaptureSession: public IMFAsyncCallback
{
public:
	VideoCaptureSession(void);
	virtual ~VideoCaptureSession(void);

	ResultCode::Result init(IMFMediaSession *pSession, IStopCallback *pIStopCallback);

	HANDLE getStartCompleteEvent();

	HANDLE getCloseCompleteEvent();

	STDMETHODIMP GetParameters(DWORD *pdwFlags, DWORD *pdwQueue);  

    STDMETHODIMP Invoke(IMFAsyncResult* pAsyncResult);

    STDMETHODIMP QueryInterface(REFIID iid, void** ppv);

	STDMETHODIMP_(ULONG) AddRef();
        
	STDMETHODIMP_(ULONG) Release();

private:

	MediaEventCode::MediaEvent processMediaEvent(CComPtrCustom<IMFMediaEvent>& pMediaEvent);


	volatile unsigned long refCount;  

	std::mutex critSec;

	CComPtrCustom<IMFMediaSession> pLocalSession;

	HANDLE closeCompleteEvent;

	HANDLE startCompleteEvent;

	IStopCallback *pLocalIStopCallback;
};

