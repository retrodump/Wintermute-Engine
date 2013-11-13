// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wme_report.h"
#include "MainDlg.h"
#include "FileUploader.h"
#include "Utils.h"
#include "IMapi.h"

// CMainDlg dialog

IMPLEMENT_DYNAMIC(CMainDlg, CDialog)

//////////////////////////////////////////////////////////////////////////
CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{

}

//////////////////////////////////////////////////////////////////////////
CMainDlg::~CMainDlg()
{
}

//////////////////////////////////////////////////////////////////////////
void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DLGICON, m_Icon);
	DDX_Control(pDX, IDC_DESCRIPTION, m_Description);
	DDX_Control(pDX, IDC_CONTENTS, m_ContentsLink);
	DDX_Control(pDX, IDC_EMAIL, m_EmailLink);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_STN_CLICKED(IDC_CONTENTS, &CMainDlg::OnContentsClick)
	ON_STN_CLICKED(IDC_EMAIL, &CMainDlg::OnMailClicked)
END_MESSAGE_MAP()


// CMainDlg message handlers

//////////////////////////////////////////////////////////////////////////
BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_Icon.SetIcon(::LoadIcon(NULL, IDI_ERROR));

	m_Description.SetWindowText("Wintermute Engine encountered a problem and needs to be closed.\n\nWe have created an error report that you can send to us. This report will help us to diagnose the cause of this error and improve our software.\n\nThis report doesn't contain any personal information.");

	::MessageBeep(MB_ICONERROR);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////
void CMainDlg::OnContentsClick()
{
	::ShellExecute(GetSafeHwnd(), "", m_ZipFile, NULL, NULL, SW_NORMAL);
}

//////////////////////////////////////////////////////////////////////////
void CMainDlg::OnMailClicked()
{
	CIMapi mail;

	if (mail.Error() == CIMapi::IMAPI_SUCCESS)
	{
		mail.To("bugreports@dead-code.org");            //  Set recipient name (me)
		mail.Subject(CString("WME crash report: ") + CUtils::GetFName(m_ZipFile));                //  Subject of this email
		mail.Attach(m_ZipFile);               //  Attaching a file

		// Put text of message in body
		mail.Text("");     //  Set body text
		mail.Send(MAPI_DIALOG);                               //  Now send the mail! 
	}
	else
	{
		MessageBox("Wintermute Engine was unable to send the e-mail.", "Wintermute Engine", MB_ICONERROR);
	}
}

//////////////////////////////////////////////////////////////////////////
bool CMainDlg::UploadFile()
{
	CFileUploader Uploader;
	CWaitCursor wait;
	if(!Uploader.SendFile(m_ZipFile, CUtils::GetFName(m_ZipFile)))
	{
		MessageBox(CString("Wintermute Engine was unable to send the error report.\n\n") + Uploader.m_LastError, "Wintermute Engine", MB_ICONERROR);
		return false;
	}
	else return true;
}

//////////////////////////////////////////////////////////////////////////
void CMainDlg::OnOK()
{
	if(UploadFile()) CDialog::OnOK();
}
