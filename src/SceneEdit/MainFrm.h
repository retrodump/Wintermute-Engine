// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__9B485CAD_F425_47C8_A153_BF014FF00F6B__INCLUDED_)
#define AFX_MAINFRM_H__9B485CAD_F425_47C8_A153_BF014FF00F6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "WorkspaceBar.h"
#include "OutputBar.h"
#include "PropertyBar.h"

#define CMDIFrameWnd CBCGMDIFrameWnd

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CWorkspaceBar	m_wndWorkSpace;
	CPropertyBar	m_wndProperty;
	CBCGStatusBar	m_wndStatusBar;

protected:  // control bar embedded members
	CBCGMenuBar		m_wndMenuBar;	
	CBCGToolBar		m_wndToolBar;	
	COutputBar	m_wndOutput;

	CBCGToolBarImages	m_UserImages;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnViewProperty();
	afx_msg void OnUpdateViewProperty(CCmdUI* pCmdUI);
	afx_msg void OnHelpDocs();
	//}}AFX_MSG
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	BOOL OnToolsViewUserToolbar (UINT id);
	void OnUpdateToolsViewUserToolbar (CCmdUI* pCmdUI);
	afx_msg LRESULT OnToolbarContextMenu(WPARAM,LPARAM);
	afx_msg void OnViewWorkspace();
	afx_msg void OnUpdateViewWorkspace(CCmdUI* pCmdUI);
	afx_msg void OnViewOutput();
	afx_msg void OnUpdateViewOutput(CCmdUI* pCmdUI);
	afx_msg void OnWindowManager();
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnShowPopupMenu (CBCGPopupMenu* pMenuPopup);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__9B485CAD_F425_47C8_A153_BF014FF00F6B__INCLUDED_)
