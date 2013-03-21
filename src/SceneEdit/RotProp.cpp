// RotProp.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEdit.h"
#include "RotProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRotProp dialog


CRotProp::CRotProp(CWnd* pParent /*=NULL*/)
	: CDialog(CRotProp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRotProp)
	m_Rotation = 0;
	//}}AFX_DATA_INIT
	m_PosX = 0;
	m_Rotation = 0;

}


void CRotProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRotProp)
	DDX_Control(pDX, IDC_POS_X, m_PosXEdit);
	DDX_Text(pDX, IDC_POS_X, m_PosX);
	DDX_Text(pDX, IDC_ROTATION, m_Rotation);
	DDV_MinMaxInt(pDX, m_Rotation, -45, 45);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRotProp, CDialog)
	//{{AFX_MSG_MAP(CRotProp)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRotProp message handlers
