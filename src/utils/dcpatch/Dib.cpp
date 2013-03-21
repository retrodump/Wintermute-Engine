////////////////////////////////////////////////////////////////
// PixieLib(TM) Copyright 1997-1999 Paul DiLascia
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
// CDib - Device Independent Bitmap.CDib is derived from CBitmap, so
// you can use it with other MFC functions that use bitmaps.
//
#include "StdAfx.h"
#include "Dib.h"
/*#include "DrawTool.h"*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int MAXPALCOLORS = 256;

IMPLEMENT_DYNAMIC(CDib, CObject)

CDib::CDib()
{
	memset(&m_bm, 0, sizeof(m_bm));
	m_hdd = NULL;
}

CDib::~CDib()
{
	DeleteObject();
}

//////////////////
// Delete Object. Delete DIB and palette.
//
BOOL CDib::DeleteObject()
{
	m_pal.DeleteObject();
	if (m_hdd) {
		DrawDibClose(m_hdd);
		m_hdd = NULL;
	}
	memset(&m_bm, 0, sizeof(m_bm));
	return CBitmap::DeleteObject();
}

//////////////////
// Read DIB from file.
//
BOOL CDib::Load(LPCTSTR lpszPathName)
{
	return Attach(::LoadImage(NULL, lpszPathName, IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
}

//////////////////
// Load bitmap resource. Never tested.
//
BOOL CDib::Load(HINSTANCE hInst, LPCTSTR lpResourceName)
{
	return Attach(::LoadImage(hInst, lpResourceName, IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
}

//////////////////
// Attach is just like the CGdiObject version,
// except it also creates the palette
//
BOOL CDib::Attach(HGDIOBJ hbm)
{
	if (CBitmap::Attach(hbm)) {
		if (!GetBitmap(&m_bm))			// load BITMAP for speed
			return FALSE;
		m_pal.DeleteObject();			// in case one is already there
		return CreatePalette(m_pal);	// create palette
	}
	return FALSE;	
}

////////////////////////////////////////////////////////////////
// Draw DIB on caller's DC. Does stretching from source to destination
// rectangles. Generally, you can let the following default to zero/NULL:
//
//		bUseDrawDib = whether to use use DrawDib, default TRUE
//		pPal	      = palette, default=NULL, (use DIB's palette)
//		bForeground = realize in foreground (default FALSE)
//
// If you are handling palette messages, you should use bForeground=FALSE,
// since you will realize the foreground palette in WM_QUERYNEWPALETTE.
//
BOOL CDib::Draw(CDC& dc, const CRect* rcDst, const CRect* rcSrc,
	BOOL bUseDrawDib, CPalette* pPal, BOOL bForeground)
{
	if (!m_hObject)
		return FALSE;

	// Select, realize palette
	if (pPal==NULL)				// no palette specified:
		pPal = GetPalette();		// use default
	CPalette* pOldPal = dc.SelectPalette(pPal, !bForeground);
	dc.RealizePalette();

	BOOL bRet = FALSE;
	if (bUseDrawDib) {
		// Compute rectangles where NULL specified
		//
		CRect rc(0,0,-1,-1);	// default for DrawDibDraw
		if (!rcSrc)
			rcSrc = &rc;
		if (!rcDst)
			rcDst=rcSrc;
		if (!m_hdd)
			VERIFY(m_hdd = DrawDibOpen());

		// Get BITMAPINFOHEADER/color table. I copy into stack object each time.
		// This doesn't seem to slow things down visibly.
		//
		DIBSECTION ds;
		VERIFY(GetObject(sizeof(ds), &ds)==sizeof(ds));
		char buf[sizeof(BITMAPINFOHEADER) + MAXPALCOLORS*sizeof(RGBQUAD)];
		BITMAPINFOHEADER& bmih = *(BITMAPINFOHEADER*)buf;
		RGBQUAD* colors = (RGBQUAD*)(&bmih+1);
		memcpy(&bmih, &ds.dsBmih, sizeof(bmih));
		GetColorTable(colors, MAXPALCOLORS);

		// Let DrawDib do the work!
		bRet = DrawDibDraw(m_hdd, dc,
			rcDst->left, rcDst->top, rcDst->Width(), rcDst->Height(),
			&bmih,			// ptr to BITMAPINFOHEADER + colors
			m_bm.bmBits,	// bits in memory
			rcSrc->left, rcSrc->top, rcSrc->Width(), rcSrc->Height(),
			bForeground ? 0 : DDF_BACKGROUNDPAL);

	} else {
		// use normal draw function
		bRet = PLDrawBitmap(dc, this, rcDst, rcSrc);
	}
	if (pOldPal)
		dc.SelectPalette(pOldPal, TRUE);
	return bRet;
}

#define PALVERSION 0x300	// magic number for LOGPALETTE

//////////////////
// Create the palette. Use halftone palette for hi-color bitmaps.
//
BOOL CDib::CreatePalette(CPalette& pal)
{ 
	// should not already have palette
	ASSERT(pal.m_hObject==NULL);

	BOOL bRet = FALSE;
	RGBQUAD* colors = new RGBQUAD[MAXPALCOLORS];
	UINT nColors = GetColorTable(colors, MAXPALCOLORS);
	if (nColors > 0) {
		// Allocate memory for logical palette 
		int len = sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * nColors;
		LOGPALETTE* pLogPal = (LOGPALETTE*)new char[len];
		if (!pLogPal)
			return NULL;

		// set version and number of palette entries
		pLogPal->palVersion = PALVERSION;
		pLogPal->palNumEntries = nColors;

		// copy color entries 
		for (UINT i = 0; i < nColors; i++) {
			pLogPal->palPalEntry[i].peRed   = colors[i].rgbRed;
			pLogPal->palPalEntry[i].peGreen = colors[i].rgbGreen;
			pLogPal->palPalEntry[i].peBlue  = colors[i].rgbBlue;
			pLogPal->palPalEntry[i].peFlags = 0;
		}

		// create the palette and destroy LOGPAL
		bRet = pal.CreatePalette(pLogPal);
		delete [] (char*)pLogPal;
	} else {
		CWindowDC dcScreen(NULL);
		bRet = pal.CreateHalftonePalette(&dcScreen);
	}
	delete colors;
	return bRet;
}

//////////////////
// Helper to get color table. Does all the mem DC voodoo.
//
UINT CDib::GetColorTable(RGBQUAD* colorTab, UINT nColors)
{
	CWindowDC dcScreen(NULL);
	CDC memdc;
	memdc.CreateCompatibleDC(&dcScreen);
	CBitmap* pOldBm = memdc.SelectObject(this);
	nColors = GetDIBColorTable(memdc, 0, nColors, colorTab);
	memdc.SelectObject(pOldBm);
	return nColors;
}

//////////////////
// Get size (width, height) of bitmap.
// extern fn works for ordinary CBitmap objects.
//
CSize PLGetBitmapSize(CBitmap* pBitmap)
{
	BITMAP bm;
	return pBitmap->GetBitmap(&bm) ?
		CSize(bm.bmWidth, bm.bmHeight) : CSize(0,0);
}

//////////////////
// You can use this static function to draw ordinary
// CBitmaps as well as CDibs
//
BOOL PLDrawBitmap(CDC& dc, CBitmap* pBitmap,
	const CRect* rcDst, const CRect* rcSrc, DWORD dwRop)
{
	// Compute rectangles where NULL specified
	CRect rc;
	if (!rcSrc) {
		// if no source rect, use whole bitmap
		rc = CRect(CPoint(0,0), PLGetBitmapSize(pBitmap));
		rcSrc = &rc;
	}
	if (!rcDst) {
		// if no destination rect, use source
		rcDst=rcSrc;
	}

	// Create memory DC
	CDC memdc;
	memdc.CreateCompatibleDC(&dc);
	CBitmap* pOldBm = memdc.SelectObject(pBitmap);

	// Blast bits from memory DC to target DC.
	// Use StretchBlt if size is different.
	//
	BOOL bRet = FALSE;
	if (rcDst->Size()==rcSrc->Size()) {
		bRet = dc.BitBlt(rcDst->left, rcDst->top, 
			rcDst->Width(), rcDst->Height(),
			&memdc, rcSrc->left, rcSrc->top, dwRop);
	} else {
		dc.SetStretchBltMode(COLORONCOLOR);
		bRet = dc.StretchBlt(rcDst->left, rcDst->top, rcDst->Width(),
			rcDst->Height(), &memdc, rcSrc->left, rcSrc->top, rcSrc->Width(),
			rcSrc->Height(), dwRop);
	}
	memdc.SelectObject(pOldBm);

	return bRet;
}


