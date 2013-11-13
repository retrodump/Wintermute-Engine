// SpriteView.h : interface of the CSpriteView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPRITEVIEW_H__BC340CB4_A2E4_497C_BB57_069D8220EBFB__INCLUDED_)
#define AFX_SPRITEVIEW_H__BC340CB4_A2E4_497C_BB57_069D8220EBFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "dcgf.h"
#include "SpriteDoc.h"
#include "../mfc_shared/UndoState.h"


class CSpriteView : public CScrollView
{
protected: // create from serialization only
	CSpriteView();
	DECLARE_DYNCREATE(CSpriteView)

// Attributes
public:
	CBSprite* m_Background;
	int m_LastSelectedFrame;
	CSpriteDoc* GetDocument();
	CBGame* Game;
	CBSprite* m_Sprite;
	CArray<CUndoState*, CUndoState*> m_UndoStates;
	int m_UndoPointer;
	bool m_Dragging;
	CPoint m_DragFrom;
	CPoint m_DragOrigin;
	bool m_DragMove;

	afx_msg void OnDisplayFPS(CCmdUI * pCmdUI);
	afx_msg void OnDisplayMode(CCmdUI * pCmdUI);
	afx_msg void OnDisplayCursor(CCmdUI * pCmdUI);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpriteView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	void GetScrollSize(CSize& Size);
	HRESULT ReloadBackground();
	void Stop();
	void Play();
	bool m_Playing;
	void UpdateDrag(CPoint Origin, CPoint Current);
	void EndDrag();
	CRect m_HotRect;
	CBObject* m_Object;
	bool m_CumulateMove;
	void DrawPoint(int X, int Y, DWORD Color);
	void DrawTracker(int X, int Y);
	void SetExpand(CBSubFrame* Sub, bool Expand);
	void GetSelectedObjects();
	void UpdateSelectionState();
	int SelectSubframe(CBSubFrame* Sub);
	CBSubFrame* m_SelectedSubframe;
	bool m_ViewReady;
	void Undo();
	HRESULT RenderFrame();
	void SetDirty(bool IsDirty=true);
	HRESULT SetUndoState(CString Name);
	void PurgeLastUndo();
	virtual ~CSpriteView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSpriteView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnToolsRefresh();
	afx_msg void OnViewBackground();
	afx_msg void OnUpdateViewBackground(CCmdUI* pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SpriteView.cpp
inline CSpriteDoc* CSpriteView::GetDocument()
   { return (CSpriteDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPRITEVIEW_H__BC340CB4_A2E4_497C_BB57_069D8220EBFB__INCLUDED_)
