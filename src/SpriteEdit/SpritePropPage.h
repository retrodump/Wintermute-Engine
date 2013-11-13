#if !defined(AFX_SPRITEPROPPAGE_H__D112358B_E1BC_4458_9C0D_0D6B8C473D52__INCLUDED_)
#define AFX_SPRITEPROPPAGE_H__D112358B_E1BC_4458_9C0D_0D6B8C473D52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpritePropPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSpritePropPage dialog

#include "SpriteView.h"
#include "afxwin.h"

class CSpritePropPage : public CDialog
{
// Construction
public:
	CFont m_BoldFont;
	void SetState();
	void Redraw();
	CSpriteView* m_View;
	void SetCurrentView(CSpriteView* View);
	CSpritePropPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSpritePropPage)
	enum { IDD = IDD_SPRITE_PROP_PAGE };
	CButton	m_Streamed;
	CButton	m_Muted;
	CButton	m_Precise;
	CEdit	m_Name;
	CButton	m_Looping;
	CButton	m_Continuous;
	CButton	m_KeepLoaded;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpritePropPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSpritePropPage)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeName();
	afx_msg void OnLooping();
	afx_msg void OnPrecise();
	afx_msg void OnContinuos();
	afx_msg void OnMuted();
	afx_msg void OnStreamed();
	afx_msg void OnKeepLoaded();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedScripts();
	CButton m_Sprites;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPRITEPROPPAGE_H__D112358B_E1BC_4458_9C0D_0D6B8C473D52__INCLUDED_)
