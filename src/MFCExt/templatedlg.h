#if !defined(AFX_TEMPLATEDLG_H__EA0BFD5E_F601_46BA_850A_3453DA2713E9__INCLUDED_)
#define AFX_TEMPLATEDLG_H__EA0BFD5E_F601_46BA_850A_3453DA2713E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TemplateDlg.h : header file
//

#include "resource.h"
#include "PropList.h"
#include "TemplateMgr.h"	// Added by ClassView
#include "../engine_core/wme_base/dcgf.h"

/////////////////////////////////////////////////////////////////////////////
// CTemplateDlg dialog

class CTemplateDlg : public CDialog
{
// Construction
public:
	CString m_SelectedObject;
	CString m_SelectedName;
	CString m_ProjectRoot;
	bool IsBinary(CString Filename);
	bool ParseMergeFile(CString SourceDir, CString TargetDir, CString MergeFile);
	CBGame* Game;
	bool ParseFile(CString SourceFile, CString TargetFile);
	bool ParseDir(CString SourceDir, CString TargetDir);
	CString m_Title;
	CString m_InitName;
	CString m_Subdir;
	bool DisplayProps();
	void SetOKState();
	CTemplateMgr m_TemplateMgr;
	bool HandleTemplate(CString Name);
	bool ReadTemplates();
	bool ProcessDir(CString SourceDir, CString TargetPath);
	bool ProcessFile(CString SourceFile, CString TargetPath);
	CString m_Target;
	CString m_Source;
	CString m_LastName;
	bool m_SelectDir;
	CTemplateDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTemplateDlg();
	CImageList m_ImageList;
	CStringArray m_RepPaths;
	static void HookPropChange(CPLProperty *Prop, CPLCategory *Cat, void *Data);
	void OnPropChange(CPLProperty *Prop, CPLCategory *Cat);

// Dialog Data
	//{{AFX_DATA(CTemplateDlg)
	enum { IDD = IDD_TEMPLATE };
	CButton	m_OK;
	CStatic	m_DirLabel;
	CButton	m_Browse;
	CStatic	m_NameLabel;
	CEdit	m_Dir;
	CEdit	m_Name;
	CListCtrl	m_Templates;
	CComboBox	m_Repository;
	CPropList	m_PropList;
	CEdit	m_Desc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTemplateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTemplateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeRepository();
	afx_msg void OnBrowse();
	afx_msg void OnChangeName();
	virtual void OnOK();
	afx_msg void OnChangeDir();
	afx_msg void OnChangeTemplate(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void AddRepositoryPath(CString Path);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEMPLATEDLG_H__EA0BFD5E_F601_46BA_850A_3453DA2713E9__INCLUDED_)
