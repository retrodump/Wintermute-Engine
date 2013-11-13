// WorkspaceBar.h : interface of the CWorkspaceBar class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKSPACEBAR_H__6681408C_FE39_413C_860D_5BE74DC0B093__INCLUDED_)
#define AFX_WORKSPACEBAR_H__6681408C_FE39_413C_860D_5BE74DC0B093__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SpriteView.h"
#include "SpritePage.h"
#include "SpritePropPage.h"

class CWorkspaceBar : public CBCGSizingControlBar
{
public:
	CWorkspaceBar();
	void ActivateSpriteView (CSpriteView* SpriteView);

// Attributes
protected:
	CBCGTabWnd	m_wndTabs;

	CSpritePage	m_wndSprite;
	CSpritePropPage	m_wndSpriteProp;
	

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkspaceBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Refresh();
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

#endif // !defined(AFX_WORKSPACEBAR_H__6681408C_FE39_413C_860D_5BE74DC0B093__INCLUDED_)
