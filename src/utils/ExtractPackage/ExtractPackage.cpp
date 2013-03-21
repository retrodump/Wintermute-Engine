// ExtractPackage.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ExtractPackage.h"
#include "Package.h"
#include "utils_mfc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;


//////////////////////////////////////////////////////////////////////////
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)){
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		return 1;
	}

	if(argc!=3){
		printf("WME Package Extractor, (c) 2005 DEAD:CODE\n\n  Usage: ExtractPackage.exe package.dcp output_dir\n\n");
		return 0;
	}

	CString PackageName = CString(argv[1]);
	CString OutputDir = CString(argv[2]);

	if(OutputDir[OutputDir.GetLength()-1]!='\\') OutputDir+="\\";

	// read package
	CPackage Package;
	if(!Package.ReadPackage(PackageName)){
		printf("Error reading package contents: %s\n", PackageName);
		return 1;
	}

	// extract files
	bool ret = true;
	for(int i=0; i<Package.GetNumEntries(); i++){
		printf("Extracting: %s ...\n", Package.GetEntryName(i));

		if(MakePath(OutputDir + Package.GetEntryName(i))==""){
			ret = false;
			break;
		}
		else{
			ret = Package.ExtractEntry(i, OutputDir);
			if(!ret) break;
		}
	}

	if(ret){
		printf("\nALL OK\n");
		MessageBeep(MB_OK);
		return 0;
	}
	else{
		printf("\n**ERROR** extracting package\n");
		MessageBeep(MB_ICONERROR);
		return 1;
	}
}


