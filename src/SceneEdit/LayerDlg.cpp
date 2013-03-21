// LayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEdit.h"
#include "LayerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLayerDlg dialog


CLayerDlg::CLayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLayerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLayerDlg)
	m_Name = _T("");
	m_Height = 0;
	m_Width = 0;
	m_CloseUp = FALSE;
	//}}AFX_DATA_INIT

	m_MainLayer = false;
}


void CLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLayerDlg)
	DDX_Control(pDX, IDC_CLOSE_UP, m_BtnCloseUp);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_IS_MAIN, m_IsMain);
	DDX_Text(pDX, IDC_NAME, m_Name);
	DDX_Text(pDX, IDC_HEIGHT, m_Height);
	DDV_MinMaxInt(pDX, m_Height, 0, 10000);
	DDX_Text(pDX, IDC_WIDTH, m_Width);
	DDV_MinMaxInt(pDX, m_Width, 0, 10000);
	DDX_Check(pDX, IDC_CLOSE_UP, m_CloseUp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLayerDlg, CDialog)
	//{{AFX_MSG_MAP(CLayerDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLayerDlg message handlers

BOOL CLayerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_IsMain.ShowWindow(m_MainLayer?SW_SHOW:SW_HIDE);
	m_BtnCloseUp.ShowWindow(m_MainLayer?SW_HIDE:SW_SHOW);

	m_OK.m_nFlatStyle = CBCGButton::BUTTONSTYLE_SEMIFLAT;
	m_Cancel.m_nFlatStyle = CBCGButton::BUTTONSTYLE_SEMIFLAT;
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
