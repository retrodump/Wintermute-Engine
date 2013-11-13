#pragma once


// CPageFilters dialog

class CPageFilters : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageFilters)

public:
	CPageFilters();
	virtual ~CPageFilters();

// Dialog Data
	enum { IDD = IDD_PAGE_FILTERS };
	CEdit	m_EdMasksUncompress;
	CEdit	m_EdMasksScript;
	CEdit	m_EdMasksExclude;
	CEdit	m_EdMasksCopy;
	CButton	m_CheckUncompress;
	CButton	m_CheckScript;
	CButton	m_CheckExclude;
	CButton	m_CheckCopy;
	BOOL	m_ActiveExclude;
	BOOL	m_ActiveScript;
	BOOL	m_ActiveUncompress;
	BOOL	m_ActiveCopy;
	CString	m_MasksExclude;
	CString	m_MasksScript;
	CString	m_MasksUncompress;
	CString	m_MasksCopy;

	void SetState();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	// Generated message map functions
	//{{AFX_MSG(CPagePackage)
	afx_msg void OnFilterActivate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
