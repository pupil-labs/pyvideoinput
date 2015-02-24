#pragma once

#include <Strsafe.h>
#include <combaseapi.h>
#include <string>


#include "videoInput.h"


struct IMFMediaType;
struct IMFAttributes;

/// Class for parsing info from IMFMediaType into the local MediaType
class FormatReader
{
public:

	static FormatReader& getInstance();

	MediaType Read(IMFMediaType *pType);
private:
	FormatReader(void);
	~FormatReader(void);
	FormatReader(const FormatReader&);
	FormatReader& operator=(const FormatReader&);
	HRESULT LogAttributeValueByIndexNew(IMFAttributes *pAttr, DWORD index, MediaType &out);
	unsigned int *GetParametr(GUID guid, MediaType &out);
	HRESULT GetGUIDNameNew(const GUID& guid, std::wstring &nameGUID);
	void LogUINT32AsUINT64New(const PROPVARIANT& var, UINT32 &uHigh, UINT32 &uLow);
	HRESULT SpecialCaseAttributeValueNew(GUID guid, const PROPVARIANT& var, MediaType &out);
	static LPCWSTR GetGUIDNameConstNew(const GUID& guid);

};

