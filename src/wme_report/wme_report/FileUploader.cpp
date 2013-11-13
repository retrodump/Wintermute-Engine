#include "StdAfx.h"
#include "FileUploader.h"

//////////////////////////////////////////////////////////////////////////
CFileUploader::CFileUploader(void)
{
	m_Session = NULL;
	m_Connection = NULL;
	m_LastError = "";
}

//////////////////////////////////////////////////////////////////////////
CFileUploader::~CFileUploader(void)
{
	Disconnect();
}


//////////////////////////////////////////////////////////////////////////
CString CFileUploader::MakeRequestHeaders(CString& strBoundary)
{
	CString strFormat;
	CString strData;

	strFormat = "Content-Type: multipart/form-data; boundary=%s\r\n";
	strData.Format(strFormat, strBoundary);

	return strData;
}

//////////////////////////////////////////////////////////////////////////
CString CFileUploader::MakePreFileData(CString& strBoundary, CString& strFileName)
{
	CString strFormat;
	CString strData;

	strFormat += _T("--%s");
	strFormat += _T("\r\n");
	strFormat += _T("Content-Disposition: form-data; name=\"crashfile\"; filename=\"%s\"");
	strFormat += _T("\r\n");
	strFormat += _T("Content-Type: application/zip");
	strFormat += _T("\r\n");
	strFormat += _T("Content-Transfer-Encoding: binary");
	strFormat += _T("\r\n\r\n");

	strData.Format(strFormat, strBoundary, strFileName);

	return strData;

}

//////////////////////////////////////////////////////////////////////////
CString CFileUploader::MakePostFileData(CString& strBoundary)
{

	CString strFormat;
	CString strData;

	strFormat = _T("\r\n");
	strFormat += _T("--%s");
	strFormat += _T("\r\n");
	strFormat += _T("Content-Disposition: form-data; name=\"submitted\"");
	strFormat += _T("\r\n\r\n");
	strFormat += _T("hello");
	strFormat += _T("\r\n");
	strFormat += _T("--%s--");
	strFormat += _T("\r\n");

	strData.Format(strFormat, strBoundary, strBoundary);

	return strData;
}


//////////////////////////////////////////////////////////////////////////
BOOL CFileUploader::SendFile(CString& strFullPath, CString& strFileName)
{
	CFile ZipFile;
	CHttpFile* pHTTP;
	CString strHTTPBoundary;
	CString strPreFileData;
	CString strPostFileData;
	DWORD dwTotalRequestLength;
	DWORD dwChunkLength;
	DWORD dwReadLength;
	DWORD dwResponseLength;
	TCHAR szError[MAX_PATH];
	void* pBuffer;
	LPSTR szResponse;
	CString strResponse;
	BOOL bSuccess = TRUE;

	CString strDebugMessage;


	m_LastError = "";

	if (!Connect())
	{
		return FALSE;
	}

	if (FALSE == ZipFile.Open(strFullPath, CFile::modeRead | CFile::shareDenyWrite))
	{
		Disconnect();
		return FALSE;
	}

	strHTTPBoundary = "WmeBoundaryDataWme";
	strPreFileData = MakePreFileData(strHTTPBoundary, strFileName);
	strPostFileData = MakePostFileData(strHTTPBoundary);

	dwTotalRequestLength = strPreFileData.GetLength() + strPostFileData.GetLength() + (DWORD)ZipFile.GetLength();

	dwChunkLength = 64 * 1024;

	pBuffer = malloc(dwChunkLength);

	if (pBuffer == NULL)
	{
		Disconnect();
		return FALSE;
	}

	try
	{
		pHTTP = m_Connection->OpenRequest(CHttpConnection::HTTP_VERB_POST, "/crashrpt.php");
		pHTTP->AddRequestHeaders(MakeRequestHeaders(strHTTPBoundary));
		pHTTP->SendRequestEx(dwTotalRequestLength, HSR_SYNC | HSR_INITIATE);
		pHTTP->Write((LPSTR)(LPCSTR)strPreFileData, strPreFileData.GetLength());


		dwReadLength = -1;
		while (0 != dwReadLength)
		{
			dwReadLength = ZipFile.Read(pBuffer, dwChunkLength);
			if (0 != dwReadLength)
			{
				pHTTP->Write(pBuffer, dwReadLength);
			}
		}
		pHTTP->Write((LPSTR)(LPCSTR)strPostFileData, strPostFileData.GetLength());


		pHTTP->EndRequest(HSR_SYNC);

		dwResponseLength = (DWORD)pHTTP->GetLength();
		while (0 != dwResponseLength)
		{
			szResponse = (LPSTR)malloc(dwResponseLength + 1);
			szResponse[dwResponseLength] = '\0';
			pHTTP->Read(szResponse, dwResponseLength);
			strResponse += szResponse;
			free(szResponse);
			dwResponseLength = (DWORD)pHTTP->GetLength();
		}
		//AfxMessageBox(strResponse);
	}
	catch (CException* e)
	{
		e->GetErrorMessage(szError, MAX_PATH);
		e->Delete();
		//AfxMessageBox(szError);
		m_LastError = CString(szError);
		bSuccess = FALSE;
	}

	pHTTP->Close();
	delete pHTTP;

	ZipFile.Close();

	if (pBuffer != NULL)
	{
		free(pBuffer);
	}

	Disconnect();

	return bSuccess;
}

//////////////////////////////////////////////////////////////////////////
BOOL CFileUploader::Connect()
{
	Disconnect();

	TCHAR szError[MAX_PATH];
	m_Session = new CInternetSession();
	try
	{
		m_Connection = m_Session->GetHttpConnection("dead-code.org");
	}
	catch (CException* e)
	{
		e->GetErrorMessage(szError, MAX_PATH);
		e->Delete();
		//AfxMessageBox(szError);
		m_LastError = CString(szError);
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
BOOL CFileUploader::Disconnect()
{
	if(m_Connection) delete m_Connection;
	m_Connection = NULL;

	if(m_Session) delete m_Session;
	m_Session = NULL;

	return TRUE;
}