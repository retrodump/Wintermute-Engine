// SpriteDoc.h : interface of the CSpriteDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPRITEDOC_H__E9585F32_985C_4BE8_AA5E_AFE42D780285__INCLUDED_)
#define AFX_SPRITEDOC_H__E9585F32_985C_4BE8_AA5E_AFE42D780285__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dcgf.h"


class CSpriteDoc : public CDocument
{
protected: // create from serialization only
	CSpriteDoc();
	DECLARE_DYNCREATE(CSpriteDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpriteDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSpriteDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	HRESULT WriteEditorData(LPCTSTR lpszPathName, CBDynBuffer* Buffer);
	bool QueryProjectRoot(CString Filename="");
	CString m_ProjectRoot;

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSpriteDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPRITEDOC_H__E9585F32_985C_4BE8_AA5E_AFE42D780285__INCLUDED_)
