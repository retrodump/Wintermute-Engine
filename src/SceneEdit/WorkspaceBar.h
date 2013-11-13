// WorkspaceBar.h : interface of the CWorkspaceBar class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKSPACEBAR_H__712E4516_56C5_4CD5_B543_FCFCACBDF3A7__INCLUDED_)
#define AFX_WORKSPACEBAR_H__712E4516_56C5_4CD5_B543_FCFCACBDF3A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ScenePage.h"
#include "ScenePropPage.h"
#include "3DPage.h"

class CWorkspaceBar : public CBCGSizingControlBar
{
public:
	CWorkspaceBar();

// Attributes
protected:
	CBCGTabWnd	m_wndTabs;

	CScenePage m_wndScene;
	CScenePropPage m_wndSceneProp;
	C3DPage m_wnd3DProp;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkspaceBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Refresh(bool Full);
	void ActivateSceneView (CSceneView* SceneView);
	virtual ~CWorkspaceBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CWorkspaceBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKSPACEBAR_H__712E4516_56C5_4CD5_B543_FCFCACBDF3A7__INCLUDED_)
