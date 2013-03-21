#if !defined(AFX_SOUNDPAGE_H__3F428DC7_B49B_4B7A_BBC3_F3E6BC596B67__INCLUDED_)
#define AFX_SOUNDPAGE_H__3F428DC7_B49B_4B7A_BBC3_F3E6BC596B67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SoundPage.h : header file
//

#include "../mfc_shared/amsEdit.h"
#include "dcgf_ad.h"
#include "SceneView.h"


/////////////////////////////////////////////////////////////////////////////
// CSoundPage dialog

class CSoundPage : public CDialog
{
// Construction
public:
	CFont m_BoldFont;
	CSoundPage(CWnd* pParent = NULL);   // standard constructor

	void SetState();
	void SetPos();
	void ShowEntity(CAdEntity* Entity, CSceneView* View, bool Force=false);
	CAdEntity* m_Entity;
	void ScaleFilePair(CButton* Button, CEdit* Edit);
	CSceneView* m_View;


// Dialog Data
	//{{AFX_DATA(CSoundPage)
	enum { IDD = IDD_SOUND_PAGE };
	CSpinButtonCtrl	m_VolumeSpin;
	CEdit	m_Volume;
	CEdit	m_StartTime;
	CButton	m_SoundPanning;
	CBCGButton	m_Custom;
	CBCGButton	m_Play;
	CBCGButton	m_BrowseSound;
	CBCGButton	m_Events;
	CEdit	m_Sound;
	CAMSIntegerEdit	m_PosY;
	CAMSIntegerEdit	m_PosX;
	CEdit	m_Name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSoundPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSoundPage)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeName();
	afx_msg void OnBrowseSound();
	afx_msg void OnEvents();
	afx_msg void OnKillfocusPosX();
	afx_msg void OnKillfocusPosY();
	afx_msg void OnPlay();
	afx_msg void OnCustom();
	afx_msg void OnSoundPanning();
	afx_msg void OnKillfocusStartTime();
	afx_msg void OnKillfocusVolume();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOUNDPAGE_H__3F428DC7_B49B_4B7A_BBC3_F3E6BC596B67__INCLUDED_)
