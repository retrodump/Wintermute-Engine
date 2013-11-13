#if !defined(AFX_INSTALLDLG_H__454C6E3D_B86C_4581_8465_74F670F94839__INCLUDED_)
#define AFX_INSTALLDLG_H__454C6E3D_B86C_4581_8465_74F670F94839__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InstallDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInstallDlg dialog

#include "Package.h"

class CInstallDlg : public CDialog
{
// Construction
public:
	void AddFile(CString Filename);
	void ScanDir(CString BaseDir);
	CString m_GameDir;
	void SetItemIcon(int Item, int Icon);
	void ProcessMessages();
	bool InstFinish(int Index);
	bool InstCopy(int Index);
	bool InstPreCopy(int Index);
	bool InstPatch(int Index);
	bool InstInit(int Index);
	bool Install();
	CImageList* m_ImageList;
	CInstallDlg(CWnd* pParent = NULL);   // standard constructor
	CStringArray m_CopyFiles;
	CPackage m_Package;

	static BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam);
	CStringArray m_WMEWindowTitles;


// Dialog Data
	//{{AFX_DATA(CInstallDlg)
	enum { IDD = IDD_INSTALL };
	CProgressCtrl	m_Progress;
	CStatic	m_InfoText;
	CListCtrl	m_ActionsList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInstallDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInstallDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSTALLDLG_H__454C6E3D_B86C_4581_8465_74F670F94839__INCLUDED_)
