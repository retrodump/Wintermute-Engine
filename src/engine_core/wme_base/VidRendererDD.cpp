// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "VidRendererDD.h"



//////////////////////////////////////////////////////////////////////////
CVidRendererDD::CVidRendererDD(CBGame* inGame):CVidRenderer(inGame)
{

}



//////////////////////////////////////////////////////////////////////////
CVidRendererDD::~CVidRendererDD()
{

}


//////////////////////////////////////////////////////////////////////////
HRESULT CVidRendererDD::ProcessFrame(LPBITMAPINFOHEADER Frame)
{
	CBSurfaceDD* surf = (CBSurfaceDD*)m_Surface;

	LONG BmpPitch = Frame->biWidth * (Frame->biBitCount >> 3);

	surf->Lock();
	
	BYTE* BmpBuffer = (BYTE*)Frame+Frame->biSize+Frame->biClrUsed * sizeof(RGBQUAD);
	BYTE* TxtBuffer = (BYTE*)surf->m_SurfaceDesc.lpSurface;

	// bitmap is bottom up...
	BmpBuffer += BmpPitch * (Frame->biHeight - 1);

	RenderFrame(BmpBuffer, TxtBuffer, BmpPitch, surf->m_SurfaceDesc.lPitch, Frame->biWidth, Frame->biHeight);

	surf->Unlock();

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CVidRendererDD::ProcessFrameSlow(LPBITMAPINFOHEADER Frame)
{
	LPDIRECTDRAWSURFACE7 pDestSurface = ((CBSurfaceDD*)m_Surface)->m_Surface;

	LPBITMAPINFO pBmpInfo = (LPBITMAPINFO)Frame;


	LPBITMAPINFOHEADER pBmpInfoHeader = (LPBITMAPINFOHEADER)pBmpInfo;

	HDC hdc;
	pDestSurface->GetDC(&hdc);

	LPBYTE pBmpBits = ((LPBYTE)pBmpInfo) + pBmpInfo->bmiHeader.biSize;

	LPBYTE pGDIBits;
	HBITMAP hBmp = CreateDIBSection(hdc, pBmpInfo, DIB_RGB_COLORS, (LPVOID*)&pGDIBits, NULL, 0);

	memcpy(pGDIBits, pBmpBits, pBmpInfoHeader->biSizeImage);

	HDC hBmpDC = CreateCompatibleDC(hdc);

	HGDIOBJ pOldObj = SelectObject(hBmpDC, hBmp);
	BitBlt(hdc, 0, 0, pBmpInfoHeader->biWidth, pBmpInfoHeader->biHeight, hBmpDC, 0, 0, SRCCOPY);

	if(pOldObj) SelectObject(hBmpDC, pOldObj);
	DeleteObject(hBmp);

	DeleteDC(hBmpDC);

	pDestSurface->ReleaseDC(hdc);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CVidRendererDD::GetTargetFormat(LPBITMAPV4HEADER Format)
{
	CBSurfaceDD* surf = (CBSurfaceDD*)m_Surface;

	Format->bV4BitCount = (WORD)surf->m_SurfaceDesc.ddpfPixelFormat.dwRGBBitCount;
	Format->bV4V4Compression = BI_BITFIELDS;
	
	if(Format->bV4BitCount==24) Format->bV4V4Compression = BI_RGB;

	Format->bV4ClrUsed = 0;
	Format->bV4RedMask = surf->m_SurfaceDesc.ddpfPixelFormat.dwRBitMask;
	Format->bV4GreenMask = surf->m_SurfaceDesc.ddpfPixelFormat.dwGBitMask;
	Format->bV4BlueMask  = surf->m_SurfaceDesc.ddpfPixelFormat.dwBBitMask;
	Format->bV4AlphaMask = surf->m_SurfaceDesc.ddpfPixelFormat.dwRGBAlphaBitMask;
	Format->bV4SizeImage = ((Format->bV4Width+3)&0xFFFFFFFC)*Format->bV4Height * (Format->bV4BitCount>>3);

	return S_OK;
}
