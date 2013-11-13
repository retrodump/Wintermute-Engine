#pragma once


// CCompatDlg dialog

class CCompatDlg : public CDialog
{
	DECLARE_DYNAMIC(CCompatDlg)

public:
	CCompatDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCompatDlg();

// Dialog Data
	enum { IDD = IDD_COMPAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_CompatKillMethodThreads;
	virtual BOOL OnInitDialog();
};
