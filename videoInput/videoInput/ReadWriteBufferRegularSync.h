#pragma once


#include "IReadWriteBuffer.h"


class ReadWriteBufferRegularSync :
	public IReadWriteBuffer
{
public:
	ReadWriteBufferRegularSync(unsigned long imageSize);
	virtual ~ReadWriteBufferRegularSync(void);
	
	virtual ResultCode::Result readData(unsigned char *data);
	virtual void copy(const unsigned char *source, unsigned char *destination, bool state);

protected:

	HANDLE syncEvent;

};

