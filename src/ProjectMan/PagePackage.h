#if !defined(AFX_PAGEPACKAGE_H__17CE0DEA_830D_43B2_9B83_D0A5015364E4__INCLUDED_)
#define AFX_PAGEPACKAGE_H__17CE0DEA_830D_43B2_9B83_D0A5015364E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PagePackage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPagePackage dialog

class CPagePackage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPagePackage)

// Construction
public:
	void AddPackage(CString Name);
	CString m_ProjectRoot;
	void SetState();
	CPagePackage();
	~CPagePackage();
	CString m_SelectedPackage;
	CStringArray m_PackageNames;
	void OnStateChange();
	void OnBrowseIcon(CEdit& EditName);

// Dialog Data
	//{{AFX_DATA(CPagePackage)
	enum { IDD = IDD_PAGE_PACKAGE };
	CComboBox	m_Packages;
	CButton	m_ButtonBindPackage;
	CButton	m_ButtonBrowseIcon;
	CButton	m_ButtonBrowseIconSet;
	CEdit	m_EditIconName;
	CEdit	m_EditIconNameSet;
	CEdit	m_EditExeName;
	CEdit	m_EditSetName;
	CButton	m_CheckChangeIcon;
	CButton	m_CheckChangeIconSet;
	CButton	m_CheckCopyExe;
	CButton	m_CheckCopySet;
	CButton	m_CheckCopyD3DX;
	CEdit	m_EdOutput;
	CString	m_OutputDir;
	BOOL	m_CopyExe;
	BOOL	m_CopySet;
	BOOL	m_CopyD3DX;
	BOOL	m_ChangeIcon;
	BOOL	m_ChangeIconSet;
	CString	m_ExeName;
	CString	m_SetName;
	CString	m_IconName;
	CString	m_IconNameSet;
	BOOL	m_BindPackage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPagePackage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPagePackage)
	virtual BOOL OnInitDialog();	
	afx_msg void OnBrowseOutput();
	afx_msg void OnBrowseIcon();
	afx_msg void OnBrowseIconSet();
	afx_msg void OnBindPackage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL m_AddCrashLib;
	BOOL m_AddFpsDisplay;
	BOOL m_AddDebugMode;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEPACKAGE_H__17CE0DEA_830D_43B2_9B83_D0A5015364E4__INCLUDED_)
