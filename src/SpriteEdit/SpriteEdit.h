// SpriteEdit.h : main header file for the SPRITEEDIT application
//

#if !defined(AFX_SPRITEEDIT_H__1D201C6E_5AFF_4028_B661_551BAE7669FF__INCLUDED_)
#define AFX_SPRITEEDIT_H__1D201C6E_5AFF_4028_B661_551BAE7669FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


#ifdef _DEBUG
	#define SPRITEEDIT_REG_PATH "Software\\DEAD:CODE\\SpriteEdit\\Debug"
#else
	#define SPRITEEDIT_REG_PATH "Software\\DEAD:CODE\\SpriteEdit\\Retail"
#endif

#include "dcgf.h"

/////////////////////////////////////////////////////////////////////////////
// CSpriteEditApp:
// See SpriteEdit.cpp for the implementation of this class
//

class CSpriteEditApp : public CWinApp,
					  public CBCGWorkspace
{
public:
	CString m_LastDirSound;
	CString m_LastDirImage;
	CString m_LastDirScript;
	CSpriteEditApp();
	bool m_Active;
	bool EditFile(CString Filename, CBGame* Game, CWnd* Parent=NULL);

	void AddProjectRoot(CString Folder);
	CStringArray m_ProjectRoots;

	// Override from CBCGWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpriteEditApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSpriteEditApp)
	afx_msg void OnAppAbout();
	afx_msg void OnToolsOptions();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CSpriteEditApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPRITEEDIT_H__1D201C6E_5AFF_4028_B661_551BAE7669FF__INCLUDED_)

