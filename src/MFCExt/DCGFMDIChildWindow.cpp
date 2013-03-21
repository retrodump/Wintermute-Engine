// DCGFChildWindow.cpp : implementation file
//

#include "stdafx.h"
#include "DCGFMDIChildWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDCGFMDIChildWindow

IMPLEMENT_DYNCREATE(CDCGFMDIChildWindow, CMDIChildWnd)


//////////////////////////////////////////////////////////////////////////
CDCGFMDIChildWindow::CDCGFMDIChildWindow()
{
	m_View = NULL;	
}


//////////////////////////////////////////////////////////////////////////
CDCGFMDIChildWindow::~CDCGFMDIChildWindow()
{
	m_View = NULL; // ref only
}


BEGIN_MESSAGE_MAP(CDCGFMDIChildWindow, CMDIChildWnd)
	//{{AFX_MSG_MAP(CDCGFMDIChildWindow)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDCGFMDIChildWindow message handlers
