// ViewHint.cpp : implementation file
//

#include "stdafx.h"
#include "ProjectMan.h"
#include "ViewHint.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewHint

IMPLEMENT_DYNCREATE(CViewHint, CHtmlView)


//////////////////////////////////////////////////////////////////////////
CViewHint::CViewHint()
{
	//{{AFX_DATA_INIT(CViewHint)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_View = NULL;
	m_Filename = m_LastFilename = "";
}


//////////////////////////////////////////////////////////////////////////
CViewHint::~CViewHint()
{
	m_View = NULL;
}


//////////////////////////////////////////////////////////////////////////
void CViewHint::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewHint)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewHint, CHtmlView)
	//{{AFX_MSG_MAP(CViewHint)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewHint diagnostics

#ifdef _DEBUG
void CViewHint::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CViewHint::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewHint message handlers

//////////////////////////////////////////////////////////////////////////
void CViewHint::OnInitialUpdate() 
{
	CHtmlView::OnInitialUpdate();

	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char full_path[MAX_PATH];

	GetModuleFileName(AfxGetInstanceHandle(), full_path, MAX_PATH);
	_splitpath(full_path, drive, dir, NULL, NULL);
	m_AppPath.Format("%s%s", drive, dir);
	if(m_AppPath=="" || m_AppPath[m_AppPath.GetLength()-1]!='\\')
		m_AppPath += "\\";

}


//////////////////////////////////////////////////////////////////////////
BOOL CViewHint::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if(m_View && m_View->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)) return TRUE;
	else return CHtmlView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


//////////////////////////////////////////////////////////////////////////
void CViewHint::DisplayHint(CString Filename)
{
	if(m_Filename==""){
		m_Filename = m_AppPath + "help\\" + CString(AfxGetAppName()) + "\\" + Filename;
		CFileFind finder;
		if(!finder.FindFile(m_Filename)){
			m_Filename = m_AppPath + CString("help\\") + CString(AfxGetAppName()) + "\\generic.html";
		}

		if(m_Filename != m_LastFilename)
			AfxBeginThread(LoaderThread, this, THREAD_PRIORITY_BELOW_NORMAL);
		else
			m_Filename = "";
	}
}


//////////////////////////////////////////////////////////////////////////
UINT CViewHint::LoaderThread(LPVOID Param)
{
	CViewHint* _this = (CViewHint*)Param;
	if(_this == NULL ||!_this->IsKindOf(RUNTIME_CLASS(CViewHint))){
		return 1;
	}
	
	_this->Navigate(_this->m_Filename, NULL, NULL, NULL, NULL);
	_this->m_LastFilename = _this->m_Filename;
	_this->m_Filename = "";
	return 0;
}