// EmptyPage.cpp : implementation file
//

#include "stdafx.h"
#include "SpriteEdit.h"
#include "EmptyPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEmptyPage dialog


CEmptyPage::CEmptyPage(CWnd* pParent /*=NULL*/)
	: CDialog(CEmptyPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEmptyPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CEmptyPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEmptyPage)
	DDX_Control(pDX, IDC_EMPTY_NOTE, m_EmptyNote);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEmptyPage, CDialog)
	//{{AFX_MSG_MAP(CEmptyPage)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmptyPage message handlers


//////////////////////////////////////////////////////////////////////////
void CEmptyPage::OnOK() 
{
	// do nothing
}


//////////////////////////////////////////////////////////////////////////
void CEmptyPage::OnCancel() 
{
	// do nothing
}


//////////////////////////////////////////////////////////////////////////
void CEmptyPage::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if(!::IsWindow(m_EmptyNote)) return;

	CRect rcClient, rc;
	GetClientRect(&rcClient);
	m_EmptyNote.GetClientRect(&rc);
		

	m_EmptyNote.SetWindowPos(NULL, (rcClient.Width() - rc.Width())/2, (rcClient.Height() - rc.Height())/2, rc.Width(), rc.Height(), SWP_NOACTIVATE | SWP_NOZORDER);	
}
