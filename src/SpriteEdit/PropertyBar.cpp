// PropertyBar.cpp : implementation file
//

#include "stdafx.h"
#include "SpriteEdit.h"
#include "PropertyBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyBar

CPropertyBar::CPropertyBar()
{
}

CPropertyBar::~CPropertyBar()
{
}


BEGIN_MESSAGE_MAP(CPropertyBar, CBCGSizingControlBar)
	//{{AFX_MSG_MAP(CPropertyBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPropertyBar message handlers

int CPropertyBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGSizingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	if (!m_wndEmpty.Create(IDD_EMPTY_PAGE, this) || !m_wndFrame.Create(IDD_FRAME_PAGE, this))
	{
		TRACE0("Failed to create property page\n");
		return -1;      // fail to create
	}

	m_wndEmpty.ShowWindow(SW_SHOW);

	return 0;
}


//////////////////////////////////////////////////////////////////////////
void CPropertyBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGSizingControlBar::OnSize(nType, cx, cy);
	
	m_wndEmpty.SetWindowPos  (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFrame.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}


//////////////////////////////////////////////////////////////////////////
void CPropertyBar::ShowNone()
{
	m_wndEmpty.ShowWindow(SW_SHOW);
	m_wndFrame.ShowWindow(SW_HIDE);

	SetTitle("Properties");
}


//////////////////////////////////////////////////////////////////////////
void CPropertyBar::ShowFrame(CSpriteView* View, CBFrame* Frame, CBSubFrame* Subframe, bool Force)
{
	m_wndEmpty.ShowWindow(SW_HIDE);
	m_wndFrame.ShowWindow(SW_SHOW);

	SetTitle("Frame properties");
	m_wndFrame.ShowFrame(View, Frame, Subframe, Force);
}


//////////////////////////////////////////////////////////////////////////
void CPropertyBar::SetTitle(const char *Title)
{
	CString str;
	GetWindowText(str);
	if(str.Compare(Title)!=0) SetWindowText(Title);
}
