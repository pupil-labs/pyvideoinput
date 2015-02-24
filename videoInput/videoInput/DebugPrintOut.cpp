#include "stdafx.h"


#include "DebugPrintOut.h"


DebugPrintOut::DebugPrintOut(void):verbose(true)
{
}

DebugPrintOut::~DebugPrintOut(void)
{
}

DebugPrintOut& DebugPrintOut::getInstance()
{
	static DebugPrintOut instance;

	return instance;
}

void DebugPrintOut::printOut(const std::wstring format, ...)
{
	if(verbose)
	{
		int i = 0;

		wchar_t *p = NULL;

		va_list args;

		const wchar_t *p_format = format.c_str();

		va_start(args, p_format);

		bool state = true;

	
		if(wcscmp(p_format, L"%i"))
		{
				i = va_arg (args, int);
		}
 
		if(wcscmp(p_format, L"%s"))
		{
				p = va_arg (args, wchar_t *);
		}
		
		wprintf(p_format, i,p);

		va_end (args);
	}
}

void DebugPrintOut::printOut(const wchar_t *format, ...)
{
	if(verbose)
	{
		int i = 0;

		wchar_t *p = NULL;

		va_list args;

		va_start(args, format);

		bool state = true;

	
		if(wcscmp(format, L"%i"))
		{
				i = va_arg (args, int);
		}
 
		if(wcscmp(format, L"%s"))
		{
				p = va_arg (args, wchar_t *);
		}
		
		wprintf(format, i,p);

		va_end (args);
	}
}

void DebugPrintOut::setVerbose(bool state)
{
	verbose = state;
}