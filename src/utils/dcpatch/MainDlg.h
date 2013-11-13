// MainDlg.h : header file
//
//{{AFX_INCLUDES()
#include "browser.h"
//}}AFX_INCLUDES

#if !defined(AFX_MainDlg_H__9345D01F_5F04_43E0_9215_D0CC598F7D86__INCLUDED_)
#define AFX_MainDlg_H__9345D01F_5F04_43E0_9215_D0CC598F7D86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

#include "PalHook.h"
#include "StaticBitmap.h"
#include "CoolButton.h"

class CMainDlg : public CDialog
{
// Construction
public:
	CPoint m_DragFrom;
	bool m_Dragging;
	CMainDlg(CWnd* pParent = NULL);	// standard constructor
	CPalMsgHandler m_palMsgHandler;	// handles palette messages
	CStaticBitmap  m_BackBmp;		// one of these allowed

// Dialog Data
	//{{AFX_DATA(CMainDlg)
	enum { IDD = IDD_MAIN_DIALOG };
	CButton	m_InstButton;
	CBrowser	m_Readme;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnInstall();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual void OnOK();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTitleChangeReadme(LPCTSTR Text);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MainDlg_H__9345D01F_5F04_43E0_9215_D0CC598F7D86__INCLUDED_)
