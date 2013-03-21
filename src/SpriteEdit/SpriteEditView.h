// SpriteEditView.h : interface of the CSpriteEditView class
//


#pragma once


class CSpriteEditView : public CView
{
protected: // create from serialization only
	CSpriteEditView();
	DECLARE_DYNCREATE(CSpriteEditView)

// Attributes
public:
	CSpriteEditDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CSpriteEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SpriteEditView.cpp
inline CSpriteEditDoc* CSpriteEditView::GetDocument() const
   { return reinterpret_cast<CSpriteEditDoc*>(m_pDocument); }
#endif

