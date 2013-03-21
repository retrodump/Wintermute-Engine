// SceneView.h : interface of the CSceneView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCENEVIEW_H__3AEEF10C_52BA_4EFD_802A_9FFF567186CE__INCLUDED_)
#define AFX_SCENEVIEW_H__3AEEF10C_52BA_4EFD_802A_9FFF567186CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "dcgf_ad.h"
#include "HotObject.h"
#include "SceneDoc.h"
#include "../mfc_shared/UndoState.h"


class CSceneView : public CScrollView
{
protected: // create from serialization only
	CSceneView();
	DECLARE_DYNCREATE(CSceneView)

// Attributes
public:
	CSceneDoc* GetDocument();
	CAdGame* Game;
	CArray<CHotObject*, CHotObject*> m_HotObjects;
	CArray<CUndoState*, CUndoState*> m_UndoStates;
	int m_UndoPointer;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSceneView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	void AfterPosChange();
	CAdActor* m_Actor;
	CAdActor3D* m_Actor3D;
	CAdActor3DX* m_Actor3DX;
	CAdScaleLevel* m_SelectedScale;
	CAdRotLevel* m_SelectedRot;
	void DrawScale(CAdScaleLevel* Scale);
	void DrawRot(CAdRotLevel* Rot);
	void PurgeLastUndo();
	void GetSelectedObjects();
	void UpdateSelectionState();
	HRESULT SetUndoState(CString Name);
	void SetDirty(bool IsDirty=true);
	double PointDist(int X1, int Y1, int X2, int Y2, int PX, int PY);
	void AddPointToRegion(int x, int y, CBRegion* Region);
	void RestoreState();
	bool m_RemovingPoint;
	bool m_AddingScale;
	bool m_AddingRot;
	bool m_AddingPoint;
	bool m_PlacingActor;
	void RemovePoint();
	void AddPoint();
	void AddScale();
	void AddRot();
	void UpdateRegions();
	void RegisterAllObjects();
	CPoint m_DragOrigin;
	void UpdateDrag(CPoint Origin, CPoint Current);
	void EndDrag();
	CHotObject* m_DragObject;
	CPoint m_DragFrom;
	bool m_Dragging;
	CHotObject* GetObjectAt(int X, int Y);
	void ClearHotObjects();
	void DrawEntity(CAdSceneNode* Node, CAdEntity* Entity, bool Selected=false);
	int m_GameHeight;
	int m_GameWidth;
	void DrawRegion(CAdSceneNode* Node, CBRegion* Region, bool Selected=false, bool Blocked=false, bool Decor=false);
	void DrawWaypoints(CAdWaypointGroup* Region, bool Selected=false);
	afx_msg void OnDisplayFPS(CCmdUI * pCmdUI);
	afx_msg void OnDisplayMode(CCmdUI * pCmdUI);
	afx_msg void OnDisplayCursor(CCmdUI * pCmdUI);
	CAdLayer* m_SelectedLayer;
	bool m_SelectedWaypoints;
	bool m_SelectedFree;
	CAdObject* m_SelectedFreeObject;
	CAdWaypointGroup* m_SelectedWptGroup;
	CAdSceneNode* m_SelectedNode;
	HRESULT RenderFrame();
	virtual ~CSceneView();
	void Undo();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSceneView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnToolsRefresh();
	afx_msg void OnScaleProperties();
	afx_msg void OnRotProperties();
	afx_msg void OnScaleRemove();
	afx_msg void OnRotRemove();
	afx_msg void OnActorLoad();
	afx_msg void OnActorPlace();
	afx_msg void OnActorRemove();
	afx_msg void OnActorAnim();
	afx_msg void OnUpdateActorRemove(CCmdUI* pCmdUI);
	afx_msg void OnUpdateActorAnim(CCmdUI* pCmdUI);
	afx_msg void OnActorCopyData();
	afx_msg void OnUpdateActorCopyData(CCmdUI* pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnFilePrintPreview();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SceneView.cpp
inline CSceneDoc* CSceneView::GetDocument()
   { return (CSceneDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCENEVIEW_H__3AEEF10C_52BA_4EFD_802A_9FFF567186CE__INCLUDED_)
