// WorkspaceBar.cpp : implementation of the CWorkspaceBar class
//

#include "stdafx.h"
#include "SceneEdit.h"
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
	
	if (!m_wndScene.Create(IDD_SCENE_PAGE, &m_wndTabs) ||
		!m_wndSceneProp.Create(IDD_SCENE_PROP_PAGE, &m_wndTabs) ||
		!m_wnd3DProp.Create(IDD_3D_PAGE, &m_wndTabs)
		)
	{
		TRACE0("Failed to create workspace view\n");
		return -1;      // fail to create
	}


	// Attach tree windows to tab:
	m_wndTabs.AddTab (&m_wndScene, _T("Layout"), 0 /* Image number */);
	m_wndTabs.AddTab (&m_wndSceneProp, _T("Properties"), 1 /* Image number */);
	m_wndTabs.AddTab (&m_wnd3DProp, _T("3D"), 2 /* Image number */);

	return 0;
}

void CWorkspaceBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGSizingControlBar::OnSize(nType, cx, cy);

	// Tab control should cover a whole client area:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy,
		SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}



//////////////////////////////////////////////////////////////////////////
void CWorkspaceBar::ActivateSceneView (CSceneView* SceneView){
	m_wndScene.SetCurrentView(SceneView);
	m_wndSceneProp.SetCurrentView(SceneView);
	m_wnd3DProp.SetCurrentView(SceneView);
}


//////////////////////////////////////////////////////////////////////////
void CWorkspaceBar::Refresh(bool Full)
{
	if(Full) m_wndScene.Redraw();
	else m_wndScene.RedrawNodes();

	m_wndSceneProp.Redraw();
	m_wnd3DProp.Redraw();
}
