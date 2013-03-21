#if !defined(AFX_NEWPROPDLG_H__FFAF1AB6_56A2_4F08_BC52_63CCE61D509B__INCLUDED_)
#define AFX_NEWPROPDLG_H__FFAF1AB6_56A2_4F08_BC52_63CCE61D509B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewPropDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewPropDlg dialog

class CNewPropDlg : public CDialog
{
// Construction
public:
	void SetState();
	CNewPropDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewPropDlg)
	enum { IDD = IDD_NEW_PROP };
	CButton	m_OK;
	CEdit	m_PropNameEdit;
	CString	m_PropName;
	CString	m_PropValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewPropDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewPropDlg)
	afx_msg void OnChangePropName();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPROPDLG_H__FFAF1AB6_56A2_4F08_BC52_63CCE61D509B__INCLUDED_)
