#if !defined(AFX_VIEWPROPS_H__E4ABD20D_E352_4B7B_88CB_0DFBADC033C3__INCLUDED_)
#define AFX_VIEWPROPS_H__E4ABD20D_E352_4B7B_88CB_0DFBADC033C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewProps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewProps form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "../MFCExt/FormViewEx.h"
#include "ProjectView.h"
#include "Package.h"
#include "../MFCExt/PropList.h"


class CViewProps : public CFormViewEx
{
protected:
	CViewProps();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewProps)

// Form Data
public:
	//{{AFX_DATA(CViewProps)
	enum { IDD = IDD_PROPS };
	CPropList	m_PropList;
	//}}AFX_DATA

// Attributes
public:
	CProjectView* m_View;

// Operations
public:
	void SetCurrentPackage(CPackage* Package=NULL);
	CPackage* m_CurrentPackage;
	void CopyStr(char* Dest, const char* Src);
	HRESULT DisplayProps();
	HRESULT Redraw();

	void BrowseFile(CProjectDoc* Doc, CPLProperty* prop, char** pFilename, CString Mask, CString DefaultDir="");
	void OnPropChange(CPLProperty* Prop, CPLCategory* Cat);
	void OnPropFocus(CPLProperty* Prop, CPLCategory* Cat);
	bool OnButtonScripts(CButton* Button, CPLPropertyEditEx* Prop, CPLCategory* Cat);
	bool OnButtonViewport(CButton* Button, CPLPropertyEditEx* Prop, CPLCategory* Cat);
	bool OnButtonSaveLoad(CButton* Button, CPLPropertyEditEx* Prop, CPLCategory* Cat);
	bool OnButtonFile(CButton* Button, CPLPropertyEditEx* Prop, CPLCategory* Cat);
	bool OnButtonGUID(CButton* Button, CPLPropertyEditEx* Prop, CPLCategory* Cat);
	bool OnButtonCompat(CButton* Button, CPLPropertyEditEx* Prop, CPLCategory* Cat);
	static void HookPropChange(CPLProperty* Prop, CPLCategory* Cat, void* Data);
	static void HookPropFocus(CPLProperty* Prop, CPLCategory* Cat, void* Data);
	static bool HookButtonScripts(CButton* Button, CPLPropertyEditEx* Prop, CPLCategory* Cat, void* Data);
	static bool HookButtonViewport(CButton* Button, CPLPropertyEditEx* Prop, CPLCategory* Cat, void* Data);
	static bool HookButtonSaveLoad(CButton* Button, CPLPropertyEditEx* Prop, CPLCategory* Cat, void* Data);
	static bool HookButtonFile(CButton* Button, CPLPropertyEditEx* Prop, CPLCategory* Cat, void* Data);
	static bool HookButtonGUID(CButton* Button, CPLPropertyEditEx* Prop, CPLCategory* Cat, void* Data);
	static bool HookButtonCompat(CButton* Button, CPLPropertyEditEx* Prop, CPLCategory* Cat, void* Data);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewProps)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewProps();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewProps)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPopupFileBrowse();
	afx_msg void OnPopupFileEdit();
	afx_msg void OnPopupFileRemove();
	afx_msg void OnUpdatePopupFileEdit(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePopupFileRemove(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWPROPS_H__E4ABD20D_E352_4B7B_88CB_0DFBADC033C3__INCLUDED_)
