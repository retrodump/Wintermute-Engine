// ProjectView.h : interface of the CProjectView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJECTVIEW_H__59417E41_2166_424F_89D4_D81CCD982396__INCLUDED_)
#define AFX_PROJECTVIEW_H__59417E41_2166_424F_89D4_D81CCD982396__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "../MFCExt/DCGFView.h"
#include "ProjectDoc.h"

class CViewHint;
class CViewLog;
class CViewProps;
class CViewTree;
class CProjectDoc;

class CProjectView : public CDCGFView
{
protected: // create from serialization only
	CProjectView();
	DECLARE_DYNCREATE(CProjectView)

// Attributes
public:
	CProjectDoc* GetDocument();

	CViewHint*  m_ViewHint;     
	CViewLog*   m_ViewLog;   
	CViewProps* m_ViewProps;
	CViewTree*  m_ViewTree;

	CAdScene* m_Scene;
	CAdActor* m_Actor;
	CAdEntity* m_Entity;
	CBSprite* m_Sprite;
	CUIWindow* m_Window;
	CAdActor3D* m_Actor3D;
	CAdActor3DX* m_Actor3DX;
	CBFont* m_Font;
	CBSound* m_Sound;
	CString m_Filename;
	CString m_Message;
	bool m_SoundReady;

	CString m_TalkFile;
	bool m_TalkReady;

	bool m_InitialRefreshDone;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	static void _stdcall EngineLogCallback(char* Text, HRESULT Result, void* Data);
	CString m_SampleText;
	DWORD m_LastActorTime;
	CPoint m_SpriteHotspot;
	void GetSpriteHotspot(CBSprite* Sprite, LONG* X, LONG* Y);
	CString m_ScheduledFilename;
	bool m_ThreadWorking;
	static UINT LoaderThread(LPVOID Param);
	CRect m_Rect;
	HRESULT OnDrawContent();
	void Cleanup();
	void SelectFile(CString Filename);
	void OnPackageChange(CString Package="");
	HRESULT PreviewFile(CString Filename);
	virtual ~CProjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CProjectView)
	afx_msg void OnProjectSettings();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ProjectView.cpp
inline CProjectDoc* CProjectView::GetDocument()
   { return (CProjectDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTVIEW_H__59417E41_2166_424F_89D4_D81CCD982396__INCLUDED_)
