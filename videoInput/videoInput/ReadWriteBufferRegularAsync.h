#pragma once
#include "ireadwritebuffer.h"
class ReadWriteBufferRegularAsync :
	public IReadWriteBuffer
{
public:
	ReadWriteBufferRegularAsync(unsigned long imageSize);
	virtual ~ReadWriteBufferRegularAsync(void);

	virtual ResultCode::Result readData(unsigned char *data);
	virtual void copy(const unsigned char *source, unsigned char *destination, bool state);

};

