#if !defined(AFX_VIEWLOG_H__6A043845_696F_4B10_8D7D_652C371530CC__INCLUDED_)
#define AFX_VIEWLOG_H__6A043845_696F_4B10_8D7D_652C371530CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewLog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewLog form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "../MFCExt/FormViewEx.h"
#include "ProjectView.h"

class CViewLog : public CFormViewEx
{
protected:
	CViewLog();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewLog)

// Form Data
public:
	//{{AFX_DATA(CViewLog)
	enum { IDD = IDD_LOG };
	CListCtrl	m_List;
	//}}AFX_DATA

// Attributes
public:
	CProjectView* m_View;
	CImageList m_ImageList;
	
// Operations
public:
	void AddError(CString Text, CString Filename="");
	void AddWarning(CString Text, CString Filename="");
	void AddInfo(CString Text, CString Filename="");
	void AddLog(CString Text, CString Filename="");

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewLog)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewLog();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewLog)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWLOG_H__6A043845_696F_4B10_8D7D_652C371530CC__INCLUDED_)
