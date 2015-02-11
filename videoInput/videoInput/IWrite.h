#pragma once


#include "videoInput.h"


class IWrite
{
public:

	IWrite(void)
	{
	}

	virtual ~IWrite(void)
	{
	}

	virtual void writeData(const unsigned char *data)=0;
};

