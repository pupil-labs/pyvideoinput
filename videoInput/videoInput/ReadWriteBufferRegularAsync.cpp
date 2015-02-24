#include "stdafx.h"
#include "ReadWriteBufferRegularAsync.h"


ReadWriteBufferRegularAsync::ReadWriteBufferRegularAsync(unsigned long imageSize)
	:IReadWriteBuffer(imageSize)
{
}

ReadWriteBufferRegularAsync::~ReadWriteBufferRegularAsync(void)
{
}

ResultCode::Result ReadWriteBufferRegularAsync::readData(unsigned char *data)
{
	ResultCode::Result result = ResultCode::READINGPIXELS_REJECTED;

	if(readyToRead)
	{
		copy(pixels.get(), data, false);

		result = ResultCode::READINGPIXELS_DONE;
	}

	return result;
}

void ReadWriteBufferRegularAsync::copy(const unsigned char *source, 
									   unsigned char *destination, 
									   bool state)
{
	std::lock_guard<std::mutex> lock(critSec);

	readyToRead = state;

	memcpy(destination, source, size);
}