// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "SpriteEdit.h"
#include "../mfc_shared/utils_mfc.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

const int  iMaxUserToolbars		= 10;
const UINT uiFirstUserToolBarId	= AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId	= uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_COMMAND(ID_VIEW_PROPERTY, OnViewProperty)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PROPERTY, OnUpdateViewProperty)
	ON_COMMAND(ID_HELP_DOCS, OnHelpDocs)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_WINDOW_MANAGER, OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, OnViewCustomize)
	ON_REGISTERED_MESSAGE(BCGM_RESETTOOLBAR, OnToolbarReset)
	ON_REGISTERED_MESSAGE(BCGM_TOOLBARMENU, OnToolbarContextMenu)
	ON_COMMAND_EX_RANGE(ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10, OnToolsViewUserToolbar)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10, OnUpdateToolsViewUserToolbar)
	ON_COMMAND(ID_VIEW_WORKSPACE, OnViewWorkspace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE, OnUpdateViewWorkspace)
	ON_COMMAND(ID_VIEW_OUTPUT, OnViewOutput)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, OnUpdateViewOutput)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CURSOR,
	ID_INDICATOR_MODE,
	ID_INDICATOR_FPS,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// enable Office XP look:
	CBCGVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGVisualManagerXP));

	// Load toolbar user images:
	if (!m_UserImages.Load (_T(".\\UserImages.bmp")))
	{
		TRACE(_T("Failed to load user images\n"));
		ASSERT (FALSE);
	}
	else
	{
		CBCGToolBar::SetUserImages (&m_UserImages);
	}

	CBCGToolBar::EnableQuickCustomization ();

	EnableMDITabs ();

	if (!m_wndMenuBar.Create (this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if (!m_wndWorkSpace.Create (_T("Sprite"), this, CSize (228, 150),
		TRUE /* Has gripper */, ID_VIEW_WORKSPACE,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT))
	{
		TRACE0("Failed to create workspace bar\n");
		return -1;      // fail to create
	}

	if (!m_wndProperty.Create (_T("Properties"), this, CSize (228, 200),
		TRUE /* Has gripper */, ID_VIEW_PROPERTY,
		WS_CHILD | WS_VISIBLE | CBRS_BOTTOM))
	{
		TRACE0("Failed to create property bar\n");
		return -1;      // fail to create
	}


	if (!m_wndOutput.Create (_T("Output"), this, CSize (150, 150),
		TRUE /* Has gripper */, ID_VIEW_OUTPUT,
		WS_CHILD | CBRS_BOTTOM))
	{
		TRACE0("Failed to create output bar\n");
		return -1;      // fail to create
	}

	CString strMainToolbarTitle;
	strMainToolbarTitle.LoadString (IDS_MAIN_TOOLBAR);
	m_wndToolBar.SetWindowText (strMainToolbarTitle);
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndWorkSpace.EnableDocking(CBRS_ALIGN_ANY);
	m_wndProperty.EnableDocking(CBRS_ALIGN_ANY);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_wndToolBar);
	DockControlBar(&m_wndWorkSpace);
	DockControlBarLeftOf(&m_wndProperty, &m_wndWorkSpace);
	DockControlBar(&m_wndOutput);


	// Allow user-defined toolbars operations:
	InitUserToobars (NULL,
					uiFirstUserToolBarId,
					uiLastUserToolBarId);

	// Enable windows manager:
	EnableWindowsDialog (ID_WINDOW_MANAGER, IDS_WINDOWS_MANAGER, TRUE);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnViewCustomize()
{
	//------------------------------------
	// Create a customize toolbars dialog:
	//------------------------------------
	CBCGToolbarCustomize* pDlgCust = new CBCGToolbarCustomize (this,
		TRUE /* Automatic menus scaning */
		);

	pDlgCust->EnableUserDefinedToolbars ();
	pDlgCust->Create ();
}

LRESULT CMainFrame::OnToolbarContextMenu(WPARAM,LPARAM lp)
{
	CPoint point (BCG_GET_X_LPARAM(lp), BCG_GET_Y_LPARAM(lp));

	CMenu menu;
	VERIFY(menu.LoadMenu (IDR_POPUP_TOOLBAR));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	SetupToolbarMenu (*pPopup, ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10);

	CBCGPopupMenu* pPopupMenu = new CBCGPopupMenu;
	pPopupMenu->Create (this, point.x, point.y, pPopup->Detach ());

	return 0;
}

BOOL CMainFrame::OnToolsViewUserToolbar (UINT uiId)
{
	CBCGToolBar* pUserToolBar = GetUserBarByIndex (uiId - ID_VIEW_USER_TOOLBAR1);
	if (pUserToolBar == NULL)
	{
		ASSERT (FALSE);
		return TRUE;
	}

	ShowControlBar (pUserToolBar, !(pUserToolBar->GetStyle () & WS_VISIBLE), FALSE);
	RecalcLayout();
	return TRUE;
}

void CMainFrame::OnUpdateToolsViewUserToolbar (CCmdUI* pCmdUI)
{
	CBCGToolBar* pUserToolBar = GetUserBarByIndex (pCmdUI->m_nID - ID_VIEW_USER_TOOLBAR1);
	if (pUserToolBar == NULL)
	{
		pCmdUI->Enable (FALSE);
		return;
	}

	pCmdUI->Enable ();
	pCmdUI->SetCheck (pUserToolBar->GetStyle () & WS_VISIBLE);
}

afx_msg LRESULT CMainFrame::OnToolbarReset(WPARAM /*wp*/,LPARAM)
{
	// TODO: reset toolbar with id = (UINT) wp to its initial state:
	//
	// UINT uiToolBarId = (UINT) wp;
	// if (uiToolBarId == IDR_MAINFRAME)
	// {
	//		do something with m_wndToolBar
	// }

	return 0;
}

BOOL CMainFrame::OnShowPopupMenu (CBCGPopupMenu* pMenuPopup)
{
	//---------------------------------------------------------
	// Replace ID_VIEW_TOOLBARS menu item to the toolbars list:
	//---------------------------------------------------------
    CMDIFrameWnd::OnShowPopupMenu (pMenuPopup);

    if (pMenuPopup != NULL &&
		pMenuPopup->GetMenuBar ()->CommandToIndex (ID_VIEW_TOOLBARS) >= 0)
    {
		if (CBCGToolBar::IsCustomizeMode ())
		{
			//----------------------------------------------------
			// Don't show toolbars list in the cuztomization mode!
			//----------------------------------------------------
			return FALSE;
		}

		pMenuPopup->RemoveAllItems ();

		CMenu menu;
		VERIFY(menu.LoadMenu (IDR_POPUP_TOOLBAR));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		SetupToolbarMenu (*pPopup, ID_VIEW_USER_TOOLBAR1, ID_VIEW_USER_TOOLBAR10);
		pMenuPopup->GetMenuBar ()->ImportFromMenu (*pPopup, TRUE);
    }

	return TRUE;
}

void CMainFrame::OnViewWorkspace() 
{
	ShowControlBar (&m_wndWorkSpace,
					!(m_wndWorkSpace.GetStyle () & WS_VISIBLE),
					FALSE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewWorkspace(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndWorkSpace.GetStyle () & WS_VISIBLE);
}

void CMainFrame::OnViewOutput() 
{
	ShowControlBar (&m_wndOutput,
					!(m_wndOutput.GetStyle () & WS_VISIBLE),
					FALSE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewOutput(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndOutput.GetStyle () & WS_VISIBLE);
}

void CMainFrame::OnWindowManager() 
{
	ShowWindowsDialog ();
}


//////////////////////////////////////////////////////////////////////////
void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	((CSpriteEditApp*)AfxGetApp())->m_Active = nState != WA_INACTIVE;
	
}


//////////////////////////////////////////////////////////////////////////
void CMainFrame::OnViewProperty() 
{
	ShowControlBar (&m_wndProperty, !(m_wndProperty.GetStyle () & WS_VISIBLE), FALSE);
	RecalcLayout ();
}


//////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateViewProperty(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_wndProperty.GetStyle () & WS_VISIBLE);
}


//////////////////////////////////////////////////////////////////////////
void CMainFrame::OnHelpDocs() 
{
	char full_path[MAX_PATH];
	GetModuleFileName(AfxGetInstanceHandle(), full_path, MAX_PATH);


	::HtmlHelp(NULL, GetPath(full_path) + "wme.chm", HH_DISPLAY_TOPIC, 0);
}
