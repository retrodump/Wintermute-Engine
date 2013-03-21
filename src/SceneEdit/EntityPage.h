#if !defined(AFX_ENTITYPAGE_H__67887E7A_1B41_4DFB_AABF_71BCC8F91451__INCLUDED_)
#define AFX_ENTITYPAGE_H__67887E7A_1B41_4DFB_AABF_71BCC8F91451__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EntityPage.h : header file
//

#include "../mfc_shared/amsEdit.h"
#include "dcgf_ad.h"
#include "SceneView.h"

/////////////////////////////////////////////////////////////////////////////
// CEntityPage dialog

class CEntityPage : public CDialog
{
// Construction
public:
	CFont m_BoldFont;
	void SetState();
	void SetPos();
	void ShowEntity(CAdEntity* Entity, CSceneView* View, bool Force=false);
	CAdEntity* m_Entity;
	void ScaleFilePair(CButton* Button, CEdit* Edit);
	CEntityPage(CWnd* pParent = NULL);   // standard constructor
	CSceneView* m_View;

// Dialog Data
	//{{AFX_DATA(CEntityPage)
	enum { IDD = IDD_ENTITY_PAGE };
	CEdit	m_Item;
	CButton	m_IgnoreItems;
	CButton	m_SoundPanning;
	CBCGButton	m_Custom;
	CBCGButton	m_BrowseCaption;
	CBCGButton	m_WalkToGet;
	CBCGButton	m_WalkToSet;
	CEdit	m_Caption;
	CBCGButton	m_Events;
	CEdit	m_Sprite;
	CButton	m_Shadowable;
	CButton	m_Scalable;
	CButton	m_Registrable;
	CComboBox m_WalkToDir;
	CAMSIntegerEdit	m_PosY;
	CAMSIntegerEdit	m_PosX;
	CAMSIntegerEdit	m_WalkToY;
	CAMSIntegerEdit	m_WalkToX;
	CEdit	m_Font;
	CEdit	m_Cursor;
	CEdit	m_Name;
	CBCGButton	m_BrowseSprite;
	CBCGButton	m_BrowseFont;
	CBCGButton	m_BrowseCursor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEntityPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEntityPage)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeName();
	afx_msg void OnRegistrable();
	afx_msg void OnScalable();
	afx_msg void OnShadowable();
	afx_msg void OnBrowseSprite();
	afx_msg void OnBrowseFont();
	afx_msg void OnBrowseCursor();
	afx_msg void OnKillfocusPosX();
	afx_msg void OnKillfocusPosY();
	afx_msg void OnKillfocusWalkX();
	afx_msg void OnKillfocusWalkY();
	afx_msg void OnEvents();
	afx_msg void OnChangeCaption();
	afx_msg void OnCustom();
	afx_msg void OnSoundPanning();
	afx_msg void OnIgnoreItems();
	afx_msg void OnChangeItemName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnWalkDir();
	afx_msg void OnWalkGet();
	afx_msg void OnWalkSet();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENTITYPAGE_H__67887E7A_1B41_4DFB_AABF_71BCC8F91451__INCLUDED_)
