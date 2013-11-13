#if !defined(AFX_POINTPOSDLG_H__39436FFD_3EB3_4BF4_96A4_F4C1846DE976__INCLUDED_)
#define AFX_POINTPOSDLG_H__39436FFD_3EB3_4BF4_96A4_F4C1846DE976__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PointPosDlg.h : header file
//

#include "../mfc_shared/amsEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CPointPosDlg dialog

class CPointPosDlg : public CDialog
{
// Construction
public:
	CPointPosDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPointPosDlg)
	enum { IDD = IDD_POINT_POS };
	CAMSIntegerEdit	m_PosYEdit;
	CAMSIntegerEdit	m_PosXEdit;
	int		m_PosY;
	int		m_PosX;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPointPosDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPointPosDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POINTPOSDLG_H__39436FFD_3EB3_4BF4_96A4_F4C1846DE976__INCLUDED_)
