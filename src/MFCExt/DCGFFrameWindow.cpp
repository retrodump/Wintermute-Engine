// DCGFFrameWindow.cpp : implementation file
//

#include "stdafx.h"
#include "DCGFFrameWindow.h"
#include "DCGFApp.h"
#include "DCGFStringTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDCGFFrameWindow

IMPLEMENT_DYNCREATE(CDCGFFrameWindow, CMDIFrameWnd)


//////////////////////////////////////////////////////////////////////////
CDCGFFrameWindow::CDCGFFrameWindow()
{
}


//////////////////////////////////////////////////////////////////////////
CDCGFFrameWindow::~CDCGFFrameWindow()
{
}


BEGIN_MESSAGE_MAP(CDCGFFrameWindow, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CDCGFFrameWindow)
	ON_WM_MEASUREITEM()
	ON_WM_MENUCHAR()
	ON_WM_INITMENUPOPUP()
	ON_WM_ACTIVATE()
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDCGFFrameWindow message handlers

//////////////////////////////////////////////////////////////////////////
HMENU CDCGFFrameWindow::NewMenu(UINT nMenuID, const UINT* pToolbarIDs, int nNumToolbars)
{
	m_Menu.LoadMenu(nMenuID);
	CDCGFStringTable::LocMenu(&m_Menu);
	m_Menu.LoadToolbars(pToolbarIDs, nNumToolbars);

	return(m_Menu.Detach());
}


//////////////////////////////////////////////////////////////////////////
HMENU CDCGFFrameWindow::NewDefaultMenu(UINT nMenuID, const UINT* pToolbarIDs, int nNumToolbars)
{
	m_DefaultMenu.LoadMenu(nMenuID);
	CDCGFStringTable::LocMenu(&m_DefaultMenu);
	m_DefaultMenu.LoadToolbars(pToolbarIDs, nNumToolbars);
	return(m_DefaultMenu.Detach());
}


//////////////////////////////////////////////////////////////////////////
void CDCGFFrameWindow::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	BOOL setflag=FALSE;
	if(lpMeasureItemStruct->CtlType==ODT_MENU){
		if(IsMenu((HMENU)lpMeasureItemStruct->itemID)){
			CMenu* cmenu = 
				CMenu::FromHandle((HMENU)lpMeasureItemStruct->itemID);
			
			if(m_Menu.IsMenu(cmenu)||m_DefaultMenu.IsMenu(cmenu)){
				m_Menu.MeasureItem(lpMeasureItemStruct);
				setflag=TRUE;
			}
		}
	}
	
	if(!setflag)CMDIFrameWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);	
}


//////////////////////////////////////////////////////////////////////////
LRESULT CDCGFFrameWindow::OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu) 
{
	LRESULT lresult;
	if(m_Menu.IsMenu(pMenu)||m_DefaultMenu.IsMenu(pMenu))
		lresult=BCMenu::FindKeyboardShortcut(nChar, nFlags, pMenu);
	else
		lresult=CMDIFrameWnd::OnMenuChar(nChar, nFlags, pMenu);
	return(lresult);
}


//////////////////////////////////////////////////////////////////////////
void CDCGFFrameWindow::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	CMDIFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	if(!bSysMenu){
		if(m_Menu.IsMenu(pPopupMenu)||m_DefaultMenu.IsMenu(pPopupMenu))
			BCMenu::UpdateMenu(pPopupMenu);
	}
}


//////////////////////////////////////////////////////////////////////////
void CDCGFFrameWindow::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	((CDCGFApp*)AfxGetApp())->m_Active = nState != WA_INACTIVE;	
}


//////////////////////////////////////////////////////////////////////////
void CDCGFFrameWindow::GetMessageString( UINT nID, CString& rMessage ) const
{
	CMDIFrameWnd::GetMessageString(nID, rMessage);

	rMessage = CString(LOC(rMessage));
}