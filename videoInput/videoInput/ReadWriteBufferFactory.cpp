#include "stdafx.h"
#include "ReadWriteBufferFactory.h"
#include "ReadWriteBufferRegularSync.h"
#include "ReadWriteBufferRegularAsync.h"


ReadWriteBufferFactory::ReadWriteBufferFactory(void)
{
}

ReadWriteBufferFactory::~ReadWriteBufferFactory(void)
{
}

ReadWriteBufferFactory &ReadWriteBufferFactory::getInstance()
{
	static ReadWriteBufferFactory instance;

	return instance;
}

IReadWriteBuffer *ReadWriteBufferFactory::createIReadWriteBuffer(ReadMode::Read readMode, 
																 unsigned int bufferSize)
{
	IReadWriteBuffer *result = 0;

	switch (readMode)
	{
		
	case ReadMode::ASYNC:
		result = new ReadWriteBufferRegularAsync(bufferSize);
		break;
	case ReadMode::SYNC:
		result = new ReadWriteBufferRegularSync(bufferSize);
	default:
		break;
	}


	return result;
}