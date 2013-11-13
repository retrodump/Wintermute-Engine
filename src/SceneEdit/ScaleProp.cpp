// ScaleProp.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEdit.h"
#include "ScaleProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScaleProp dialog


CScaleProp::CScaleProp(CWnd* pParent /*=NULL*/)
	: CDialog(CScaleProp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScaleProp)
	m_PosY = 0;
	m_Scale = 0;
	//}}AFX_DATA_INIT
}


void CScaleProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScaleProp)
	DDX_Control(pDX, IDC_POS_Y, m_PosYEdit);
	DDX_Text(pDX, IDC_POS_Y, m_PosY);
	DDX_Text(pDX, IDC_SCALE, m_Scale);
	DDV_MinMaxUInt(pDX, m_Scale, 0, 1000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScaleProp, CDialog)
	//{{AFX_MSG_MAP(CScaleProp)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScaleProp message handlers
