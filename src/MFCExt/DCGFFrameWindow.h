#if !defined(AFX_DCGFFRAMEWINDOW_H__ACFEC0AD_D02B_4E60_9FB4_0E230BBE97C0__INCLUDED_)
#define AFX_DCGFFRAMEWINDOW_H__ACFEC0AD_D02B_4E60_9FB4_0E230BBE97C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DCGFFrameWindow.h : header file
//

#include "BCMenu.h"

/////////////////////////////////////////////////////////////////////////////
// CDCGFFrameWindow frame

class CDCGFFrameWindow : public CMDIFrameWnd
{
	DECLARE_DYNCREATE(CDCGFFrameWindow)
protected:
	CDCGFFrameWindow();           // protected constructor used by dynamic creation

// Attributes
public:
	HMENU NewMenu(UINT nMenuID, const UINT* pToolbarIDs, int nNumToolbars);
	HMENU NewDefaultMenu(UINT nMenuID, const UINT* pToolbarIDs, int nNumToolbars);
	BCMenu m_Menu,m_DefaultMenu;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDCGFFrameWindow)
	virtual void GetMessageString( UINT nID, CString& rMessage ) const;
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDCGFFrameWindow();

	// Generated message map functions
	//{{AFX_MSG(CDCGFFrameWindow)
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DCGFFRAMEWINDOW_H__ACFEC0AD_D02B_4E60_9FB4_0E230BBE97C0__INCLUDED_)
