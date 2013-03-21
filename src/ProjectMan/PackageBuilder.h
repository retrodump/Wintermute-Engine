// PackageBuilder.h: interface for the CPackageBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKAGEBUILDER_H__0CE0EEB1_9089_47F2_A110_3ADE72F73954__INCLUDED_)
#define AFX_PACKAGEBUILDER_H__0CE0EEB1_9089_47F2_A110_3ADE72F73954__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afxtempl.h"
#include "Package.h"

class CProjectDoc;
class CCompileDlg;

class CPackageBuilder  
{
public:	
	bool AppendFiles(CString File1, CString File2);
	typedef struct{
		CString Name;
		CString FullName;
		CString ShortName;
		DWORD Length;
		DWORD CompressedLength;
		DWORD PackageOffset;
		DWORD Flags;
		DWORD TimeDate1;
		DWORD TimeDate2;
		bool Valid;
	} TFile;


	typedef struct{
		CString Name;
		CString FullName;	
		BYTE Priority;
		BYTE CD;
		int GameVersion;
		CString Description;
		CArray<TFile*, TFile*> m_Files;
	} TPackage;

	static CString Copyright;
	void DeleteAllPAckages(CString Path, CPackage* SinglePackage);
	bool CreatePackage(TPackage* Package, CCompileDlg* dlg, CString OutputPath);
	bool CreateMasterPackage(CCompileDlg* dlg, CString OutputPath);
	void Cleanup();
	bool GetAllFiles(TPackage* Package, CString Path);
	int m_TotalFiles;
	int m_ProcessedFiles;
	bool Compile(CPackage* SinglePackage=NULL);
	CPackageBuilder(CProjectDoc* Doc);
	virtual ~CPackageBuilder();
	CProjectDoc* m_Doc;
	CArray<TPackage*, TPackage*> m_Packages;
	void WriteString(FILE* f, const char* str, BYTE xor=0);
	bool AddGDF(CString ExeName);
};

#endif // !defined(AFX_PACKAGEBUILDER_H__0CE0EEB1_9089_47F2_A110_3ADE72F73954__INCLUDED_)
