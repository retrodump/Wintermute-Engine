// CompileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "projectman.h"
#include "CompileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCompileDlg dialog


CCompileDlg::CCompileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCompileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCompileDlg)
	//}}AFX_DATA_INIT

	m_CloseRequest = false;
}


void CCompileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCompileDlg)
	DDX_Control(pDX, IDC_TITLE_2, m_Title2);
	DDX_Control(pDX, IDC_TITLE_1, m_Title1);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCompileDlg, CDialog)
	//{{AFX_MSG_MAP(CCompileDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCompileDlg message handlers


//////////////////////////////////////////////////////////////////////////
void CCompileDlg::OnOK() 
{
	//CDialog::OnOK();
}


//////////////////////////////////////////////////////////////////////////
void CCompileDlg::OnCancel() 
{
	m_CloseRequest = true;
	//CDialog::OnCancel();
}


//////////////////////////////////////////////////////////////////////////
void CCompileDlg::Update()
{
	MSG message;
	while(::PeekMessage(&message, NULL, 0, 0, PM_REMOVE)){
		::TranslateMessage(&message);
		::DispatchMessage(&message);
	}
}


//////////////////////////////////////////////////////////////////////////
BOOL CCompileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	LOCWIN(this);
	
	return TRUE;
}
