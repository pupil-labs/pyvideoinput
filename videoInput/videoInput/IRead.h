#pragma once



#include "videoInput.h"


class IRead
{
public:
	virtual ResultCode::Result readData(unsigned char *data)=0;

	IRead(){}
	virtual ~IRead(){}
};