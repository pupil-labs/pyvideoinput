#include "stdafx.h"
#include "ReadWriteBufferRegularSync.h"


ReadWriteBufferRegularSync::ReadWriteBufferRegularSync(unsigned long imageSize)
	:IReadWriteBuffer(imageSize)
{
	syncEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
}

ReadWriteBufferRegularSync::~ReadWriteBufferRegularSync(void)
{
}

ResultCode::Result ReadWriteBufferRegularSync::readData(unsigned char *data)
{
	ResultCode::Result result = ResultCode::READINGPIXELS_REJECTED;

	DWORD dwWaitResult = 0;
		
	dwWaitResult = WaitForSingleObject(syncEvent, 1000);

	if (dwWaitResult == WAIT_TIMEOUT)
	{
		result = ResultCode::READINGPIXELS_REJECTED_TIMEOUT;	
	}
	else
	{
		copy(pixels.get(), data, false);

		result = ResultCode::READINGPIXELS_DONE;
	}

	return result;
}

void ReadWriteBufferRegularSync::copy(const unsigned char *source, 
									   unsigned char *destination, 
									   bool state)
{
	std::lock_guard<std::mutex> lock(critSec);

	if(state)
	{
		SetEvent(syncEvent);
	}
	else
	{
		ResetEvent(syncEvent);
	}

	memcpy(destination, source, size);
}
