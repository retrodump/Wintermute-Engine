#if !defined(AFX_PROPSDLG_H__372E2895_2AEC_4F59_AC45_510AE5040B70__INCLUDED_)
#define AFX_PROPSDLG_H__372E2895_2AEC_4F59_AC45_510AE5040B70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropsDlg.h : header file
//

#include "dcgf_ad.h"
#include "SceneView.h"
#include "PropPair.h"

/////////////////////////////////////////////////////////////////////////////
// CPropsDlg dialog

class CPropsDlg : public CDialog
{
// Construction
public:
	int GetCurSel();
	virtual  ~CPropsDlg();
	HRESULT DeleteProps();
	CPropsDlg(CWnd* pParent = NULL);   // standard constructor

	void SetState();
	void Redraw(int sel = -1);
	CBObject* m_Object;
	CArray<CPropPair*, CPropPair*> m_Properties;
	CSceneView* m_View;


// Dialog Data
	//{{AFX_DATA(CPropsDlg)
	enum { IDD = IDD_CUSTOM_PROP };
	CBCGButton	m_RemoveProp;
	CListCtrl	m_PropList;
	CBCGButton	m_EditProp;
	CBCGButton	m_AddProp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPropsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnItemchangedPropList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkPropList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddProp();
	afx_msg void OnEditProp();
	afx_msg void OnRemoveProp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPSDLG_H__372E2895_2AEC_4F59_AC45_510AE5040B70__INCLUDED_)
