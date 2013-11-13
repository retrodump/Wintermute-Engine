// DCGFApp.cpp : implementation file
//

#include "stdafx.h"
#include "DCGFApp.h"
#include "DCGFMDIChildWindow.h"
#include "DCGFView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDCGFApp

IMPLEMENT_DYNCREATE(CDCGFApp, CWinApp)


//////////////////////////////////////////////////////////////////////////
CDCGFApp::CDCGFApp()
{
	m_Active = false;
}


//////////////////////////////////////////////////////////////////////////
CDCGFApp::~CDCGFApp()
{
}


//////////////////////////////////////////////////////////////////////////
BOOL CDCGFApp::InitInstance()
{
	char CurrentDir[MAX_PATH+1];
	GetCurrentDirectory(MAX_PATH, CurrentDir);
	AddProjectRoot(CString(CurrentDir));

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
int CDCGFApp::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinApp::ExitInstance();
}


BEGIN_MESSAGE_MAP(CDCGFApp, CWinApp)
	//{{AFX_MSG_MAP(CDCGFApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDCGFApp message handlers


//////////////////////////////////////////////////////////////////////////
BOOL CDCGFApp::OnIdle(LONG lCount)
{
	if(m_Active){
		CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
		if(pFrame==NULL) return FALSE;

		// Get the active MDI child window.
		CDCGFMDIChildWindow* pChild = (CDCGFMDIChildWindow*)pFrame->GetActiveFrame();
		if(pChild==NULL || !pChild->IsKindOf(RUNTIME_CLASS(CDCGFMDIChildWindow))) return FALSE;

		CView *pView = pChild->m_View;
		if(pView==NULL || !pView->GetSafeHwnd()) return FALSE;
		if(pView->IsKindOf(RUNTIME_CLASS(CDCGFView))) pView->Invalidate();	

		CWinApp::OnIdle(lCount);
		return TRUE;
	}
	return CWinApp::OnIdle(lCount);
}


//////////////////////////////////////////////////////////////////////////
void CDCGFApp::AddProjectRoot(CString Folder)
{
	if(Folder=="") return;

	Folder.Replace('/', '\\');

	if(Folder[Folder.GetLength()-1]!='\\') Folder+='\\';

	for(int i=0; i<m_ProjectRoots.GetSize(); i++){
		if(m_ProjectRoots[i]==Folder) return;
	}
	m_ProjectRoots.Add(Folder);
}
