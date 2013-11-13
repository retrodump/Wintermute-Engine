// wme_report.h : main header file for the wme_report DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include <imagehlp.h>

// Cwme_reportApp
// See wme_report.cpp for the implementation of this class
//
// based on dbghelp.h
typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
										 CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
										 CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
										 CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam
										 );


class CReportApp : public CWinApp
{
public:
	CReportApp();

// Overrides
public:
	virtual BOOL InitInstance();
	void CreateCrashLog(CString& FileName, CString& AppDescription, EXCEPTION_POINTERS* pExPtrs);
	void CreateMinidump(CString& FileName, EXCEPTION_POINTERS* pExPtrs);


	DECLARE_MESSAGE_MAP()	
	
};

