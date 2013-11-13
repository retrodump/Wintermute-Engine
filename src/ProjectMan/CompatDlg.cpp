// CompatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProjectMan.h"
#include "CompatDlg.h"


// CCompatDlg dialog

IMPLEMENT_DYNAMIC(CCompatDlg, CDialog)

//////////////////////////////////////////////////////////////////////////
CCompatDlg::CCompatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCompatDlg::IDD, pParent)
	, m_CompatKillMethodThreads(FALSE)
{

}

//////////////////////////////////////////////////////////////////////////
CCompatDlg::~CCompatDlg()
{
}

//////////////////////////////////////////////////////////////////////////
void CCompatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_COMPAT_KILL_METHOD_THREADS, m_CompatKillMethodThreads);
}


BEGIN_MESSAGE_MAP(CCompatDlg, CDialog)
END_MESSAGE_MAP()


// CCompatDlg message handlers

//////////////////////////////////////////////////////////////////////////
BOOL CCompatDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LOCWIN(this);

	return TRUE;
}
