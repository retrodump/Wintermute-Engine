#if !defined(AFX_VIEWTREE_H__892BF323_2260_4F6F_9252_627E507C2592__INCLUDED_)
#define AFX_VIEWTREE_H__892BF323_2260_4F6F_9252_627E507C2592__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewTree.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewTree form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "../MFCExt/FormViewEx.h"
#include "ProjectView.h"
#include "TreeItem.h"


class CViewTree : public CFormViewEx
{
protected:
	CViewTree();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewTree)

// Form Data
public:
	//{{AFX_DATA(CViewTree)
	enum { IDD = IDD_TREE };
	CButton	m_Preview;
	CTreeCtrl	m_Tree;
	//}}AFX_DATA

// Attributes
public:
	CProjectView* m_View;
	bool m_ScheduledEdit;
// Operations
public:
	static void __stdcall ErrorCallback(int Line, char *Text, void *Data);
	CString m_ScriptError;
	CString GetCurrentFile();
	CString GetCurrentPath();
	CString m_LastImportDir;
	void GetExpandedItems(HTREEITEM Parent);
	CStringList m_ExpandedList;
	HRESULT CreateFolder(CString BasePath, bool IsDir=true);
	bool m_Redrawing;
	HRESULT Refresh();
	HRESULT SelectFile(CString Filename);
	HRESULT DisplayPopupMenu(CPoint Point, CTreeItem* Item);
	HRESULT DisplaySimplePopupMenu(CPoint Point);
	DWORD GetFileType(CString Filename);
	DWORD GetFolderType(CString Filename, CString& Keyfile);
	CTreeItem* FillItemInfo(CProjectDoc* Doc, CFileFind& InFinder, HTREEITEM Parent);
	void DeleteSubitems(HTREEITEM Parent);
	HRESULT Redraw(bool InitialRedraw = true);
	void ExploreDir(CString Dir, HTREEITEM Parent);
	static int CALLBACK TreeCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	bool m_ShowHiddenFiles;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewTree)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewTree();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewTree)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPopupTreeOpen();
	afx_msg void OnPopupTreeEdit();
	afx_msg void OnPopupTreeCreateFolder();
	afx_msg void OnPopupTreeRefresh();
	afx_msg void OnPopupTreeExplore();
	afx_msg void OnPopupTreeFolderTop();
	afx_msg void OnPopupTreeRemove();
	afx_msg void OnPopupImportFolder();
	afx_msg void OnPopupImportFile();
	afx_msg void OnPopupTreePromote();
	afx_msg void OnPopupTreeDegrade();
	afx_msg void OnPopupTreeCompile();
	afx_msg void OnPopupTreePrimaryPackage();
	afx_msg void OnPopupAddActor();
	afx_msg void OnPopupAddEntity();
	afx_msg void OnPopupAddFont();
	afx_msg void OnPopupAddScene();
	afx_msg void OnPopupAddScript();
	afx_msg void OnPopupAddSprite();
	afx_msg void OnPopupAddWindow();
	afx_msg void OnPreview();
	afx_msg void OnPopupTreeCopy();
	afx_msg void OnPopupTreeCheckSyntax();
	afx_msg void OnPopupTreeStartupScene();
	afx_msg void OnPopupTreeStartupSceneDebug();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPopupAddActor3d();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWTREE_H__892BF323_2260_4F6F_9252_627E507C2592__INCLUDED_)
