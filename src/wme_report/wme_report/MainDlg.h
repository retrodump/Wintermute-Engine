#pragma once
#include "afxwin.h"
#include "StatLink.h"

// CMainDlg dialog

class CMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMainDlg();

// Dialog Data
	enum { IDD = IDD_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_ZipFile;
	CStatic m_Icon;
	virtual BOOL OnInitDialog();
	CStatic m_Description;
	CStaticLink m_ContentsLink;
	CStaticLink m_EmailLink;	
	afx_msg void OnContentsClick();
	afx_msg void OnMailClicked();
	bool UploadFile();
protected:
	virtual void OnOK();
};
