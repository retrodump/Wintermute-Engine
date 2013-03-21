#if !defined(AFX_PROPERTYBAR_H__5EB6961B_69AF_44BA_98CE_FA1D1759EFE4__INCLUDED_)
#define AFX_PROPERTYBAR_H__5EB6961B_69AF_44BA_98CE_FA1D1759EFE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyBar.h : header file
//

#include "SpriteView.h"
#include "FramePage.h"
#include "EmptyPage.h"
#include "dcgf.h"

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
	void SetTitle(const char* Title);
	void ShowFrame(CSpriteView* View, CBFrame* Frame, CBSubFrame* Subframe, bool Force=false);
	void ShowNone();
	virtual ~CPropertyBar();
	CFramePage m_wndFrame;
	CEmptyPage m_wndEmpty;

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
