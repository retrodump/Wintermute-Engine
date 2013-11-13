// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BFILEMANAGER_H__46BF5841_1837_11D4_9F37_9067C7F29A3D__INCLUDED_)
#define AFX_BFILEMANAGER_H__46BF5841_1837_11D4_9F37_9067C7F29A3D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <map>
#include "coll_templ.h"

class CBFileManager:CBBase
{
public:	
	bool FindPackageSignature(FILE* f, DWORD* Offset);
	HRESULT Cleanup();
	HRESULT SetBasePath(char* Path);
	HRESULT RestoreCurrentDir();
	char* m_BasePath;
	bool GetFullPath(char* Filename, char* Fullname);
	CBFile* OpenFileRaw(char* Filename);
	HRESULT CloseFile(CBFile* File);
	CBFile* OpenFile(char* Filename, bool AbsPathWarning=true);
	CBFileEntry* GetPackageEntry(char* Filename);
	FILE* OpenSingleFile(char* Name);
	FILE* OpenPackage(char* Name);
	HRESULT RegisterPackages();
	HRESULT InitPaths();
	HRESULT ReloadPaths();
	typedef enum{
		PATH_PACKAGE, PATH_SINGLE
	} TPathType;
	HRESULT AddPath(TPathType Type, char* Path);
	HRESULT RequestCD(int CD, char* PackageFile, char* Filename);
	HRESULT SaveFile(char* Filename, BYTE* Buffer, DWORD BufferSize, bool Compressed=false, BYTE* PrefixBuffer=NULL, DWORD PrefixSize=0);
	void MakeRelativePath(char* Path);
	bool MakeAbsolutePath(char* RelPath, char* AbsPath, int BufSize);
	BYTE* ReadWholeFile(char* Filename, DWORD* Size=NULL, bool MustExist=true);
	CBFileManager(CBGame* inGame=NULL);
	virtual ~CBFileManager();
	CBArray<char*, char*> m_SinglePaths;
	CBArray<char*, char*> m_PackagePaths;
	CBArray<CBPackage*, CBPackage*> m_Packages;
	CBArray<CBFile*, CBFile*> m_OpenFiles;

	map<string, CBFileEntry*> m_Files;
private:
	HRESULT RegisterPackage(char* Path, char* Name, bool SearchSignature=false);	
	map<string, CBFileEntry*>::iterator m_FilesIter;
	
};

#endif // !defined(AFX_BFILEMANAGER_H__46BF5841_1837_11D4_9F37_9067C7F29A3D__INCLUDED_)
