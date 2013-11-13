#if !defined(AFX_BGDLG_H__61E5479C_E104_49E8_9E06_1BFEFBB1BD9C__INCLUDED_)
#define AFX_BGDLG_H__61E5479C_E104_49E8_9E06_1BFEFBB1BD9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BgDlg.h : header file
//

#include "../mfc_shared/amsEdit.h"
#include "dcgf.h"

/////////////////////////////////////////////////////////////////////////////
// CBgDlg dialog

class CBgDlg : public CDialog
{
// Construction
public:
	CBgDlg(CWnd* pParent = NULL);   // standard constructor
	CBGame* m_Game;

// Dialog Data
	//{{AFX_DATA(CBgDlg)
	enum { IDD = IDD_BACKGROUND };
	CAMSIntegerEdit	m_EdOffsetY;
	CAMSIntegerEdit	m_EdOffsetX;
	CEdit	m_EdFilename;
	CEdit	m_EdAlpha;
	CSpinButtonCtrl	m_AlphaSpin;
	int		m_Alpha;
	CString	m_Filename;
	int		m_OffsetX;
	int		m_OffsetY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBgDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBrowse();
	afx_msg void OnNoImage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BGDLG_H__61E5479C_E104_49E8_9E06_1BFEFBB1BD9C__INCLUDED_)
