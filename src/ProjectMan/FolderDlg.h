#if !defined(AFX_FOLDERDLG_H__7C90D629_38E0_4ADA_B259_808960174B59__INCLUDED_)
#define AFX_FOLDERDLG_H__7C90D629_38E0_4ADA_B259_808960174B59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FolderDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFolderDlg dialog

class CFolderDlg : public CDialog
{
// Construction
public:
	CString m_BasePath;
	CFolderDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFolderDlg)
	enum { IDD = IDD_NEW_FOLDER };
	CEdit	m_Name;
	CString	m_FolderName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFolderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFolderDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FOLDERDLG_H__7C90D629_38E0_4ADA_B259_808960174B59__INCLUDED_)
