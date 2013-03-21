// WaypointsPage.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEdit.h"
#include "SceneView.h"
#include "WaypointsPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaypointsPage dialog


CWaypointsPage::CWaypointsPage(CWnd* pParent /*=NULL*/)
	: CDialog(CWaypointsPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWaypointsPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_WptGroup = NULL;
	m_View = NULL;
}


void CWaypointsPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaypointsPage)
	DDX_Control(pDX, IDC_NAME, m_Name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWaypointsPage, CDialog)
	//{{AFX_MSG_MAP(CWaypointsPage)
	ON_EN_CHANGE(IDC_NAME, OnChangeName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaypointsPage message handlers


//////////////////////////////////////////////////////////////////////////
void CWaypointsPage::OnCancel() 
{
	// do nothing
}


//////////////////////////////////////////////////////////////////////////
void CWaypointsPage::OnOK() 
{
	// do nothing
}


//////////////////////////////////////////////////////////////////////////
void CWaypointsPage::OnChangeName() 
{
	if(m_WptGroup){
		CString str;
		m_Name.GetWindowText(str);
		if(str.Compare(m_WptGroup->m_Name)!=0){
			m_View->SetUndoState("Change name");
			m_WptGroup->SetName((char*)LPCSTR(str));
			((CMainFrame*)AfxGetMainWnd())->m_wndWorkSpace.Refresh(false);
		}
	}	
}


//////////////////////////////////////////////////////////////////////////
void CWaypointsPage::ShowWaypoints(CAdWaypointGroup *Group, CSceneView* View, bool Force)
{
	if(Group==m_WptGroup && !Force) return;
	else{
		m_WptGroup = Group;
		m_View = View;
		if(m_WptGroup){
			m_Name.SetWindowText(m_WptGroup->m_Name);
		}
	}
}
