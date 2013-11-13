#if !defined(AFX_PROPSHEETEX_H__F1F7E4D6_FB24_4B28_898F_0C7ABC77C467__INCLUDED_)
#define AFX_PROPSHEETEX_H__F1F7E4D6_FB24_4B28_898F_0C7ABC77C467__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropSheetEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropSheetEx

class CPropSheetEx : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropSheetEx)

// Construction
public:
	CPropSheetEx(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropSheetEx(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropSheetEx)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropSheetEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropSheetEx)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPSHEETEX_H__F1F7E4D6_FB24_4B28_898F_0C7ABC77C467__INCLUDED_)
