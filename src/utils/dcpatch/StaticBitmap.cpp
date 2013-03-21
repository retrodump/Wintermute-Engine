// StaticBitmap.cpp : implementation file
//

#include "stdafx.h"
#include "dcpatch.h"
#include "StaticBitmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CStaticBitmap, CStatic)

BEGIN_MESSAGE_MAP(CStaticBitmap, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//////////////////
// Constructor: Load DIB from resource
//
CStaticBitmap::CStaticBitmap()
{
}

//////////////////
// Paint the DIB. Should really take into account
// all the styles here (sunken, raised, etc.)
//
void CStaticBitmap::OnPaint()
{
	CPaintDC dc(this);
	m_dib.Draw(dc);
}

//////////////////
// Dialog is dead: destroy bitmap.
// This is required in case programmer uses a static dialog
//
void CStaticBitmap::PostNcDestroy()
{
	m_dib.DeleteObject();
}


//////////////////////////////////////////////////////////////////////////
BOOL CStaticBitmap::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}
