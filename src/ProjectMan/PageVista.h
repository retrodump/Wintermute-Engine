#pragma once
#include "afxwin.h"


// CPageVista dialog

class CPageVista : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageVista)

public:
	CPageVista();
	virtual ~CPageVista();
	CString m_ProjectRoot;

// Dialog Data
	enum { IDD = IDD_PAGE_VISTA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_CheckAddGeData;
	CEdit m_EdGdfFile;
	CEdit m_EdThumbnailFile;
	CButton m_BtnBrowseGdf;
	CButton m_BtnBrowseThumbnail;
	virtual BOOL OnInitDialog();
	void SetState();
	afx_msg void OnBnClickedBrowseGdf();
	afx_msg void OnBnClickedBrowseThumbnail();
	afx_msg void OnBnClickedGeData();
	BOOL m_AddGeData;
	CString m_GdfFile;
	CString m_ThumbnailFile;
};
