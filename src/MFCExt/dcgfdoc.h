#if !defined(AFX_DCGFDOC_H__CD03E370_77E0_4CEC_B0F3_6514EB9C2B84__INCLUDED_)
#define AFX_DCGFDOC_H__CD03E370_77E0_4CEC_B0F3_6514EB9C2B84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DCGFDoc.h : header file
//

#ifdef DCGF_ADVENTURE
	#include "../engine_core/wme_ad/dcgf_ad.h"
#else
	#include "../engine_core/wme_base/dcgf.h"
#endif


/////////////////////////////////////////////////////////////////////////////
// CDCGFDoc document

class CDCGFDoc : public CDocument
{
protected:
	CDCGFDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDCGFDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDCGFDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	HRESULT WriteEditorData(LPCTSTR lpszPathName, CBDynBuffer* Buffer);
	bool QueryProjectRoot(CString Filename="");
	CString m_ProjectRoot;
	virtual ~CDCGFDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDCGFDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DCGFDOC_H__CD03E370_77E0_4CEC_B0F3_6514EB9C2B84__INCLUDED_)
