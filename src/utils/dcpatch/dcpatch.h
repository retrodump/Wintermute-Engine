// dcpatch.h : main header file for the dcpatch application
//

#if !defined(AFX_dcpatch_H__19324DFB_901F_4F1E_97A4_623D2590B794__INCLUDED_)
#define AFX_dcpatch_H__19324DFB_901F_4F1E_97A4_623D2590B794__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "utils.h"
#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMainApp:
// See dcpatch.cpp for the implementation of this class
//

class CMainApp : public CWinApp
{
public:
	CMainApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMainApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_dcpatch_H__19324DFB_901F_4F1E_97A4_623D2590B794__INCLUDED_)
