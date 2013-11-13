// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "ProjectMan.h"
#include "ProjectView.h"

#include "ViewHint.h"
#include "ViewLog.h"
#include "ViewProps.h"
#include "ViewTree.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CDCGFMDIChildWindow)

BEGIN_MESSAGE_MAP(CChildFrame, CDCGFMDIChildWindow)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction


//////////////////////////////////////////////////////////////////////////
CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	
}


//////////////////////////////////////////////////////////////////////////
CChildFrame::~CChildFrame()
{
}


//////////////////////////////////////////////////////////////////////////
BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CDCGFMDIChildWindow::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_CHILD /*| WS_VISIBLE*/ | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
		| FWS_ADDTOTITLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
void CChildFrame::ActivateFrame(int nCmdShow)
{
	// TODO: Modify this function to change how the frame is activated.

	nCmdShow = SW_SHOWMAXIMIZED;
	CDCGFMDIChildWindow::ActivateFrame(nCmdShow);

	m_Framework.LoadSettings();
}


/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CDCGFMDIChildWindow::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CDCGFMDIChildWindow::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers


//////////////////////////////////////////////////////////////////////////
BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	TVisualObject *SplitMain = new TVisualObject(1,"MainSplitter",1,3,pContext);
	SplitMain->SetDefaultSize(0, 0, CSize(20,-1));
	SplitMain->SetDefaultSize(0, 1, CSize(20,-1));

	// right side
	TVisualObject *SplitRight = new TVisualObject(2, 0,2, 2,1,pContext);
	SplitRight->SetDefaultSize(0, 0, CSize(-1,80));
	
		TVisualObject *ViewProject = new TVisualObject(3,0,0,pContext,RUNTIME_CLASS(CProjectView), CSize(0,100));
		
		TVisualObject *TabLog = new TVisualObject(4,1,0,pContext,RUNTIME_CLASS(TTabWnd), CSize(0,0));
			TVisualObject *ViewHint = new TVisualObject(5, LOC("/str0105/Help"),pContext,RUNTIME_CLASS(CViewHint));
			TVisualObject *ViewLog = new TVisualObject(6, LOC("/str0106/Log"),pContext,RUNTIME_CLASS(CViewLog));
		
		
	TVisualObject *ViewProps = new TVisualObject(7,0,0,pContext,RUNTIME_CLASS(CViewProps), CSize(0,100));
	TVisualObject *ViewTree = new TVisualObject(8,0,1,pContext,RUNTIME_CLASS(CViewTree), CSize(0,100));


	m_Framework.Add(SplitMain);

		m_Framework.Add(SplitMain, SplitRight);
			m_Framework.Add(SplitRight,ViewProject);
			m_Framework.Add(SplitRight,TabLog);
				m_Framework.Add(TabLog,ViewHint);
				m_Framework.Add(TabLog,ViewLog);
	
	
		m_Framework.Add(SplitMain, ViewProps);
		m_Framework.Add(SplitMain, ViewTree);

	BOOL ret = m_Framework.Create(this);
		
	// get main view
	TVisualFrameworkIterator iterator(m_Framework);
	while (!iterator.End()) {
		TVisualObject *pObject = iterator.Get();
		if(pObject->GetWnd() && pObject->GetWnd()->IsKindOf(RUNTIME_CLASS(CProjectView))){
			m_View = (CProjectView*)pObject->GetWnd();
			break;
		}
		iterator ++;
	}

	// get view panes
	if(m_View){
		while (!iterator.End()) {
			TVisualObject *pObject = iterator.Get();
			if(!pObject || !pObject->GetSafeWnd()) continue;

			if(pObject->GetWnd()->IsKindOf(RUNTIME_CLASS(CViewHint)))
				((CProjectView*)m_View)->m_ViewHint = (CViewHint*)pObject->GetWnd();

			else if(pObject->GetWnd()->IsKindOf(RUNTIME_CLASS(CViewLog)))
				((CProjectView*)m_View)->m_ViewLog = (CViewLog*)pObject->GetWnd();

			else if(pObject->GetWnd()->IsKindOf(RUNTIME_CLASS(CViewProps)))
				((CProjectView*)m_View)->m_ViewProps = (CViewProps*)pObject->GetWnd();

			else if(pObject->GetWnd()->IsKindOf(RUNTIME_CLASS(CViewTree)))
				((CProjectView*)m_View)->m_ViewTree = (CViewTree*)pObject->GetWnd();

			iterator ++;
		}
	}

	return ret;
}


//////////////////////////////////////////////////////////////////////////
void CChildFrame::OnDestroy() 
{
	CDCGFMDIChildWindow::OnDestroy();
	

	if(m_View){
		CProjectView* View = (CProjectView*)m_View;

		View->m_ViewHint->m_View = NULL;
		View->m_ViewLog->m_View = NULL;
		View->m_ViewProps->m_View = NULL;
		View->m_ViewTree->m_View = NULL;

		View->m_ViewHint = NULL;
		View->m_ViewLog = NULL;
		View->m_ViewProps = NULL;
		View->m_ViewTree = NULL;
	}

	if(m_View && m_View->GetSafeHwnd()){
		m_Framework.SaveSettings();
	}


	GetParentFrame()->SetActiveView(NULL);
	SetActiveView(NULL);
	m_Framework.Destroy();
}


//////////////////////////////////////////////////////////////////////////
void CChildFrame::OnClose() 
{
	if(m_View && m_View->GetSafeHwnd()){
		m_Framework.SaveSettings();
	}
	
	CDCGFMDIChildWindow::OnClose();
}
