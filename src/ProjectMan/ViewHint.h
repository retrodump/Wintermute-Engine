#if !defined(AFX_VIEWHINT_H__621C5849_E301_459A_9C7B_076AFCC506CC__INCLUDED_)
#define AFX_VIEWHINT_H__621C5849_E301_459A_9C7B_076AFCC506CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewHint.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewHint html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

#include "ProjectView.h"

class CViewHint : public CHtmlView
{
protected:
	CViewHint();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewHint)

// html Data
public:
	//{{AFX_DATA(CViewHint)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CString m_AppPath;
	CProjectView* m_View;
	CString m_Filename;
	CString m_LastFilename;

// Operations
public:
	void DisplayHint(CString Filename);
	static UINT LoaderThread(LPVOID Param);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewHint)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewHint();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewHint)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWHINT_H__621C5849_E301_459A_9C7B_076AFCC506CC__INCLUDED_)
