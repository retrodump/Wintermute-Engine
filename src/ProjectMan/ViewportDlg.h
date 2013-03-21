#if !defined(AFX_VIEWPORTDLG_H__23D9C523_4AE5_43BD_8B71_DBE9F1EE6CA1__INCLUDED_)
#define AFX_VIEWPORTDLG_H__23D9C523_4AE5_43BD_8B71_DBE9F1EE6CA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewportDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewportDlg dialog

class CViewportDlg : public CDialog
{
// Construction
public:
	void SetState();
	RECT m_Rect;
	CViewportDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CViewportDlg)
	enum { IDD = IDD_VIEWPORT };
	CEdit	m_Width;
	CEdit	m_PosY;
	CEdit	m_PosX;
	CEdit	m_Height;
	CButton	m_Fullscreen;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CViewportDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnFullscreen();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWPORTDLG_H__23D9C523_4AE5_43BD_8B71_DBE9F1EE6CA1__INCLUDED_)
