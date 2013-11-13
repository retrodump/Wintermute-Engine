// Package.h: interface for the CPackage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKAGE_H__0D35F2AA_A415_4705_9BD5_02198C813680__INCLUDED_)
#define AFX_PACKAGE_H__0D35F2AA_A415_4705_9BD5_02198C813680__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "BFileEntry.h"

class CPackage  
{
public:
	CString GetEntryName(int Entry);
	FILE* m_File;
	BYTE m_CD;
	char* m_Name;
	bool ExtractEntry(int Entry, CString TargetDir);
	int GetNumEntries();
	bool ReadPackage(CString Filename);
	CPackage();
	virtual ~CPackage();
	CArray<CBFileEntry*, CBFileEntry*> m_Files;
};

#endif // !defined(AFX_PACKAGE_H__0D35F2AA_A415_4705_9BD5_02198C813680__INCLUDED_)
