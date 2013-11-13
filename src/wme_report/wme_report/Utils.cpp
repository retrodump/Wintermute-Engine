#include "StdAfx.h"
#include "Utils.h"

//////////////////////////////////////////////////////////////////////////
CUtils::CUtils(void)
{
}

//////////////////////////////////////////////////////////////////////////
CUtils::~CUtils(void)
{
}

//////////////////////////////////////////////////////////////////////////
CString CUtils::GetTempPath()
{
	CString TempPath;
	::GetTempPath(MAX_PATH, TempPath.GetBuffer(MAX_PATH));
	TempPath.ReleaseBuffer();

	CString TempName;
	::GetTempFileName(TempPath, "wme", 0, TempName.GetBuffer(MAX_PATH));
	TempName.ReleaseBuffer();
	::DeleteFile(TempName);	
	TempName += "\\";

	TempName = MakePath(TempName);
		
	return TempName;
}

//////////////////////////////////////////////////////////////////////////
CString CUtils::GetAppDataPath()
{
	LPMALLOC pMalloc;
	if(SUCCEEDED(SHGetMalloc(&pMalloc)))
	{
		LPITEMIDLIST pidl;
		SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &pidl);
		if(pidl)
		{
			CString AppDataPath;
			SHGetPathFromIDList(pidl, AppDataPath.GetBuffer(MAX_PATH));
			AppDataPath.ReleaseBuffer();
			pMalloc->Free(pidl);
			AppDataPath += "\\";
			return AppDataPath;
		}
	}
	return "";
}

//////////////////////////////////////////////////////////////////////////
CString CUtils::GetZipName(const char* AppName, int VerMajor, int VerMinor, int VerBuild)
{
	CString Path = GetAppDataPath();	
	Path += "Wintermute Engine\\CrashReports\\";

	SYSTEMTIME st;
	::GetLocalTime(&st);

	CString FileName = "";
	FileName.Format("%04d%02d%02d-%02d-%02d-%02d-%s-%d.%d.%03d.zip",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond,
		AppName, VerMajor, VerMinor, VerBuild);

	Path += FileName;
	MakePath(Path);
	return Path;
}

//////////////////////////////////////////////////////////////////////////
CString CUtils::MakePath(CString Path)
{
	char drive[_MAX_DRIVE+1];
	char path[_MAX_PATH+1];
	int i;
	CString result;

	if(Path[Path.GetLength()-1] != '\\')
	{
		_splitpath(LPCSTR(Path), drive, path, NULL, NULL);
		Path = CString(drive) + CString(path);
	}
	if(Path[Path.GetLength()-1] != '\\') Path += '\\';


	for(i = 0; i<Path.GetLength(); i++)
		if(Path[i] == '/') Path.SetAt(i, '\\');

	for(i = 0; i<Path.GetLength(); i++)
		if(i == 0 || Path[i] != '\\' || (Path[i] == '\\' && Path[i-1] != '\\')){
			result += Path[i];
			if(Path[i] == '\\'){
				CreateDirectory(LPCSTR(result), NULL);
				DWORD attr = GetFileAttributes(LPCSTR(result));
				if(attr == 0xFFFFFFFF || (attr & FILE_ATTRIBUTE_DIRECTORY) == 0) return "";
			}

		}

		return result;
}

//////////////////////////////////////////////////////////////////////////
CString CUtils::GetFName(CString Filename)
{
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath(Filename, NULL, NULL, fname, ext);

	CString ret;
	ret.Format("%s%s", fname, ext);
	return ret;
}
