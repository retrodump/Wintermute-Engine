// SpriteEditView.cpp : implementation of the CSpriteEditView class
//

#include "stdafx.h"
#include "SpriteEdit.h"

#include "SpriteEditDoc.h"
#include "SpriteEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSpriteEditView

IMPLEMENT_DYNCREATE(CSpriteEditView, CView)

BEGIN_MESSAGE_MAP(CSpriteEditView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CSpriteEditView construction/destruction

CSpriteEditView::CSpriteEditView()
{
	// TODO: add construction code here

}

CSpriteEditView::~CSpriteEditView()
{
}

BOOL CSpriteEditView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSpriteEditView drawing

void CSpriteEditView::OnDraw(CDC* /*pDC*/)
{
	CSpriteEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CSpriteEditView printing

BOOL CSpriteEditView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSpriteEditView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSpriteEditView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CSpriteEditView diagnostics

#ifdef _DEBUG
void CSpriteEditView::AssertValid() const
{
	CView::AssertValid();
}

void CSpriteEditView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSpriteEditDoc* CSpriteEditView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSpriteEditDoc)));
	return (CSpriteEditDoc*)m_pDocument;
}
#endif //_DEBUG


// CSpriteEditView message handlers
