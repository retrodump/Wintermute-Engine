#pragma once
#include "afxwin.h"
#include "../engine_core/wme_base/dcgf.h"

// CSaveLoadDlg dialog

class CSaveLoadDlg : public CDialog
{
	DECLARE_DYNAMIC(CSaveLoadDlg)

public:
	CSaveLoadDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSaveLoadDlg();

// Dialog Data
	enum { IDD = IDD_SAVELOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_LoadX;
	int m_LoadY;
	int m_SaveX;
	int m_SaveY;
	CString m_LoadFile;
	CString m_SaveFile;
	afx_msg void OnBnClickedLoadBrowse();
	afx_msg void OnBnClickedSaveBrowse();
	CEdit m_LoadFileControl;
	CEdit m_SaveFileControl;
	CBGame* Game;

};
