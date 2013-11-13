// FilterUncompressed.h: interface for the CFilterUncompressed class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILTERUNCOMPRESSED_H__63A3F8B1_5736_4AC0_8E3F_D56E30C1A3A3__INCLUDED_)
#define AFX_FILTERUNCOMPRESSED_H__63A3F8B1_5736_4AC0_8E3F_D56E30C1A3A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PackagerFilter.h"

class CFilterUncompressed : public CPackagerFilter  
{
public:
	CFilterUncompressed(CProjectDoc* Doc);
	virtual ~CFilterUncompressed();
	virtual CString GetSection();
	virtual HRESULT DefaultSettings();
	virtual CPackagerFilter::TProcessedType ProcessFile(CString FullFilename, CString Filename, CString OutputPath, CString& NewFilename, BYTE* Buffer, DWORD Size, BYTE** NewBuffer, DWORD* NewSize);
};

#endif // !defined(AFX_FILTERUNCOMPRESSED_H__63A3F8B1_5736_4AC0_8E3F_D56E30C1A3A3__INCLUDED_)
