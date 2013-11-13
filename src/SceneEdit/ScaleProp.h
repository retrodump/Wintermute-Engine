#if !defined(AFX_SCALEPROP_H__B49F8293_4E82_4A29_8E0E_DC8BE0CFAA06__INCLUDED_)
#define AFX_SCALEPROP_H__B49F8293_4E82_4A29_8E0E_DC8BE0CFAA06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScaleProp.h : header file
//

#include "../mfc_shared/amsEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CScaleProp dialog

class CScaleProp : public CDialog
{
// Construction
public:
	CScaleProp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScaleProp)
	enum { IDD = IDD_SCALE };
	CAMSIntegerEdit	m_PosYEdit;
	int		m_PosY;
	UINT	m_Scale;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScaleProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScaleProp)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCALEPROP_H__B49F8293_4E82_4A29_8E0E_DC8BE0CFAA06__INCLUDED_)
