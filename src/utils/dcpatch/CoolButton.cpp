// CoolButton.cpp : implementation file
//

#include "stdafx.h"
#include "dcpatch.h"
#include "CoolButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCoolButton

IMPLEMENT_DYNAMIC(CCoolButton, CButton)


//////////////////////////////////////////////////////////////////////////
CCoolButton::CCoolButton()
{
	m_bPushed			= FALSE;
	m_bClickiedInside	= FALSE;
	m_bHighlighted		= FALSE;
	m_bCaptured			= FALSE;
}


//////////////////////////////////////////////////////////////////////////
CCoolButton::~CCoolButton()
{
	m_BitmapN.DeleteObject();
	m_BitmapP.DeleteObject();
	m_BitmapH.DeleteObject();
}


BEGIN_MESSAGE_MAP(CCoolButton, CButton)
	//{{AFX_MSG_MAP(CCoolButton)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoolButton message handlers


//////////////////////////////////////////////////////////////////////////
void CCoolButton::PreSubclassWindow() 
{
	ModifyStyle (BS_DEFPUSHBUTTON, BS_OWNERDRAW);	
	CButton::PreSubclassWindow();
}


//////////////////////////////////////////////////////////////////////////
BOOL CCoolButton::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= BS_OWNERDRAW;
	cs.style &= ~BS_DEFPUSHBUTTON;
	
	return CButton::PreCreateWindow(cs);
}


//////////////////////////////////////////////////////////////////////////
BOOL CCoolButton::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
void CCoolButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	ASSERT (lpDIS != NULL);
	ASSERT (lpDIS->CtlType == ODT_BUTTON);

	CDC* pDC = CDC::FromHandle (lpDIS->hDC);
	ASSERT_VALID (pDC);

	if(m_bPushed) m_BitmapP.Draw(*pDC);
	else if(m_bHighlighted) m_BitmapH.Draw(*pDC);
	else m_BitmapN.Draw(*pDC);
}

//////////////////////////////////////////////////////////////////////////
void CCoolButton::SetBitmaps(int Normal, int Pushed, int Hover, int X, int Y)
{
	VERIFY(m_BitmapN.Load(AfxGetResourceHandle(), Normal));
	VERIFY(m_BitmapP.Load(AfxGetResourceHandle(), Pushed));
	VERIFY(m_BitmapH.Load(AfxGetResourceHandle(), Hover));

	CSize size = m_BitmapN.GetSize();
	if(X==0 && Y==0)
		SetWindowPos(NULL, 0,0, size.cx, size.cy, SWP_NOMOVE | SWP_NOZORDER);
	else
		SetWindowPos(NULL, X, Y, size.cx, size.cy, SWP_NOZORDER);
/*
	HRGN region = ::BitmapToRegion((HBITMAP)m_BitmapN, RGB(255, 0, 255));
	if(region){
		SetWindowRgn(region, TRUE);
	}
*/	
}


//////////////////////////////////////////////////////////////////////////
void CCoolButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_bHover = FALSE;
	
	BOOL bRedraw = FALSE;
	
	CRect rectClient;
	GetClientRect (rectClient);
	
	if (rectClient.PtInRect (point))
	{
		m_bHover = TRUE;
		
		if (!m_bHighlighted)
		{
			m_bHighlighted = TRUE;
			bRedraw = TRUE;
		}
		
		if ((nFlags & MK_LBUTTON) && !m_bPushed && m_bClickiedInside)
		{
			m_bPushed = TRUE;
			bRedraw = TRUE;
		}
		
		if (!m_bCaptured)
		{
			SetCapture ();
			m_bCaptured = TRUE;
			bRedraw = TRUE;
		}
	}
	else
	{
		if (nFlags & MK_LBUTTON)
		{
			if (m_bPushed)
			{
				m_bPushed = FALSE;
				bRedraw = TRUE;
			}
		}
		else if (m_bHighlighted)
		{
			m_bHighlighted = FALSE;
			bRedraw = TRUE;
		}
		
		if (m_bCaptured && (!nFlags & MK_LBUTTON))
		{
			ReleaseCapture ();
			m_bCaptured = FALSE;
			
			bRedraw = TRUE;
		}
	}
	
	if (bRedraw)
	{
		Invalidate ();
		UpdateWindow ();
	}
	
	
	CButton::OnMouseMove(nFlags, point);
}


//////////////////////////////////////////////////////////////////////////
void CCoolButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bPushed = TRUE;
	m_bClickiedInside = TRUE;
	m_bHighlighted = TRUE;

	if (!m_bCaptured)
	{
		SetCapture ();
		m_bCaptured = TRUE;
	}

	Invalidate ();
	UpdateWindow ();
	PlaySound(MAKEINTRESOURCE(IDR_BUTTON_SOUND), AfxGetInstanceHandle(), SND_ASYNC | SND_RESOURCE);

	CButton::OnLButtonDown(nFlags, point);
}


//////////////////////////////////////////////////////////////////////////
void CCoolButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bPushed = FALSE;
	m_bClickiedInside = FALSE;
	m_bHighlighted = FALSE;

	Invalidate ();
	UpdateWindow ();

	CButton::OnLButtonUp(nFlags, point);

	if (m_bCaptured)
	{
		ReleaseCapture ();
		m_bCaptured = FALSE;
	}
}


//////////////////////////////////////////////////////////////////////////
void CCoolButton::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	m_bPushed = TRUE;
	m_bClickiedInside = TRUE;
	m_bHighlighted = TRUE;

	Invalidate ();
	UpdateWindow ();

	CButton::OnLButtonDblClk(nFlags, point);
}


//////////////////////////////////////////////////////////////////////////
void CCoolButton::OnEnable(BOOL bEnable) 
{
	if (!bEnable){
		// control disabled
		m_bPushed = FALSE;
		m_bClickiedInside = FALSE;
		m_bHighlighted = FALSE;
		
		if (m_bCaptured)
		{
			ReleaseCapture ();
			m_bCaptured = FALSE;
		}
	}
	
	CButton::OnEnable(bEnable);	
}
