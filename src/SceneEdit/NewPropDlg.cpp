// NewPropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEdit.h"
#include "NewPropDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewPropDlg dialog



//////////////////////////////////////////////////////////////////////////
CNewPropDlg::CNewPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewPropDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewPropDlg)
	m_PropName = _T("");
	m_PropValue = _T("");
	//}}AFX_DATA_INIT
}


//////////////////////////////////////////////////////////////////////////
void CNewPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewPropDlg)
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDC_PROP_NAME, m_PropNameEdit);
	DDX_Text(pDX, IDC_PROP_NAME, m_PropName);
	DDX_Text(pDX, IDC_PROP_VALUE, m_PropValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewPropDlg, CDialog)
	//{{AFX_MSG_MAP(CNewPropDlg)
	ON_EN_CHANGE(IDC_PROP_NAME, OnChangePropName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewPropDlg message handlers


//////////////////////////////////////////////////////////////////////////
void CNewPropDlg::SetState()
{
	CString str;
	m_PropNameEdit.GetWindowText(str);

	m_OK.EnableWindow(str!="");
}


//////////////////////////////////////////////////////////////////////////
void CNewPropDlg::OnChangePropName() 
{
	SetState();
}


//////////////////////////////////////////////////////////////////////////
BOOL CNewPropDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetState();
	return TRUE;
}
