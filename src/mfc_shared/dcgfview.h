#if !defined(AFX_DCGFVIEW_H__94D2CED5_3B4A_42F2_88B5_012BBFDB2FAD__INCLUDED_)
#define AFX_DCGFVIEW_H__94D2CED5_3B4A_42F2_88B5_012BBFDB2FAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DCGFView.h : header file
//

#ifdef DCGF_ADVENTURE
	#include "dcgf_ad.h"
#else
	#include "dcgf.h"
#endif

#include "UndoState.h"

#include "afxtempl.h"

/////////////////////////////////////////////////////////////////////////////
// CDCGFView view

class CDCGFView : public CScrollView
{
protected:
	CDCGFView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDCGFView)

// Attributes
public:

#ifdef DCGF_ADVENTURE
	CAdGame* Game;
#else
	CBGame* Game;
#endif

	CArray<CUndoState*, CUndoState*> m_UndoStates;
	int m_UndoPointer;

	bool Undo();
	HRESULT SetUndoState(CString Name);
	void PurgeLastUndo();

	bool m_ViewReady;

// Operations
public:
	virtual bool ProcessUndoBuffer(BYTE* buf);
	virtual bool FillUndoBuffer(CBDynBuffer* buf);
	virtual HRESULT RenderFrame();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDCGFView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDCGFView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDCGFView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DCGFVIEW_H__94D2CED5_3B4A_42F2_88B5_012BBFDB2FAD__INCLUDED_)
