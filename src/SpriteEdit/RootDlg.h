#if !defined(AFX_ROOTDLG_H__C3ABB47A_7BE0_4F97_87F3_E9C78C595084__INCLUDED_)
#define AFX_ROOTDLG_H__C3ABB47A_7BE0_4F97_87F3_E9C78C595084__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RootDlg.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CRootDlg dialog

class CRootDlg : public CDialog
{
// Construction
public:
	void AddPrevFolder(CString Folder);
	void SetState();
	CString m_SelectedFolder;
	CString m_Filename;
	CRootDlg(CWnd* pParent = NULL);   // standard constructor
	CStringArray m_PrevFolders;

// Dialog Data
	//{{AFX_DATA(CRootDlg)
	enum { IDD = IDD_PROJECT_ROOT };
	CButton	m_BtnOK;
	CButton	m_BtnBrowse;
	CButton	m_BtnPrev;
	CButton	m_BtnCustom;
	CListBox	m_PrevList;
	CEdit	m_CustomName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRootDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRootDlg)
	afx_msg void OnBrowse();
	afx_msg void OnCustom();
	afx_msg void OnPrevious();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeCustomName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROOTDLG_H__C3ABB47A_7BE0_4F97_87F3_E9C78C595084__INCLUDED_)
