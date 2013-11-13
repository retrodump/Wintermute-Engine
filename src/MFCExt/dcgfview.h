#if !defined(AFX_DCGFVIEW_H__94D2CED5_3B4A_42F2_88B5_012BBFDB2FAD__INCLUDED_)
#define AFX_DCGFVIEW_H__94D2CED5_3B4A_42F2_88B5_012BBFDB2FAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DCGFView.h : header file
//

#ifdef DCGF_ADVENTURE
	#include "../engine_core/wme_ad/dcgf_ad.h"
#else
	#include "../engine_core/wme_base/dcgf.h"
#endif

#include "UndoState.h"
#include "BCMenu.h"
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
	HRESULT RenderFrame();

	bool m_ViewReady;

// Operations
public:
	virtual HRESULT OnDrawContent();
	virtual bool ProcessUndoBuffer(BYTE* buf);
	virtual bool FillUndoBuffer(CBDynBuffer* buf);
	BCMenu m_PopupMenu;
	BOOL ShowPopupMenu(int MenuID, int ToolbarID=-1, LPRECT Rect=NULL);
	bool m_RenderingDisabled;

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
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DCGFVIEW_H__94D2CED5_3B4A_42F2_88B5_012BBFDB2FAD__INCLUDED_)
