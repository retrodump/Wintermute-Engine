#if !defined(AFX_EVENTSDLG_H__BE1748F2_0582_4C7E_9EFF_FD84595D1AA5__INCLUDED_)
#define AFX_EVENTSDLG_H__BE1748F2_0582_4C7E_9EFF_FD84595D1AA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EventsDlg.h : header file
//

#include "dcgf_ad.h"

/////////////////////////////////////////////////////////////////////////////
// CEventsDlg dialog

class CEventsDlg : public CDialog
{
// Construction
public:
	void AddSupportedType(TEventType Type);
	CBGame* m_Game;
	CBEvent* m_SelectedEvent;
	void SetState();
	void Redraw();
	CBEventSet* m_EventSet;
	CEventsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEventsDlg();
	CArray<TEventType, TEventType> m_Supported;

// Dialog Data
	//{{AFX_DATA(CEventsDlg)
	enum { IDD = IDD_EVENTS };
	CBCGButton	m_RemoveEvent;
	CListCtrl	m_Events;
	CBCGButton	m_EventUp;
	CBCGButton	m_EventProperties;
	CBCGButton	m_EventDown;
	CBCGButton	m_AddEvent;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEventsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEventsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEvent(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddEvent();
	afx_msg void OnEventUp();
	afx_msg void OnEventDown();
	afx_msg void OnEventProperties();
	afx_msg void OnRemoveEvent();
	afx_msg void OnDblclkEvents(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVENTSDLG_H__BE1748F2_0582_4C7E_9EFF_FD84595D1AA5__INCLUDED_)
