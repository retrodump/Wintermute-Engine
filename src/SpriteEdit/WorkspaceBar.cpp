// WorkspaceBar.cpp : implementation of the CWorkspaceBar class
//

#include "stdafx.h"
#include "SpriteEdit.h"
#include "WorkspaceBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar

BEGIN_MESSAGE_MAP(CWorkspaceBar, CBCGSizingControlBar)
	//{{AFX_MSG_MAP(CWorkspaceBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar construction/destruction

CWorkspaceBar::CWorkspaceBar()
{
	// TODO: add one-time construction code here

}

CWorkspaceBar::~CWorkspaceBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

int CWorkspaceBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGSizingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	// Create tabs window:
	if (!m_wndTabs.Create (CBCGTabWnd::STYLE_3D, rectDummy, this, 1))
	{
		TRACE0("Failed to create workspace tab window\n");
		return -1;      // fail to create
	}

	m_wndTabs.SetImageList (IDB_WORKSPACE, 16, RGB (255, 0, 255));

	// Create tree windows.
	// TODO: create your own tab windows here:
	const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
								TVS_LINESATROOT | TVS_HASBUTTONS;
	
	if (!m_wndSprite.Create(IDD_SPRITE_PAGE, &m_wndTabs) ||
		!m_wndSpriteProp.Create(IDD_SPRITE_PROP_PAGE, &m_wndTabs))
	{
		TRACE0("Failed to create workspace view\n");
		return -1;      // fail to create
	}

	// Attach tree windows to tab:
	m_wndTabs.AddTab (&m_wndSprite, _T("Frames"), 0 /* Image number */);
	m_wndTabs.AddTab (&m_wndSpriteProp, _T("Properties"), 1 /* Image number */);

	return 0;
}


//////////////////////////////////////////////////////////////////////////
void CWorkspaceBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGSizingControlBar::OnSize(nType, cx, cy);

	// Tab control should cover a whole client area:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy,
		SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}



//////////////////////////////////////////////////////////////////////////
void CWorkspaceBar::ActivateSpriteView (CSpriteView* SpriteView){
	m_wndSprite.SetCurrentView(SpriteView);
	m_wndSpriteProp.SetCurrentView(SpriteView);
}


//////////////////////////////////////////////////////////////////////////
void CWorkspaceBar::Refresh()
{
	m_wndSprite.Redraw();
	m_wndSpriteProp.Redraw();
}
