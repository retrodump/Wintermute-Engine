#if !defined(AFX_DCGFAPP_H__765FE56C_6377_4293_A27B_28D93D64B874__INCLUDED_)
#define AFX_DCGFAPP_H__765FE56C_6377_4293_A27B_28D93D64B874__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DCGFApp.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CDCGFApp thread

class CDCGFApp : public CWinApp
{
	DECLARE_DYNCREATE(CDCGFApp)
protected:
	CDCGFApp();           // protected constructor used by dynamic creation

// Attributes
public:
	CStringArray m_ProjectRoots;
	virtual BOOL OnIdle(LONG lCount);
	bool m_Active;

	CString m_LastDirScript;

// Operations
public:
	void AddProjectRoot(CString Folder);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDCGFApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDCGFApp();

	// Generated message map functions
	//{{AFX_MSG(CDCGFApp)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DCGFAPP_H__765FE56C_6377_4293_A27B_28D93D64B874__INCLUDED_)
