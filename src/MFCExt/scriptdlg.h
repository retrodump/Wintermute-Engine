#if !defined(AFX_SCRIPTDLG_H__0B717E2B_9724_403B_A8C3_1B6757436970__INCLUDED_)
#define AFX_SCRIPTDLG_H__0B717E2B_9724_403B_A8C3_1B6757436970__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScriptDlg.h : header file
//

#include "resource.h"
#include "DCGFApp.h"
#include "DCGFView.h"
#include "utils_mfc.h"
#include "BtnST.h"
#include "../engine_core/wme_base/dcgf.h"



/////////////////////////////////////////////////////////////////////////////
// CScriptDlg dialog

class CScriptDlg : public CDialog
{
// Construction
public:
	CScriptDlg(CWnd* pParent = NULL);   // standard constructor

	void SetState();
	void Redraw(int sel = -1);
	CBObject* m_Object;
	CStringArray m_ScriptNames;
	CDCGFView* m_View;

	
// Dialog Data
	//{{AFX_DATA(CScriptDlg)
	enum { IDD = IDD_SCRIPTS };
	CButtonST	m_BtnUp;
	CListBox	m_Scripts;
	CButtonST	m_BtnNew;
	CButtonST	m_BtnEdit;
	CButtonST	m_BtnDown;
	CButtonST	m_BtnDelete;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScriptDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScriptDlg)
	afx_msg void OnRemoveScript();
	afx_msg void OnAddScript();
	afx_msg void OnScriptDown();
	afx_msg void OnScriptUp();
	afx_msg void OnEditScript();
	afx_msg void OnSelchangeScripts();
	afx_msg void OnDblclkScripts();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRIPTDLG_H__0B717E2B_9724_403B_A8C3_1B6757436970__INCLUDED_)
