// SceneEdit.h : main header file for the SCENEEDIT application
//

#if !defined(AFX_SCENEEDIT_H__73BD41A5_0205_405C_AE8E_679F61C115B8__INCLUDED_)
#define AFX_SCENEEDIT_H__73BD41A5_0205_405C_AE8E_679F61C115B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "dcgf_ad.h"

#ifdef _DEBUG
	#define SCENEEDIT_REG_PATH "Software\\DEAD:CODE\\SceneEdit\\Debug"
#else
	#define SCENEEDIT_REG_PATH "Software\\DEAD:CODE\\SceneEdit\\Retail"
#endif


/////////////////////////////////////////////////////////////////////////////
// CSceneEditApp:
// See SceneEdit.cpp for the implementation of this class
//

class CSceneEditApp : public CWinApp,
					  public CBCGWorkspace
{
public:
	bool EditFile(CString Filename, CBGame* Game, CWnd* Parent=NULL);
	CString m_LastDirActor;
	CString m_LastDirSound;
	CString m_LastDirScript;
	CString m_LastDirCursor;
	CString m_LastDirFont;
	CString m_LastDirSprite;
	CString m_LastDirGeom;

	bool m_Active;
	CSceneEditApp();

	void AddProjectRoot(CString Folder);
	CStringArray m_ProjectRoots;

	// Override from CBCGWorkspace
	virtual void PreLoadState ();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSceneEditApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSceneEditApp)
	afx_msg void OnAppAbout();
	afx_msg void OnToolsOptions();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern CSceneEditApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCENEEDIT_H__73BD41A5_0205_405C_AE8E_679F61C115B8__INCLUDED_)
