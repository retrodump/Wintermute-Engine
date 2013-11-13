#if !defined(AFX_ROTPROP_H__FDFC4D14_357B_4E0C_90F4_34B0A5FEFF5A__INCLUDED_)
#define AFX_ROTPROP_H__FDFC4D14_357B_4E0C_90F4_34B0A5FEFF5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RotProp.h : header file
//

#include "../mfc_shared/amsEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CRotProp dialog

class CRotProp : public CDialog
{
// Construction
public:
	CRotProp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRotProp)
	enum { IDD = IDD_ROT };
	CAMSIntegerEdit	m_PosXEdit;
	int		m_PosX;
	int		m_Rotation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRotProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRotProp)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROTPROP_H__FDFC4D14_357B_4E0C_90F4_34B0A5FEFF5A__INCLUDED_)
