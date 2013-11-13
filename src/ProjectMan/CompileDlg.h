#if !defined(AFX_COMPILEDLG_H__701B26A3_4BD2_4DBB_AD7B_9F76602CCAF6__INCLUDED_)
#define AFX_COMPILEDLG_H__701B26A3_4BD2_4DBB_AD7B_9F76602CCAF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CompileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCompileDlg dialog

class CCompileDlg : public CDialog
{
// Construction
public:
	void Update();
	bool m_CloseRequest;
	CCompileDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCompileDlg)
	enum { IDD = IDD_COMPILE };
	CStatic	m_Title2;
	CStatic	m_Title1;
	CProgressCtrl	m_Progress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCompileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCompileDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMPILEDLG_H__701B26A3_4BD2_4DBB_AD7B_9F76602CCAF6__INCLUDED_)
