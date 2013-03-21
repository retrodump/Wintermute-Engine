#pragma once

#include <afxinet.h>

class CFileUploader
{
public:
	CFileUploader(void);
	virtual ~CFileUploader(void);

	BOOL SendFile(CString& strFullPath, CString& strFileName);
	CString m_LastError;

private:
	CString MakeRequestHeaders(CString& strBoundary);
	CString MakePreFileData(CString& strBoundary, CString& strFileName);
	CString MakePostFileData(CString& strBoundary);	
	BOOL Connect();
	BOOL Disconnect();
	CHttpConnection* m_Connection;
	CInternetSession* m_Session;
};
