#if !defined(AFX_FRAMEPAGE_H__6A070AB2_BD1E_48AC_966A_32F3DEDF8B11__INCLUDED_)
#define AFX_FRAMEPAGE_H__6A070AB2_BD1E_48AC_966A_32F3DEDF8B11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FramePage.h : header file
//

#include "../mfc_shared/amsEdit.h"
#include "dcgf.h"
#include "SpriteView.h"


/////////////////////////////////////////////////////////////////////////////
// CFramePage dialog

class CFramePage : public CDialog
{
// Construction
public:
	void ShowTotalTime();
	void SetPos();
	void ShowRGB();
	void SetState();
	CFramePage(CWnd* pParent = NULL);   // standard constructor
	void ShowFrame(CSpriteView* View, CBFrame* Frame, CBSubFrame* Subframe, bool Force=false);
	CSpriteView* m_View;
	CBFrame* m_Frame;
	CBSubFrame* m_Subframe;
	void ScalePair(CButton *Button, CEdit *Edit);

// Dialog Data
	//{{AFX_DATA(CFramePage)
	enum { IDD = IDD_FRAME_PAGE };
	CEdit	m_TotalTime;
	CBCGButton	m_CopyAlpha;
	CBCGButton	m_CopyTrans;
	CBCGButton	m_CopyMirror;
	CButton	m_KillSound;
	CButton	m_Keyframe;
	CButton	m_Decoration;
	CEdit	m_Event;
	CEdit	m_Sound;
	CBCGButton	m_BrowseSound;
	CEdit	m_Alpha;
	CBCGColorButton	m_Transparent;
	CStatic	m_TransRGB;
	CAMSIntegerEdit	m_MoveY;
	CAMSIntegerEdit	m_MoveX;
	CEdit	m_Image;
	CAMSIntegerEdit	m_HotspotY;
	CAMSIntegerEdit	m_HotspotX;
	CEdit	m_Delay;
	CBCGButton	m_CopyMove;
	CBCGButton	m_CopyHotspot;
	CBCGButton	m_CopyDelay;
	CBCGButton	m_BrowseImage;
	CSpinButtonCtrl	m_AlphaSpin;
	CStatic	m_AlphaRGB;
	CBCGColorButton	m_AlphaColor;
	CButton	m_3DOnly;
	CButton	m_2DOnly;
	CButton	m_MirrorX;
	CButton	m_MirrorY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFramePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFramePage)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLeaveNum();
	afx_msg void OnChangeAlpha();
	afx_msg void OnTransparent();
	afx_msg void OnAlphaColor();
	afx_msg void On2dOnly();
	afx_msg void On3dOnly();
	afx_msg void OnMirrorX();
	afx_msg void OnMirrorY();
	afx_msg void OnBrowseImage();
	afx_msg void OnCopyDelay();
	afx_msg void OnCopyMirror();
	afx_msg void OnCopyHotspot();
	afx_msg void OnCopyMove();
	afx_msg void OnBrowseSound();
	afx_msg void OnKeyframe();
	afx_msg void OnDecoration();
	afx_msg void OnKillSound();
	afx_msg void OnCopyTrans();
	afx_msg void OnCopyAlpha();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAMEPAGE_H__6A070AB2_BD1E_48AC_966A_32F3DEDF8B11__INCLUDED_)
