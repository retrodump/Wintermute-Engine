// PropertyBar.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEdit.h"
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
	

	if (!m_wndEmpty.Create(IDD_EMPTY_PAGE, this) || !m_wndEntity.Create(IDD_ENTITY_PAGE, this)  || !m_wndSound.Create(IDD_SOUND_PAGE, this) || !m_wndRegion.Create(IDD_REGION_PAGE, this) || !m_wndWaypoints.Create(IDD_WAYPOINTS_PAGE, this))
	{
		TRACE0("Failed to create region page\n");
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
	m_wndEntity.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndSound.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndRegion.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndWaypoints.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}


//////////////////////////////////////////////////////////////////////////
void CPropertyBar::ShowNone()
{
	m_wndEmpty.ShowWindow(SW_SHOW);
	m_wndEntity.ShowWindow(SW_HIDE);
	m_wndSound.ShowWindow(SW_HIDE);
	m_wndRegion.ShowWindow(SW_HIDE);
	m_wndWaypoints.ShowWindow(SW_HIDE);

	SetTitle("Properties");
}


//////////////////////////////////////////////////////////////////////////
void CPropertyBar::ShowEntity(CAdEntity *Entity, CSceneView* View, bool Force)
{
	m_wndEmpty.ShowWindow(SW_HIDE);
	
	if(Entity->m_Subtype==ENTITY_SOUND){
		m_wndEntity.ShowWindow(SW_HIDE);
		m_wndSound.ShowWindow(SW_SHOW);
	}
	else {
		m_wndEntity.ShowWindow(SW_SHOW);
		m_wndSound.ShowWindow(SW_HIDE);
	}

	m_wndRegion.ShowWindow(SW_HIDE);
	m_wndWaypoints.ShowWindow(SW_HIDE);

	if(Entity->m_Subtype==ENTITY_SOUND){
		SetTitle("Sound properties");
		m_wndSound.ShowEntity(Entity, View, Force);
	}
	else{
		SetTitle("Entity properties");
		m_wndEntity.ShowEntity(Entity, View, Force);
	}
}


//////////////////////////////////////////////////////////////////////////
void CPropertyBar::ShowRegion(CAdRegion *Region, CSceneView* View, bool Force)
{
	m_wndEmpty.ShowWindow(SW_HIDE);
	m_wndEntity.ShowWindow(SW_HIDE);
	m_wndSound.ShowWindow(SW_HIDE);
	m_wndRegion.ShowWindow(SW_SHOW);
	m_wndWaypoints.ShowWindow(SW_HIDE);

	SetTitle("Region properties");
	m_wndRegion.ShowRegion(Region, View, Force);
}


//////////////////////////////////////////////////////////////////////////
void CPropertyBar::ShowWaypoints(CAdWaypointGroup *Group, CSceneView* View, bool Force)
{
	m_wndEmpty.ShowWindow(SW_HIDE);
	m_wndEntity.ShowWindow(SW_HIDE);
	m_wndSound.ShowWindow(SW_HIDE);
	m_wndRegion.ShowWindow(SW_HIDE);
	m_wndWaypoints.ShowWindow(SW_SHOW);

	SetTitle("Waypoint group properties");
	m_wndWaypoints.ShowWaypoints(Group, View, Force);
}



//////////////////////////////////////////////////////////////////////////
void CPropertyBar::SetTitle(const char *Title)
{
	CString str;
	GetWindowText(str);
	if(str.Compare(Title)!=0) SetWindowText(Title);
}
