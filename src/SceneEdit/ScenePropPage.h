#if !defined(AFX_SCENEPROPPAGE_H__72B54108_1923_442C_AE3E_A6CAC5C72893__INCLUDED_)
#define AFX_SCENEPROPPAGE_H__72B54108_1923_442C_AE3E_A6CAC5C72893__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScenePropPage.h : header file
//

#include "SceneView.h"

/////////////////////////////////////////////////////////////////////////////
// CScenePropPage dialog

class CScenePropPage : public CDialog
{
// Construction
public:
	CFont m_BoldFont;
	void SetState();
	void Redraw();
	void SetMargins();
	CScenePropPage(CWnd* pParent = NULL);   // standard constructor
	void SetCurrentView(CSceneView *View);
	CSceneView* m_View;
	void OnCancel();
	void OnOK();

// Dialog Data
	//{{AFX_DATA(CScenePropPage)
	enum { IDD = IDD_SCENE_PROP_PAGE };
	CButton	m_Persistent;
	CButton	m_PersistentSprites;
	CButton	m_ShowDecor;
	CBCGColorButton	m_DecorSel;
	CBCGColorButton	m_Decor;
	CBCGButton	m_Custom;
	CButton	m_ShowScale;
	CButton	m_ShowRegions;
	CButton	m_ShowEntities;
	CButton	m_ShowBlocked;
	CBCGColorButton	m_BlockedSel;
	CBCGColorButton	m_Blocked;
	CEdit	m_MarginV;
	CEdit	m_MarginH;
	CEdit	m_Name;
	CBCGButton	m_Events;
	CBCGColorButton	m_WaypointsSel;
	CBCGColorButton	m_Waypoints;
	CBCGColorButton	m_RegionSel;
	CBCGColorButton	m_Region;
	CBCGColorButton	m_EntitySel;
	CBCGColorButton	m_Entity;
	CBCGColorButton	m_Frame;
	CBCGColorButton	m_Scale;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScenePropPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScenePropPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeName();
	afx_msg void OnKillfocusMarginH();
	afx_msg void OnKillfocusMarginV();
	afx_msg void OnColBlocked();
	afx_msg void OnColBlockedSel();
	afx_msg void OnColEntity();
	afx_msg void OnColEntitySel();
	afx_msg void OnColFrame();
	afx_msg void OnColRegion();
	afx_msg void OnColRegionSel();
	afx_msg void OnColWaypoints();
	afx_msg void OnColWaypointsSel();
	afx_msg void OnEvents();
	afx_msg void OnColScale();
	afx_msg void OnShowRegions();
	afx_msg void OnShowBlocked();
	afx_msg void OnShowEntities();
	afx_msg void OnShowScale();
	afx_msg void OnCustom();
	afx_msg void OnShowDecor();
	afx_msg void OnColDecor();
	afx_msg void OnColDecorSel();
	afx_msg void OnPersistent();
	afx_msg void OnPersistentSprites();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCENEPROPPAGE_H__72B54108_1923_442C_AE3E_A6CAC5C72893__INCLUDED_)
