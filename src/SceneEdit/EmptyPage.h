#if !defined(AFX_EMPTYPAGE_H__956E1AFF_6014_4588_9055_961FBDF09DAA__INCLUDED_)
#define AFX_EMPTYPAGE_H__956E1AFF_6014_4588_9055_961FBDF09DAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EmptyPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEmptyPage dialog

class CEmptyPage : public CDialog
{
// Construction
public:
	CEmptyPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEmptyPage)
	enum { IDD = IDD_EMPTY_PAGE };
	CStatic	m_EmptyNote;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEmptyPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEmptyPage)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMPTYPAGE_H__956E1AFF_6014_4588_9055_961FBDF09DAA__INCLUDED_)
