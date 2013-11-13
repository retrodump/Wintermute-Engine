#pragma once

#include "../mfc_shared/amsEdit.h"
#include "SceneView.h"
#include "afxwin.h"
#include "afxcmn.h"


// C3DPage dialog

class C3DPage : public CDialog
{
	DECLARE_DYNAMIC(C3DPage)

public:
	C3DPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~C3DPage();

	void SetCurrentView(CSceneView *View);
	CSceneView* m_View;
	void Redraw();
	void ScaleFilePair(CButton *Button, CEdit *Edit);
	void SetState();
	void SetResolution();
	void SetWaypointHeight();


// Dialog Data
	enum { IDD = IDD_3D_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_BrowseGeom;
	CButton m_ShowGeom;
	CEdit m_GeomName;
	CComboBox m_Camera;
	CComboBox m_Shadows;
	virtual BOOL OnInitDialog();
	afx_msg void OnCameraSel();
	afx_msg void OnShadowsSel();
	afx_msg void OnBrowseGeom();
	afx_msg void OnShowGeom();
	CAMSIntegerEdit m_ResWidth;
	CAMSIntegerEdit m_ResHeight;

	afx_msg void OnEnKillfocusResWidth();
	afx_msg void OnEnKillfocusResHeight();
	CButton m_OverrideFOV;
	CSliderCtrl m_FOV;
	CAMSNumericEdit m_FOVVal;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void SetFOV();
	afx_msg void OnBnClickedOverrideFov();
	CAMSNumericEdit m_WaypointHeight;
	afx_msg void OnEnKillfocusFovVal();
	afx_msg void OnEnKillfocusWaypointHeight();
	CAMSNumericEdit m_NearPlane;
	CAMSNumericEdit m_FarPlane;
	void SetClipping();
	afx_msg void OnEnKillfocusNearPlane();
	afx_msg void OnEnKillfocusFarPlane();
	CButton m_2DPathfinding;
	afx_msg void On2DPathfinding();
	CButton m_Scroll3DCompatibility;
	afx_msg void OnScroll3DCompatibility();
	afx_msg void OnColAmbient();
	void SetFOVVal();
	CBCGColorButton	m_AmbientColor;
};
