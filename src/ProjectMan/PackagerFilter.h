// PackagerFilter.h: interface for the CPackagerFilter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKAGERFILTER_H__AEF7ADE6_185D_44C5_9E7E_50D63D9FA295__INCLUDED_)
#define AFX_PACKAGERFILTER_H__AEF7ADE6_185D_44C5_9E7E_50D63D9FA295__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PackageBuilder.h"
class CProjectDoc;

class CPackagerFilter  
{
public:
	virtual bool Initialize(CPackageBuilder::TPackage* Package);
	CProjectDoc* m_Document;
	bool FilenameMatches(CString Filename);
	typedef enum{
		PROC_ERROR, PROC_IGNORE, PROC_USE_ORIGINAL, PROC_USE_BUFFER, PROC_USE_ORIGINAL_UNCOMPRESSED, PROC_USE_BUFFER_UNCOMPRESSED
	} TProcessedType;
	typedef enum{
		FILTER_NONE, FILTER_EXCLUDE, FILTER_SCRIPT, FILTER_UNCOMPRESS, FILTER_COPY
	} TFilterType;

	TFilterType m_Type;
	virtual TProcessedType ProcessFile(CString FullFilename, CString Filename, CString OutputPath, CString& NewFilename, BYTE* Buffer, DWORD Size, BYTE** NewBuffer, DWORD* NewSize);
	virtual HRESULT SaveSettings(FILE* File);
	virtual CString GetSection();
	virtual HRESULT DefaultSettings();
	virtual HRESULT LoadSettings(CString Filename);
	CPackagerFilter(CProjectDoc* Doc);
	virtual ~CPackagerFilter();
	CStringArray m_Masks;
	bool m_Active;
};

#endif // !defined(AFX_PACKAGERFILTER_H__AEF7ADE6_185D_44C5_9E7E_50D63D9FA295__INCLUDED_)
