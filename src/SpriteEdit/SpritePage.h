#if !defined(AFX_SPRITEPAGE_H__1C67CC33_5769_4E5C_A6F5_3788F3E65CAC__INCLUDED_)
#define AFX_SPRITEPAGE_H__1C67CC33_5769_4E5C_A6F5_3788F3E65CAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpritePage.h : header file
//

#include "SpriteView.h"

/////////////////////////////////////////////////////////////////////////////
// CSpritePage dialog

class CSpritePage : public CDialog
{
// Construction
public:
	bool m_IsRedrawing;
	void SetState();
	void SetCurrentView(CSpriteView* View);
	CSpriteView* m_View;
	void Redraw();
	CSpritePage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSpritePage)
	enum { IDD = IDD_SPRITE_PAGE };
	CButton	m_Looping;
	CBCGButton	m_PrevFrame;
	CBCGButton	m_PlayStop;
	CBCGButton	m_NextFrame;
	CBCGButton	m_LastFrame;
	CBCGButton	m_FirstFrame;
	CBCGButton	m_AddSubframe;
	CBCGButton	m_SubframeDown;
	CBCGButton	m_SubframeUp;
	CBCGButton	m_FrameDown;
	CBCGButton	m_FrameUp;
	CBCGButton	m_RemoveFrame;
	CBCGButton	m_AddFrame;
	CTreeCtrl	m_SpriteTree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpritePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSpritePage)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangedSprite(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpandedSprite(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFrameUp();
	afx_msg void OnFrameDown();
	afx_msg void OnSubframeUp();
	afx_msg void OnSubframeDown();
	afx_msg void OnRemoveFrame();
	afx_msg void OnAddFrame();
	afx_msg void OnAddSubframe();
	afx_msg void OnFirstFrame();
	afx_msg void OnLastFrame();
	afx_msg void OnNextFrame();
	afx_msg void OnPrevFrame();
	afx_msg void OnPlayStop();
	afx_msg void OnLooping();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPRITEPAGE_H__1C67CC33_5769_4E5C_A6F5_3788F3E65CAC__INCLUDED_)
