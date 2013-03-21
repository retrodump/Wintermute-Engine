#if !defined(AFX_LAYERDLG_H__12770E10_C924_40AC_9D8F_7A6A589675CE__INCLUDED_)
#define AFX_LAYERDLG_H__12770E10_C924_40AC_9D8F_7A6A589675CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LayerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLayerDlg dialog

class CLayerDlg : public CDialog
{
// Construction
public:
	bool m_MainLayer;
	CLayerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLayerDlg)
	enum { IDD = IDD_LAYER };
	CButton	m_BtnCloseUp;
	CBCGButton	m_OK;
	CBCGButton	m_Cancel;
	CStatic	m_IsMain;
	CString	m_Name;
	int		m_Height;
	int		m_Width;
	BOOL	m_CloseUp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLayerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLayerDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAYERDLG_H__12770E10_C924_40AC_9D8F_7A6A589675CE__INCLUDED_)
