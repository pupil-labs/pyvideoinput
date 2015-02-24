#pragma once


#include "IReadWriteBuffer.h"
#include "videoInput.h"


class ReadWriteBufferFactory
{
public:

	static ReadWriteBufferFactory &getInstance();

	IReadWriteBuffer *createIReadWriteBuffer(ReadMode::Read readMode, unsigned int bufferSize);

private:
	ReadWriteBufferFactory(void);
	~ReadWriteBufferFactory(void);
	ReadWriteBufferFactory(const ReadWriteBufferFactory&);
	ReadWriteBufferFactory& operator=(const ReadWriteBufferFactory&);
};

