#if !defined(AFX_STATICBITMAP_H__DE001908_9EBF_454D_8BDA_8C8AED43FA8A__INCLUDED_)
#define AFX_STATICBITMAP_H__DE001908_9EBF_454D_8BDA_8C8AED43FA8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticBitmap.h : header file
//

#include "dib.h"

/////////////////////////////////////////////////////////////////////////////
// CStaticBitmap window

class CStaticBitmap : public CStatic
{
// Construction
public:
	CStaticBitmap();
	CDib m_dib;		// this is the bitmap

protected:
	virtual void PostNcDestroy();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_DYNAMIC(CStaticBitmap);


// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticBitmap)
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CStaticBitmap)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICBITMAP_H__DE001908_9EBF_454D_8BDA_8C8AED43FA8A__INCLUDED_)
