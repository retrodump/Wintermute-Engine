// BgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpriteEdit.h"
#include "BgDlg.h"
#include "../mfc_shared/utils_mfc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBgDlg dialog


//////////////////////////////////////////////////////////////////////////
CBgDlg::CBgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBgDlg)
	m_Alpha = 0;
	m_Filename = _T("");
	m_OffsetX = 0;
	m_OffsetY = 0;
	//}}AFX_DATA_INIT

	m_Game = NULL;
}


//////////////////////////////////////////////////////////////////////////
void CBgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBgDlg)
	DDX_Control(pDX, IDC_OFFSET_Y, m_EdOffsetY);
	DDX_Control(pDX, IDC_OFFSET_X, m_EdOffsetX);
	DDX_Control(pDX, IDC_FILENAME, m_EdFilename);
	DDX_Control(pDX, IDC_ALPHA, m_EdAlpha);
	DDX_Control(pDX, IDC_ALPHA_SPIN, m_AlphaSpin);
	DDX_Text(pDX, IDC_ALPHA, m_Alpha);
	DDV_MinMaxInt(pDX, m_Alpha, 0, 255);
	DDX_Text(pDX, IDC_FILENAME, m_Filename);
	DDX_Text(pDX, IDC_OFFSET_X, m_OffsetX);
	DDX_Text(pDX, IDC_OFFSET_Y, m_OffsetY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBgDlg, CDialog)
	//{{AFX_MSG_MAP(CBgDlg)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_NO_IMAGE, OnNoImage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBgDlg message handlers


//////////////////////////////////////////////////////////////////////////
BOOL CBgDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	m_AlphaSpin.SetRange(0, 255);
	
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
void CBgDlg::OnOK() 
{
	int NewVal;
	CString str;
	m_EdAlpha.GetWindowText(str);
	NewVal = atoi(LPCSTR(str));
	NewVal = min(NewVal, 255);
	NewVal = max(NewVal, 0);

	str.Format("%d", NewVal);
	m_EdAlpha.SetWindowText(str);
	
	CDialog::OnOK();
}


//////////////////////////////////////////////////////////////////////////
void CBgDlg::OnBrowse() 
{
	CString OrigFile = m_Filename;
	CString NewFile = GetRelativeFilename(m_Game, OrigFile, "Sprite Files (*.bmp; *.tga; *.png; *.jpg; *.sprite)|*.bmp;*.tga;*.png;*.jpg;*.sprite", ((CSpriteEditApp*)AfxGetApp())->m_LastDirImage, this);
	if(NewFile!=""){
		m_Filename = NewFile;
		m_EdFilename.SetWindowText(NewFile);
	}
}


//////////////////////////////////////////////////////////////////////////
void CBgDlg::OnNoImage() 
{
	m_Filename = "";
	m_EdFilename.SetWindowText("");
}
