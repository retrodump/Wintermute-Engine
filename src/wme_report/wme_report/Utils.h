#pragma once

class CUtils
{
public:
	CUtils(void);
	virtual ~CUtils(void);
	static CString GetTempPath();
	static CString GetAppDataPath();
	static CString GetZipName(const char* AppName, int VerMajor, int VerMinor, int VerBuild);
	static CString MakePath(CString Path);
	static CString GetFName(CString Filename);
};
