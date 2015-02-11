#pragma once

#include <string>

/// Class for printing info into consol
class DebugPrintOut
{
public:
	
	static DebugPrintOut& getInstance();

	void printOut(const std::wstring format, ...);

	void printOut(const wchar_t *format, ...);

	void setVerbose(bool state);
	
	bool verbose;

private:	
	DebugPrintOut(void);	

	~DebugPrintOut(void);

	DebugPrintOut(const DebugPrintOut&);	
		
	DebugPrintOut& operator=(const DebugPrintOut&);
};

