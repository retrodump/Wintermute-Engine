// ProjectMan.h : main header file for the PROJECTMAN application
//

#if !defined(AFX_PROJECTMAN_H__3270ADB5_A201_46BD_9E1D_6C47C63ED7BF__INCLUDED_)
#define AFX_PROJECTMAN_H__3270ADB5_A201_46BD_9E1D_6C47C63ED7BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "../MFCExt/DCGFApp.h"
#include "../MFCExt/DCGFStringTable.h"

/////////////////////////////////////////////////////////////////////////////
// CApp:
// See ProjectMan.cpp for the implementation of this class
//

class CApp : public CDCGFApp
{
public:
	CApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CApp)
	afx_msg void OnAppAbout();
	afx_msg void OnToolsSceneEdit();
	afx_msg void OnToolsSpriteEdit();
	afx_msg void OnToolsStringTable();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void DoSomeNagging();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTMAN_H__3270ADB5_A201_46BD_9E1D_6C47C63ED7BF__INCLUDED_)
