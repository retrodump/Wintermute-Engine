#if !defined(AFX_EVENTPROPDLG_H__B32805D2_85AE_49E4_BCDC_FE1BBBB58E5E__INCLUDED_)
#define AFX_EVENTPROPDLG_H__B32805D2_85AE_49E4_BCDC_FE1BBBB58E5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EventPropDlg.h : header file
//

#include "dcgf_ad.h"

/////////////////////////////////////////////////////////////////////////////
// CEventPropDlg dialog

class CEventPropDlg : public CDialog
{
// Construction
public:
	CBGame* m_Game;
	~CEventPropDlg();
	CString m_ScriptFile;
	TEventType m_EventType;
	CEventPropDlg(CWnd* pParent = NULL);   // standard constructor
	CArray<TEventType, TEventType> m_Supported;

// Dialog Data
	//{{AFX_DATA(CEventPropDlg)
	enum { IDD = IDD_EVENT_PROP };
	CEdit	m_Script;
	CComboBox	m_Event;
	CButton	m_BrovseScript;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEventPropDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEventPropDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowseScript();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVENTPROPDLG_H__B32805D2_85AE_49E4_BCDC_FE1BBBB58E5E__INCLUDED_)
