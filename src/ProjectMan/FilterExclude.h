// FilterExclude.h: interface for the CFilterExclude class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILTEREXCLUDE_H__DBC9C0D4_6332_4A14_B4D4_FF45EAB18970__INCLUDED_)
#define AFX_FILTEREXCLUDE_H__DBC9C0D4_6332_4A14_B4D4_FF45EAB18970__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PackagerFilter.h"

class CFilterExclude : public CPackagerFilter  
{
public:
	CFilterExclude(CProjectDoc* Doc);
	virtual ~CFilterExclude();
	virtual CString GetSection();
	virtual HRESULT DefaultSettings();
	virtual CPackagerFilter::TProcessedType ProcessFile(CString FullFilename, CString Filename, CString OutputPath, CString& NewFilename, BYTE* Buffer, DWORD Size, BYTE** NewBuffer, DWORD* NewSize);
};

#endif // !defined(AFX_FILTEREXCLUDE_H__DBC9C0D4_6332_4A14_B4D4_FF45EAB18970__INCLUDED_)
