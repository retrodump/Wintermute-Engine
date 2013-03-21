// ReminderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProjectMan.h"
#include "ReminderDlg.h"


// CReminderDlg dialog

IMPLEMENT_DYNAMIC(CReminderDlg, CDialog)

CReminderDlg::CReminderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReminderDlg::IDD, pParent)
{
	m_SelectedOption = 0;
}

CReminderDlg::~CReminderDlg()
{
}

void CReminderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PAGE, m_DonationPage);
	DDX_Control(pDX, IDC_REMIND_LATER, m_BtnRemindLater);
	DDX_Control(pDX, IDC_ALREADY_DONATED, m_BtnAlreadyDonated);
	DDX_Control(pDX, IDC_DONT_WANNA_DONATE, m_BtnDontWannaDonate);
}


BEGIN_MESSAGE_MAP(CReminderDlg, CDialog)
END_MESSAGE_MAP()


// CReminderDlg message handlers

//////////////////////////////////////////////////////////////////////////
BOOL CReminderDlg::OnInitDialog()
{	
	CDialog::OnInitDialog();

	LOCWIN(this);
	m_DonationPage.m_link = "http://dead-code.org/redir.php?target=donate";
	m_BtnRemindLater.SetCheck(BST_CHECKED);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
void CReminderDlg::OnOK()
{
	if(m_BtnRemindLater.GetCheck()) m_SelectedOption = 0;
	else if(m_BtnAlreadyDonated.GetCheck()) m_SelectedOption = 1;
	else if(m_BtnDontWannaDonate.GetCheck()) m_SelectedOption = 2;

	CDialog::OnOK();

	if(m_SelectedOption==1) MessageBox(LOC("/str1141/Thank you for supporting Wintermute Engine!"), NULL, MB_ICONINFORMATION);
}
