#if !defined(AFX_PAGEGENERAL_H__C328BC83_5BA2_4FAA_B4BD_2D64182CDB92__INCLUDED_)
#define AFX_PAGEGENERAL_H__C328BC83_5BA2_4FAA_B4BD_2D64182CDB92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// pagegeneral.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageGeneral dialog

#include "resource.h"

class CPageGeneral : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageGeneral)

// Construction
public:
	void AddRepositoryPath(CString Path, bool Select=false);
	CPageGeneral();
	~CPageGeneral();

// Dialog Data
	//{{AFX_DATA(CPageGeneral)
	enum { IDD = IDD_PAGE_GENERAL_SETTINGS };
	CButton	m_CheckUpdates;
	CButton	m_ShowHiddenFiles;
	CButton	m_SettingsDlg;
	CButton	m_TextureSubdivision;
	CButton	m_16bitTextures;
	CEdit	m_WMEPath;
	CListBox	m_Repositories;
	CButton	m_Accleration;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageGeneral)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageGeneral)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddRepository();
	afx_msg void OnRemoveRepository();
	afx_msg void OnBrowseTools();
	afx_msg void OnAccleration();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEGENERAL_H__C328BC83_5BA2_4FAA_B4BD_2D64182CDB92__INCLUDED_)
