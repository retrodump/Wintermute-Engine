#if !defined(AFX_PROPERTYBAR_H__5EB6961B_69AF_44BA_98CE_FA1D1759EFE4__INCLUDED_)
#define AFX_PROPERTYBAR_H__5EB6961B_69AF_44BA_98CE_FA1D1759EFE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyBar.h : header file
//

#include "SceneView.h"
#include "RegionPage.h"
#include "EntityPage.h"
#include "SoundPage.h"
#include "EmptyPage.h"
#include "WaypointsPage.h"
#include "dcgf_ad.h"

/////////////////////////////////////////////////////////////////////////////
// CPropertyBar window

class CPropertyBar : public CBCGSizingControlBar
{
// Construction
public:
	CPropertyBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	void ShowWaypoints(CAdWaypointGroup* Group, CSceneView* View, bool Force=false);
	void SetTitle(const char* Title);
	void ShowRegion(CAdRegion* Region, CSceneView* View, bool Force=false);
	void ShowEntity(CAdEntity* Entity, CSceneView* View, bool Force=false);
	void ShowNone();
	virtual ~CPropertyBar();
	CRegionPage m_wndRegion;
	CEntityPage m_wndEntity;
	CSoundPage m_wndSound;
	CEmptyPage m_wndEmpty;
	CWaypointsPage m_wndWaypoints;

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropertyBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYBAR_H__5EB6961B_69AF_44BA_98CE_FA1D1759EFE4__INCLUDED_)
