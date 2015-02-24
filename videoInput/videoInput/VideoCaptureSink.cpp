#include "stdafx.h"
#include "VideoCaptureSink.h"


#include <Shlwapi.h>

VideoCaptureSink::VideoCaptureSink(IWrite *pIWrite)
	:refCount(0), pLocalIWrite(pIWrite)
{
}

VideoCaptureSink::~VideoCaptureSink(void)
{
}

STDMETHODIMP VideoCaptureSink::QueryInterface(REFIID riid, void** ppv)
{
    static const QITAB qit[] = 
    {

        QITABENT(VideoCaptureSink, IMFSampleGrabberSinkCallback),
        QITABENT(VideoCaptureSink, IMFClockStateSink),
        { 0 }
    };
    return QISearch(this, qit, riid, ppv);
}

STDMETHODIMP_(ULONG) VideoCaptureSink::AddRef()
{
    return InterlockedIncrement(&refCount);
}

STDMETHODIMP_(ULONG) VideoCaptureSink::Release()
{
    ULONG cRef = InterlockedDecrement(&refCount);

    if (cRef == 0)
    {
        delete this;
    }

    return cRef;
}

STDMETHODIMP VideoCaptureSink::OnClockStart(MFTIME hnsSystemTime, LONGLONG llClockStartOffset)
{
    return S_OK;
}

STDMETHODIMP VideoCaptureSink::OnClockStop(MFTIME hnsSystemTime)
{
    return S_OK;
}

STDMETHODIMP VideoCaptureSink::OnClockPause(MFTIME hnsSystemTime)
{
    return S_OK;
}

STDMETHODIMP VideoCaptureSink::OnClockRestart(MFTIME hnsSystemTime)
{
    return S_OK;
}

STDMETHODIMP VideoCaptureSink::OnClockSetRate(MFTIME hnsSystemTime, float flRate)
{
    return S_OK;
}

STDMETHODIMP VideoCaptureSink::OnSetPresentationClock(IMFPresentationClock* pClock)
{
    return S_OK;
}

STDMETHODIMP VideoCaptureSink::OnProcessSample(REFGUID guidMajorMediaType, DWORD dwSampleFlags,
    LONGLONG llSampleTime, LONGLONG llSampleDuration, const BYTE * pSampleBuffer,
    DWORD dwSampleSize)
{
	pLocalIWrite->writeData(pSampleBuffer);
	
    return S_OK;
}

STDMETHODIMP VideoCaptureSink::OnShutdown()
{
    return S_OK;
}

