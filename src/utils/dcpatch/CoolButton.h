#if !defined(AFX_COOLBUTTON_H__9A066F16_608A_47F8_940E_EE8AE6212D76__INCLUDED_)
#define AFX_COOLBUTTON_H__9A066F16_608A_47F8_940E_EE8AE6212D76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CoolButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCoolButton window

#include "dib.h"

class CCoolButton : public CButton
{
	DECLARE_DYNAMIC(CCoolButton)
// Construction
public:
	CCoolButton();
	CDib m_BitmapN;
	CDib m_BitmapP;
	CDib m_BitmapH;

protected:
	BOOL			m_bPushed;
	BOOL			m_bClickiedInside;
	BOOL			m_bHighlighted;
	BOOL			m_bCaptured;
	BOOL			m_bHover;
	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoolButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetBitmaps(int Normal, int Pushed, int Hover, int X=0, int Y=0);
	virtual ~CCoolButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCoolButton)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnEnable(BOOL bEnable);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLBUTTON_H__9A066F16_608A_47F8_940E_EE8AE6212D76__INCLUDED_)
