#if !defined(AFX_SCRIPTDLG_H__ACAA2B29_9D74_4060_B74F_36B8C918F83A__INCLUDED_)
#define AFX_SCRIPTDLG_H__ACAA2B29_9D74_4060_B74F_36B8C918F83A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScriptDlg.h : header file
//

#include "dcgf.h"
#include "SpriteView.h"


/////////////////////////////////////////////////////////////////////////////
// CScriptDlg dialog

class CScriptDlg : public CDialog
{
// Construction
public:
	void SetState();
	void Redraw(int sel = -1);
	CScriptDlg(CWnd* pParent = NULL);   // standard constructor
	CBScriptHolder* m_Object;
	CStringArray m_ScriptNames;
	CSpriteView* m_View;

// Dialog Data
	//{{AFX_DATA(CScriptDlg)
	enum { IDD = IDD_SCRIPTS };
	CBCGButton	m_EditScript;
	CListBox	m_Scripts;
	CBCGButton	m_ScriptUp;
	CBCGButton	m_ScriptDown;
	CBCGButton	m_RemoveScript;
	CBCGButton	m_AddScript;
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
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeScripts();
	afx_msg void OnAddScript();
	afx_msg void OnRemoveScript();
	afx_msg void OnScriptUp();
	afx_msg void OnScriptDown();
	afx_msg void OnEditScript();
	afx_msg void OnDblclkScripts();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRIPTDLG_H__ACAA2B29_9D74_4060_B74F_36B8C918F83A__INCLUDED_)
