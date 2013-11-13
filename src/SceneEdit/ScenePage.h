#if !defined(AFX_SCENEPAGE_H__427A01A6_8048_4C31_81C7_2D190BC49190__INCLUDED_)
#define AFX_SCENEPAGE_H__427A01A6_8048_4C31_81C7_2D190BC49190__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScenePage.h : header file
//

#include "SceneView.h"

/////////////////////////////////////////////////////////////////////////////
// CScenePage dialog

class CScenePage : public CDialog
{
// Construction
public:
	CString GetUniqueName(CString Name);
	CMenu m_EntityMenu;
	void Redraw();
	void SetState();
	void RedrawNodes();
	CImageList m_ImageList;
	void SetCurrentView(CSceneView* View);
	CSceneView* m_View;
	CScenePage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScenePage)
	enum { IDD = IDD_SCENE_PAGE };
	CBCGButton m_AddRot;
	CStatic	m_Divide;
	CBCGButton	m_AddScale;
	CBCGButton	m_RemovePoint;
	CBCGButton	m_RemoveNode;
	CBCGButton	m_NodeUp;
	CBCGButton	m_NodeDown;
	CBCGButton	m_AddRegion;
	CBCGButton	m_AddPoint;
	CBCGMenuButton	m_AddEntity;
	CBCGButton	m_RemoveLayer;
	CBCGButton	m_LayerUp;
	CBCGButton	m_LayerProperties;
	CBCGButton	m_LayerDown;
	CBCGButton m_AddLayer;
	CListCtrl	m_NodeList;
	CListCtrl	m_LayerList;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScenePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScenePage)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeLayer(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeNode(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddLayer();
	afx_msg void OnAddEntity();
	afx_msg void OnAddPoint();
	afx_msg void OnAddRegion();
	afx_msg void OnLayerDown();
	afx_msg void OnLayerProperties();
	afx_msg void OnLayerUp();
	afx_msg void OnNodeDown();
	afx_msg void OnNodeUp();
	afx_msg void OnRemoveLayer();
	afx_msg void OnRemoveNode();
	afx_msg void OnRemovePoint();
	afx_msg void OnAddScale();
	afx_msg void OnAddRot();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCENEPAGE_H__427A01A6_8048_4C31_81C7_2D190BC49190__INCLUDED_)
