#if !defined(AFX_REGIONPAGE_H__3221EB3F_FFCC_49F5_8D8E_3A79BFEE0D4F__INCLUDED_)
#define AFX_REGIONPAGE_H__3221EB3F_FFCC_49F5_8D8E_3A79BFEE0D4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegionPage.h : header file
//

#include "dcgf_ad.h"
#include "SceneView.h"

/////////////////////////////////////////////////////////////////////////////
// CRegionPage dialog

class CRegionPage : public CDialog
{
// Construction
public:	
	CFont m_BoldFont;
	void SetState();
	void ShowRegion(CAdRegion* Region, CSceneView* View, bool Force=false);
	CAdRegion* m_Region;
	CRegionPage(CWnd* pParent = NULL);   // standard constructor
	CSceneView* m_View;

// Dialog Data
	//{{AFX_DATA(CRegionPage)
	enum { IDD = IDD_REGION_PAGE };
	CBCGButton	m_Custom;
	CBCGButton	m_Events;
	CSpinButtonCtrl	m_ScaleSpin;
	CEdit	m_Scale;
	CEdit	m_Name;
	CButton	m_TypeBlocked;
	CButton	m_TypeLayout;
	CButton	m_TypeDecoration;
	CBCGColorButton m_Alpha;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegionPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegionPage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeName();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnChangeScale();
	afx_msg void OnAlpha();
	afx_msg void OnEvents();
	afx_msg void OnCustom();
	afx_msg void OnTypeBlocked();
	afx_msg void OnTypeDecoration();
	afx_msg void OnTypeLayout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGIONPAGE_H__3221EB3F_FFCC_49F5_8D8E_3A79BFEE0D4F__INCLUDED_)
