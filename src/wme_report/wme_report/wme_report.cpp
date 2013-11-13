// wme_report.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxinet.h>
#include "wme_report.h"
#include "BugslayerUtil.h"
#include "Utils.h"
#include "zlibcpp.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//


// Cwme_reportApp

BEGIN_MESSAGE_MAP(CReportApp, CWinApp)
END_MESSAGE_MAP()


// Cwme_reportApp construction

//////////////////////////////////////////////////////////////////////////
CReportApp::CReportApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CReportApp object
CReportApp theApp;


//////////////////////////////////////////////////////////////////////////
BOOL CReportApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
extern "C" __declspec( dllexport ) void CreateReport(const char* AppName, int VerMajor, int VerMinor, int VerBuild, EXCEPTION_POINTERS* pExPtrs, bool SendReport)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (pExPtrs == NULL) return;

	CString AppDescript;
	AppDescript.Format("%s %d.%d.%03d", AppName, VerMajor, VerMinor, VerBuild);
	
	CString TempPath = CUtils::GetTempPath();
	CString CrashLogFileName = TempPath + "crash.log";
	CString DumpFileName = TempPath + "minidump.dmp";

	theApp.CreateMinidump(DumpFileName, pExPtrs);
	theApp.CreateCrashLog(CrashLogFileName, AppDescript, pExPtrs);	


	CString ZipFileName = CUtils::GetZipName(AppName, VerMajor, VerMinor, VerBuild);
	
	CZLib zip;
	zip.Open(ZipFileName);
	zip.AddFile(CrashLogFileName);
	zip.AddFile(DumpFileName);
	zip.Close();

	::DeleteFile(CrashLogFileName);
	::DeleteFile(DumpFileName);
	::RemoveDirectory(TempPath);

	if(SendReport)
	{
		CMainDlg dlg;
		dlg.m_ZipFile = ZipFileName;
		dlg.DoModal();
	}
}

//////////////////////////////////////////////////////////////////////////
void CReportApp::CreateCrashLog(CString& FileName, CString& AppDescription, EXCEPTION_POINTERS* pExPtrs)
{
	FILE* f = fopen(FileName, "a+");
	if(f)
	{
		SYSTEMTIME st;
		::GetLocalTime(&st);
		fprintf(f, "-----------------------------------------------------------------\n");
		fprintf(f, "---------- %s crash report: %02d-%02d-%04d, %02d:%02d ----------\n",
			AppDescription, st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute);
		fprintf(f, "-----------------------------------------------------------------\n");
		fprintf(f, "%s\n", GetFaultReason(pExPtrs));
		fprintf(f, "%s\n", GetRegisterString(pExPtrs));

		DWORD dwOpts = GSTSO_PARAMS | GSTSO_MODULE | GSTSO_SYMBOL | GSTSO_SRCLINE;

		fprintf(f, "Stack trace:\n");
		const char* szBuff = GetFirstStackTraceString(dwOpts, pExPtrs);

		do
		{
			fprintf(f, "%s\n", szBuff);
			fflush (f);
			szBuff = GetNextStackTraceString(dwOpts, pExPtrs);
		}
		while(szBuff!=NULL);

		fprintf(f, "\n");
		fclose(f);
	}
}

//////////////////////////////////////////////////////////////////////////
void CReportApp::CreateMinidump(CString& FileName, EXCEPTION_POINTERS* pExPtrs)
{
	HMODULE hDll = NULL;
	char szDbgHelpPath[_MAX_PATH];

	if (::GetModuleFileName(NULL, szDbgHelpPath, MAX_PATH))
	{
		char* pSlash = strchr(szDbgHelpPath, '\\');
		if (pSlash)
		{
			strcpy(pSlash+1, "DBGHELP.DLL");
			hDll = ::LoadLibrary(szDbgHelpPath);
		}
	}

	if (hDll==NULL)
	{
		// load any version we can
		hDll = ::LoadLibrary("DBGHELP.DLL");
	}

	LPCTSTR szResult = NULL;

	if (hDll)
	{
		MINIDUMPWRITEDUMP pDump = (MINIDUMPWRITEDUMP)::GetProcAddress( hDll, "MiniDumpWriteDump" );
		if (pDump)
		{
			// create the file
			HANDLE hFile = ::CreateFile(FileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

			if (hFile!=INVALID_HANDLE_VALUE)
			{
				_MINIDUMP_EXCEPTION_INFORMATION ExInfo;

				ExInfo.ThreadId = ::GetCurrentThreadId();
				ExInfo.ExceptionPointers = pExPtrs;
				ExInfo.ClientPointers = TRUE;

				// write the dump				
				pDump( GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL );
				::CloseHandle(hFile);
			}
		}
	}
}
