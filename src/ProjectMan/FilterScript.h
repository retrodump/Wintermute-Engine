// FilterScript.h: interface for the CFilterScript class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILTERSCRIPT_H__93AC86E5_361D_4440_A988_32A0702CAAB5__INCLUDED_)
#define AFX_FILTERSCRIPT_H__93AC86E5_361D_4440_A988_32A0702CAAB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PackagerFilter.h"
#include "../engine_core/wme_ad/dcgf_ad.h"

class CFilterScript : public CPackagerFilter  
{
public:
	CString m_CurrentFile;
	static void __stdcall ErrorCallback(int Line, char* Text, void* Data);
	virtual bool Initialize(CPackageBuilder::TPackage* Package);
	CAdGame* m_Game;
	virtual CString GetSection();
	virtual HRESULT DefaultSettings();
	CFilterScript(CProjectDoc* Doc);
	virtual ~CFilterScript();
	virtual CPackagerFilter::TProcessedType ProcessFile(CString FullFilename, CString Filename, CString OutputPath, CString& NewFilename, BYTE* Buffer, DWORD Size, BYTE** NewBuffer, DWORD* NewSize);
};

#endif // !defined(AFX_FILTERSCRIPT_H__93AC86E5_361D_4440_A988_32A0702CAAB5__INCLUDED_)
