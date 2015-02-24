#pragma once


#include <Unknwnbase.h>
#include <mutex>

#include "IRead.h"
#include "IWrite.h"


class IReadWriteBuffer: public IRead, public IWrite, public IUnknown
{
public:

	IReadWriteBuffer(unsigned long imageSize)
		: size(imageSize), readyToRead(true)
	{
		pixels.reset(new unsigned char[size]);

		memset((void *)pixels.get(),0,size);
	}

	virtual ~IReadWriteBuffer(void)
	{
	}


	virtual void writeData(const unsigned char *data)
	{
		copy(data, pixels.get(), true);
	}

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid,_COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject)
	{
		return S_OK;
	}
	
	virtual ULONG STDMETHODCALLTYPE AddRef(void)
	{
		return InterlockedIncrement(&refCount);
	}

	virtual ULONG STDMETHODCALLTYPE Release(void)
	{
		ULONG uCount = InterlockedDecrement(&refCount);
		if (uCount == 0)
		{
			delete this;
		}
		return uCount;
	}

	bool isReadyToRead()
	{
		return readyToRead;
	}

protected:
	
	virtual void copy(const unsigned char *source, unsigned char *destination, bool state)=0;
	
	std::mutex critSec;

	std::auto_ptr<unsigned char> pixels;

	unsigned long size;

	bool readyToRead;

private:
	
	volatile unsigned long refCount;  
	
};
