// ProjectDoc.h : interface of the CProjectDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJECTDOC_H__09543FD4_5F59_434A_93FD_2A67DE338F2C__INCLUDED_)
#define AFX_PROJECTDOC_H__09543FD4_5F59_434A_93FD_2A67DE338F2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../engine_core/wme_ad/dcgf_ad.h"
#include "../MFCExt/DCGFDoc.h"
#include "Package.h"
#include "PackagerFilter.h"
#include "EdSettings.h"
#include <afxtempl.h>

class CPackagerFilter;
class CProjectView;

class CProjectDoc : public CDCGFDoc
{
protected: // create from serialization only
	CProjectDoc();
	DECLARE_DYNCREATE(CProjectDoc)

// Attributes
public:
	CArray <CPackage*, CPackage*> m_Packages;
	CArray <CPackagerFilter*, CPackagerFilter*> m_Filters;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectDoc)
	public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnNewDocument();
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SortPackagesByPriority();
	static bool PriorityComparer(const CPackage* x, const CPackage* y);
	void PrioritizePackage(CString Name);
	CProjectView* GetView();
	CString GetWMEPath();
	void AddInfo(CString Text, CString Filename="");
	void AddLog(CString Text, CString Filename="");
	void AddError(CString Text, CString Filename="");
	void AddWarning(CString Text, CString Filename="");
	void ValidatePackages();
	bool IsPrimaryPackage(CString Name);
	CString GetFullPath(CString Filename);
	int m_PackGameVersion;
	HRESULT SetProjectRoot(LPCSTR Filename);
	HRESULT SetDefaults();
	HRESULT Cleanup();
	CString m_PackOutputFolder;
	bool m_PackBuildMasterIndex;
	bool m_EditorPreview;
	bool m_CompressedSavegames;
	bool m_ShowFPS;
	bool m_AllowWindowed;
	bool m_DebugMode;
	int m_ConsoleMode;
	bool m_D3D9;

	bool m_PackCopyExe;
	CString m_PackExeName;
	bool m_PackChangeIcon;
	CString m_PackIconName;
	CString m_BindPackage;

	bool m_PackCopySet;
	CString m_PackSetName;
	bool m_PackChangeIconSet;
	CString m_PackIconNameSet;

	bool m_PackCopyD3DX;

	bool m_PackAddGeData;
	CString m_PackGdfFile;
	CString m_PackGdfThumbnail;

	bool m_PackAddCrashLib;
	bool m_PackAddFpsDisplay;
	bool m_PackAddDebugMode;

	bool CompilePackages(CPackage* SinglePackage=NULL);


	CStringArray m_PluginList;
	virtual ~CProjectDoc();
	CAdGame* Game;
	CEdSettings* m_Settings;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CString GetUtf8String(CString& OrigStr, bool RemoveUnsafeChars=false);	

// Generated message map functions
protected:
	//{{AFX_MSG(CProjectDoc)
	afx_msg void OnProjectCompile();
	afx_msg void OnProjectRun();
	afx_msg void OnGenerateGdf();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTDOC_H__09543FD4_5F59_434A_93FD_2A67DE338F2C__INCLUDED_)
