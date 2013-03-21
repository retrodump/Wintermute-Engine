#if !defined(AFX_FORMVIEWEX_H__231DACD7_DE5B_48D6_ABE7_2110D7AF503C__INCLUDED_)
#define AFX_FORMVIEWEX_H__231DACD7_DE5B_48D6_ABE7_2110D7AF503C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormViewEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormViewEx form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "BCMenu.h"

class CFormViewEx : public CFormView
{
protected:
	CFormViewEx(UINT nIDTemplate);           // protected constructor used by dynamic creation
	//DECLARE_DYNCREATE(CFormViewEx)
	DECLARE_DYNAMIC(CFormViewEx)

// Form Data
public:
	//{{AFX_DATA(CFormViewEx)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	BCMenu m_PopupMenu;
// Operations
public:
	BOOL ShowPopupMenu(int MenuID, int ToolbarID=-1, LPRECT Rect=NULL);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormViewEx)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFormViewEx();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFormViewEx)
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMVIEWEX_H__231DACD7_DE5B_48D6_ABE7_2110D7AF503C__INCLUDED_)
