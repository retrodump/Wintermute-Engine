// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BSURFACEDD_H__C970C963_E3BD_4300_AF67_38CBF763B54D__INCLUDED_)
#define AFX_BSURFACEDD_H__C970C963_E3BD_4300_AF67_38CBF763B54D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BSurface.h"
#include "rgbpixel.h"


class CBSurfaceDD : public CBSurface  
{
public:
	bool m_Child;	
	RECT m_Rect;
	CBSurfaceDD** m_Tiles;
	int m_NumTiles;
	int m_NumRows;
	int m_NumCols;
	int m_TileWidth;
	int m_TileHeight;
	HRESULT ReleaseTiles();

	HRESULT CreateEmptySurface(int Width, int Height);
	bool m_RestoreNeeded;
	virtual HRESULT PrepareToDraw();
	virtual HRESULT Invalidate();
	virtual bool IsTransparentAtLite(int X, int Y);
	virtual HRESULT StartPixelOp();
	virtual HRESULT EndPixelOp();
	virtual HRESULT PutPixel(int X, int Y, BYTE R, BYTE G, BYTE B, int A=-1);
	virtual HRESULT GetPixel(int X, int Y, BYTE* R, BYTE* G, BYTE* B, BYTE* A=NULL);
	virtual bool ComparePixel(int X, int Y, BYTE R, BYTE G, BYTE B, int A=-1);
	CBSurfaceDD(CBGame* inGame);
	virtual ~CBSurfaceDD();

	virtual HRESULT DisplayHalfTrans(int X, int Y, RECT rect);
	virtual bool IsTransparentAt(int X, int Y);
	virtual HRESULT DisplayTransZoom(int X, int Y, RECT rect, float ZoomX, float ZoomY, DWORD Alpha=0xFFFFFFFF, TSpriteBlendMode BlendMode=BLEND_NORMAL, bool MirrorX=false, bool MirrorY=false);
	virtual HRESULT DisplayTrans(int X, int Y, RECT rect, DWORD Alpha=0xFFFFFFFF, TSpriteBlendMode BlendMode=BLEND_NORMAL, bool MirrorX=false, bool MirrorY=false);
	virtual HRESULT Display(int X, int Y, RECT rect, TSpriteBlendMode BlendMode=BLEND_NORMAL, bool MirrorX=false, bool MirrorY=false);
	virtual HRESULT DisplayZoom(int X, int Y, RECT rect, float ZoomX, float ZoomY, DWORD Alpha=0xFFFFFFFF, bool Transparent=false, TSpriteBlendMode BlendMode=BLEND_NORMAL, bool MirrorX=false, bool MirrorY=false);
	virtual HRESULT Restore();
	virtual HRESULT Create(char* Filename, bool default_ck, BYTE ck_red, BYTE ck_green, BYTE ck_blue, int LifeTime=-1, bool KeepLoaded=false);

	HRESULT SetTransDefault();
	HRESULT SetTrans(BYTE r, BYTE g, BYTE b);
	HRESULT Lock();
	HRESULT Unlock();
	HRESULT LoadFile(char* Filename);
	HRESULT GetSurfaceDesc();
	
	DDSURFACEDESC2 m_SurfaceDesc;	
	bool m_Locked;
	DWORD m_TransNative;
	RGBPixel m_TransRGB;
	LPDIRECTDRAWSURFACE7 m_Surface;
	BYTE m_TransIndex;
	bool m_TransDefault;
	virtual HRESULT Create(int Width, int Height);

	DWORD m_RedMask;
	DWORD m_RedBits;
	DWORD m_RedShift;

	DWORD m_GreenMask;
	DWORD m_GreenBits;
	DWORD m_GreenShift;

	DWORD m_BlueMask;
	DWORD m_BlueBits;
	DWORD m_BlueShift;

	DWORD m_BytesPerPixel;

	HRESULT GetBitMaskInfo(DWORD dwBitMask, DWORD* pdwShift, DWORD* pdwBits);

};

#endif // !defined(AFX_BSURFACEDD_H__C970C963_E3BD_4300_AF67_38CBF763B54D__INCLUDED_)
