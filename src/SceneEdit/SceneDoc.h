// SceneDoc.h : interface of the CSceneDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENEDOC_H__C660E4E9_3289_4ECA_93EF_8A413E945D2A__INCLUDED_)
#define AFX_SCENEDOC_H__C660E4E9_3289_4ECA_93EF_8A413E945D2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifdef DCGF_ADVENTURE
	#include "dcgf_ad.h"
#else
	#include "dcgf.h"
#endif



class CSceneDoc : public CDocument
{
protected: // create from serialization only
	CSceneDoc();
	DECLARE_DYNCREATE(CSceneDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSceneDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSceneDoc();

	HRESULT WriteEditorData(LPCTSTR lpszPathName, CBDynBuffer* Buffer);
	bool QueryProjectRoot(CString Filename="");
	CString m_ProjectRoot;



#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSceneDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCENEDOC_H__C660E4E9_3289_4ECA_93EF_8A413E945D2A__INCLUDED_)
