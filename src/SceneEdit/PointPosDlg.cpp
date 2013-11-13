// PointPosDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEdit.h"
#include "PointPosDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPointPosDlg dialog


CPointPosDlg::CPointPosDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPointPosDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPointPosDlg)
	m_PosY = 0;
	m_PosX = 0;
	//}}AFX_DATA_INIT
}


void CPointPosDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPointPosDlg)
	DDX_Control(pDX, IDC_POS_Y, m_PosYEdit);
	DDX_Control(pDX, IDC_POS_X, m_PosXEdit);
	DDX_Text(pDX, IDC_POS_Y, m_PosY);
	DDX_Text(pDX, IDC_POS_X, m_PosX);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPointPosDlg, CDialog)
	//{{AFX_MSG_MAP(CPointPosDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPointPosDlg message handlers
