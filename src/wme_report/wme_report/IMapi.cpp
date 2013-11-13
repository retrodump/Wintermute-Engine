/*
 *	$Header: /common/IMapi.cpp 2     8/05/01 10:38p Admin $
 *
 *	$History: IMapi.cpp $
 * 
 * *****************  Version 2  *****************
 * User: Admin        Date: 8/05/01    Time: 10:38p
 * Updated in $/common
 * Fixes to the /msg crash bug.  Changed the trigger point for Autostrut
 * and auto scores.  Added auto scores.
 * 
 * *****************  Version 1  *****************
 * User: Admin        Date: 28/03/01   Time: 8:42p
 * Created in $/common
 */
#include "stdafx.h"
#include <mapi.h>
#include "imapi.h"

HINSTANCE CIMapi::m_hInstMail = (HINSTANCE) NULL;
BOOL	  CIMapi::m_isMailAvail = (BOOL) -1;

CIMapi::CIMapi()
{
	m_error = 0;										//	Initially error free
	m_sessionHandle = LHANDLE(NULL);

	memset(&m_message, 0, sizeof(MapiMessage));
	memset(&m_from, 0, sizeof(MapiRecipDesc));
	m_message.lpOriginator = &m_from;
	m_from.ulRecipClass = MAPI_ORIG;

	if (m_hInstMail == (HINSTANCE) NULL)				//	Load the MAPI dll
		m_hInstMail = ::LoadLibraryA("MAPI32.DLL");

	if (m_hInstMail == (HINSTANCE) NULL)
	{
#ifndef SERVICE
		AfxMessageBox(AFX_IDP_FAILED_MAPI_LOAD);
#endif
		m_error = IMAPI_LOADFAILED;
		return;
	}

	ASSERT(m_hInstMail != (HINSTANCE) NULL);			//	Now get the pointer to the send function
	(FARPROC&) m_lpfnSendMail = GetProcAddress(m_hInstMail, "MAPISendMail");
	(FARPROC&) m_lpfnMAPILogon = GetProcAddress(m_hInstMail, "MAPILogon");

	if (m_lpfnSendMail == NULL || m_lpfnMAPILogon == NULL)
	{
#ifndef SERVICE
		AfxMessageBox(AFX_IDP_INVALID_MAPI_DLL);
#endif
		m_error = IMAPI_INVALIDDLL;
		return;
	}
}

CIMapi::~CIMapi()
{
	if (m_hInstMail != (HINSTANCE) NULL)
		::FreeLibrary(m_hInstMail);

	m_hInstMail = (HINSTANCE) NULL;
	
	free(m_message.lpFiles);
	free(m_message.lpRecips);
	free(m_message.lpszSubject);
}

BOOL CIMapi::HasEmail()
{
	if (m_isMailAvail == (BOOL) -1)
		m_isMailAvail = ::GetProfileInt(_T("MAIL"), _T("MAPI"), 0) != 0 && SearchPath(NULL, _T("MAPI32.DLL"), NULL, 0, NULL, NULL) != 0;

	return m_isMailAvail;
}

UINT CIMapi::Error()
{
	UINT temp = m_error;

	m_error = IMAPI_SUCCESS;
	return temp;
}

BOOL CIMapi::AllocNewTo()
{
	//	Allocate a new MapiRecipDesc structure and initialise it to all zeros
	m_message.lpRecips = (MapiRecipDesc *) realloc(m_message.lpRecips, (m_message.nRecipCount + 1) * sizeof(MapiRecipDesc));
	memset(&m_message.lpRecips[m_message.nRecipCount], 0, sizeof(MapiRecipDesc));

	ASSERT(m_message.lpRecips);
	return m_message.lpRecips != (MapiRecipDesc *) NULL;
}

BOOL CIMapi::To(LPCTSTR recip)
{
	LPTSTR temp = new TCHAR[_tcslen(recip) + 1],
		   target;

	_tcscpy(temp, recip);
	target = _tcstok(temp, _T(";"));

	while (target != LPTSTR(NULL))
	{
		if (AllocNewTo())
		{
			//	We succeeded in allocating a new recipient record
			m_message.lpRecips[m_message.nRecipCount].lpszName = (LPTSTR) malloc(_tcslen(target) + 1);
			_tcscpy(m_message.lpRecips[m_message.nRecipCount].lpszName, target);
			m_message.lpRecips[m_message.nRecipCount].ulRecipClass = MAPI_TO;
			m_message.nRecipCount++;
			target = _tcstok(NULL, _T(";"));
		}
		else
		{
			m_error = IMAPI_FAILTO;
			delete []  temp;
			return FALSE;
		}
	}

	delete []  temp;
	return TRUE;
}

void CIMapi::Subject(LPCTSTR subject)
{
	m_message.lpszSubject = (LPTSTR) calloc(_tcslen(subject) + 1, sizeof(TCHAR));
	_tcscpy(m_message.lpszSubject, subject);
}

BOOL CIMapi::Attach(LPCTSTR path, LPCTSTR name)
{
	//	Add a new attachment record
	m_message.lpFiles = (MapiFileDesc *) realloc(m_message.lpFiles, (m_message.nFileCount + 1) * sizeof(MapiFileDesc));
	memset(&m_message.lpFiles[m_message.nFileCount], 0, sizeof(MapiFileDesc));

	ASSERT(m_message.lpFiles);
	
	if (m_message.lpFiles == (MapiFileDesc *) NULL)
	{
		m_error = IMAPI_FAILATTACH;
		return FALSE;
	}

	m_message.lpFiles[m_message.nFileCount].lpszPathName = (LPTSTR) malloc(_tcslen(path) + 1);
	_tcscpy(m_message.lpFiles[m_message.nFileCount].lpszPathName, path);

	if (name != (LPCTSTR) NULL)
	{
		m_message.lpFiles[m_message.nFileCount].lpszFileName = (LPTSTR) malloc(_tcslen(name) + 1);
		_tcscpy(m_message.lpFiles[m_message.nFileCount].lpszFileName, name);
	}

	m_message.nFileCount++;
	return TRUE;
}

BOOL CIMapi::Send(ULONG flags)
{
#ifndef SERVICE
	CWaitCursor wait;
#endif
	int			offset = m_text.GetLength();

	//	Add 1 space per attachment at the end of the body text.
	m_text += CString(' ', m_message.nFileCount);

	//	Set each attachment to replace one of the added spaces at the end of the body text.
	for (UINT i = 0; i < m_message.nFileCount; i++)
		m_message.lpFiles[i].nPosition = offset++;

	m_message.lpszNoteText = (LPTSTR) (LPCTSTR) m_text;	//  Set the body text

#ifndef SERVICE
	// prepare for modal dialog box
	AfxGetApp()->EnableModeless(FALSE);
	HWND hWndTop;
	CWnd* pParentWnd = CWnd::GetSafeOwner(NULL, &hWndTop);

	// some extra precautions are required to use MAPISendMail as it
	// tends to enable the parent window in between dialogs (after
	// the login dialog, but before the send note dialog).
	pParentWnd->SetCapture();
	::SetFocus(NULL);
	pParentWnd->m_nFlags |= WF_STAYDISABLED;
#endif

#ifdef SERVICE
	int nError /*= m_lpfnMAPILogon(0, _T("Rob"), _T("mapi40"), MAPI_NEW_SESSION, 0, &m_sessionHandle)*/;
	
	nError = m_lpfnSendMail(m_sessionHandle, 0x00000040, &m_message, flags, 0);
#else
	int nError = m_lpfnSendMail(0, (ULONG) pParentWnd->GetSafeHwnd(), &m_message, MAPI_LOGON_UI | flags, 0);
#endif

#ifndef SERVICE
	// after returning from the MAPISendMail call, the window must
	// be re-enabled and focus returned to the frame to undo the workaround
	// done before the MAPI call.
	::ReleaseCapture();
	pParentWnd->m_nFlags &= ~WF_STAYDISABLED;

	pParentWnd->EnableWindow(TRUE);
	::SetActiveWindow(NULL);
	pParentWnd->SetActiveWindow();
	pParentWnd->SetFocus();
	
	if (hWndTop != NULL)
		::EnableWindow(hWndTop, TRUE);
	
	AfxGetApp()->EnableModeless(TRUE);
#endif

	//	Now free malloced recipients
	for (i = 0; i < m_message.nRecipCount; i++)
		free(m_message.lpRecips[i].lpszName);
 
	//	Then free malloced attachments
	for (i = 0; i < m_message.nFileCount; i++)
	{
		free(m_message.lpFiles[i].lpszPathName);
		free(m_message.lpFiles[i].lpszFileName);
	}

	if (nError != SUCCESS_SUCCESS && nError != MAPI_USER_ABORT && nError != MAPI_E_LOGIN_FAILURE)
	{
#ifndef SERVICE
		AfxMessageBox(AFX_IDP_FAILED_MAPI_SEND);
#endif
		return FALSE;
	}

	return TRUE;
}
