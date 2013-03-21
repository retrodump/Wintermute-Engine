#if !defined(AFX_OPTIONSDLG_H__963372AD_AFE5_4A84_9DF8_D1DE9B6475CD__INCLUDED_)
#define AFX_OPTIONSDLG_H__963372AD_AFE5_4A84_9DF8_D1DE9B6475CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsDlg.h : header file
//

#include "dcgf.h"

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog

class COptionsDlg : public CDialog
{
// Construction
public:
	CBGame* Game;
	~COptionsDlg();
	COptionsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionsDlg)
	enum { IDD = IDD_OPTIONS };
	CBCGButton	m_OK;
	CBCGButton	m_Cancel;
	CBCGButton	m_EditIni;
	CButton	m_Accelerated;
	CButton	m_Force16bitTextures;
	CButton	m_ForceTextureSubdivision;
	CBCGEditListBox	m_CustomPaths;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnEditIni();
	afx_msg void OnAcceleration();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSDLG_H__963372AD_AFE5_4A84_9DF8_D1DE9B6475CD__INCLUDED_)
