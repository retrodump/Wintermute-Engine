// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__51602F4F_DE81_4C19_B760_63B2B1352470__INCLUDED_)
#define AFX_MAINFRM_H__51602F4F_DE81_4C19_B760_63B2B1352470__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../MFCExt/DCGFFrameWindow.h"

class CMainFrame : public CDCGFFrameWindow
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
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL OnOpenHelpURL(UINT nID);
	HRESULT UpdateMenuURL(CMenu* Menu);
	CStringArray m_ListURL;
	HRESULT CheckForUpdates(bool Silent=true);
	HRESULT GetWMEVersionHttp(CString& WMEVersion);
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnToolsOptions();
	afx_msg void OnHelpDocs();
	afx_msg void OnHelpUpdates();
	afx_msg void OnHelpDonate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__51602F4F_DE81_4C19_B760_63B2B1352470__INCLUDED_)
