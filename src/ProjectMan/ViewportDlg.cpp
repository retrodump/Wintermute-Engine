// ViewportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "projectman.h"
#include "ViewportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewportDlg dialog


//////////////////////////////////////////////////////////////////////////
CViewportDlg::CViewportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CViewportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewportDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	SetRectEmpty(&m_Rect);
}


//////////////////////////////////////////////////////////////////////////
void CViewportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewportDlg)
	DDX_Control(pDX, IDC_WIDTH, m_Width);
	DDX_Control(pDX, IDC_POS_Y, m_PosY);
	DDX_Control(pDX, IDC_POS_X, m_PosX);
	DDX_Control(pDX, IDC_HEIGHT, m_Height);
	DDX_Control(pDX, IDC_FULLSCREEN, m_Fullscreen);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewportDlg, CDialog)
	//{{AFX_MSG_MAP(CViewportDlg)
	ON_BN_CLICKED(IDC_FULLSCREEN, OnFullscreen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewportDlg message handlers


//////////////////////////////////////////////////////////////////////////
BOOL CViewportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	LOCWIN(this);
	
	m_Fullscreen.SetCheck(IsRectEmpty(&m_Rect));

	CString str;
	str.Format("%d", m_Rect.left);
	m_PosX.SetWindowText(str);

	str.Format("%d", m_Rect.top);
	m_PosY.SetWindowText(str);

	str.Format("%d", m_Rect.right - m_Rect.left);
	m_Width.SetWindowText(str);

	str.Format("%d", m_Rect.bottom - m_Rect.top);
	m_Height.SetWindowText(str);

	SetState();

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
void CViewportDlg::SetState()
{
	m_PosX.EnableWindow(!m_Fullscreen.GetCheck());
	m_PosY.EnableWindow(!m_Fullscreen.GetCheck());
	m_Width.EnableWindow(!m_Fullscreen.GetCheck());
	m_Height.EnableWindow(!m_Fullscreen.GetCheck());
}


//////////////////////////////////////////////////////////////////////////
void CViewportDlg::OnFullscreen() 
{
	SetState();
}


//////////////////////////////////////////////////////////////////////////
void CViewportDlg::OnOK() 
{
	if(m_Fullscreen.GetCheck()) SetRectEmpty(&m_Rect);
	else{
		CString str;
		int X, Y, W, H;
		m_PosX.GetWindowText(str);
		X = atoi(str);

		m_PosY.GetWindowText(str);
		Y = atoi(str);

		m_Width.GetWindowText(str);
		W = atoi(str);

		m_Height.GetWindowText(str);
		H = atoi(str);

		SetRect(&m_Rect, X, Y, X+W, Y+H);
	}
	
	CDialog::OnOK();
}
