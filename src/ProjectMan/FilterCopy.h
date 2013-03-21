#pragma once
#include "PackagerFilter.h"

class CFilterCopy :	public CPackagerFilter
{
public:
	CFilterCopy(CProjectDoc* Doc);
	virtual ~CFilterCopy(void);

	virtual CString GetSection();
	virtual HRESULT DefaultSettings();
	virtual CPackagerFilter::TProcessedType ProcessFile(CString FullFilename, CString Filename, CString OutputPath, CString& NewFilename, BYTE* Buffer, DWORD Size, BYTE** NewBuffer, DWORD* NewSize);
};
