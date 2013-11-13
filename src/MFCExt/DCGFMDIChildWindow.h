#if !defined(AFX_DCGFCHILDWINDOW_H__EE51AA5D_65C2_49EB_A649_FDAC0B8BFEBE__INCLUDED_)
#define AFX_DCGFCHILDWINDOW_H__EE51AA5D_65C2_49EB_A649_FDAC0B8BFEBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DCGFChildWindow.h : header file
//

#include "DCGFView.h"

/////////////////////////////////////////////////////////////////////////////
// CDCGFMDIChildWindow frame

class CDCGFMDIChildWindow : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CDCGFMDIChildWindow)
protected:
	CDCGFMDIChildWindow();           // protected constructor used by dynamic creation

// Attributes
public:
	CDCGFView* m_View;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDCGFMDIChildWindow)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDCGFMDIChildWindow();

	// Generated message map functions
	//{{AFX_MSG(CDCGFMDIChildWindow)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DCGFCHILDWINDOW_H__EE51AA5D_65C2_49EB_A649_FDAC0B8BFEBE__INCLUDED_)
