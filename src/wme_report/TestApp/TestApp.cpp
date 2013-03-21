// TestApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


typedef void (*CreateReport)(const char* AppName, int VerMajor, int VerMinor, int VerBuild, EXCEPTION_POINTERS* pExPtrs, bool SendReport);
LONG SendReport(EXCEPTION_POINTERS* exPtr);

int _tmain(int argc, _TCHAR* argv[])
{
	__try
	{
		DWORD d = ::GetCurrentThreadId();

		BYTE* b = NULL;
		b[10] = 0;
		return 0;
	}
	__except(SendReport(GetExceptionInformation()))
	{
		return 0;
	}	
}



//////////////////////////////////////////////////////////////////////////
LONG SendReport(EXCEPTION_POINTERS* exPtr)
{

#ifdef _DEBUG
	HMODULE  hMod = LoadLibrary("..\\debug\\wme_report.dll");
#else
	HMODULE  hMod = LoadLibrary("..\\release\\wme_report.dll");
#endif

	CreateReport cr = (CreateReport)GetProcAddress(hMod, "CreateReport");
	cr("wme", 1, 7, 3, exPtr, true);

	//return EXCEPTION_CONTINUE_SEARCH;
	return EXCEPTION_EXECUTE_HANDLER;
}
