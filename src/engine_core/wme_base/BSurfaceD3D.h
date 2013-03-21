// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BSURFACED3D_H__0E7DF98A_8A09_44C5_97FC_B284F25987F4__INCLUDED_)
#define AFX_BSURFACED3D_H__0E7DF98A_8A09_44C5_97FC_B284F25987F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BSurface.h"

class CBSurfaceD3D : public CBSurface  
{
public:
	bool m_Child;	
	HRESULT ReleaseTiles();
	RECT m_Rect;
	CBSurfaceD3D** m_Tiles;
	int m_NumTiles;
	int m_NumRows;
	int m_NumCols;
	int m_TileWidth;
	int m_TileHeight;

	virtual HRESULT PrepareToDraw();
	virtual HRESULT Invalidate();
	virtual HRESULT StartPixelOp();
	virtual HRESULT EndPixelOp();
	virtual HRESULT PutPixel(int X, int Y, BYTE R, BYTE G, BYTE B, int A=-1);
	virtual HRESULT GetPixel(int X, int Y, BYTE* R, BYTE* G, BYTE* B, BYTE* A=NULL);
	virtual bool ComparePixel(int X, int Y, BYTE R, BYTE G, BYTE B, int A=-1);
	HRESULT GetTextureFormat();
	virtual HRESULT Create(int Width, int Height);
	virtual bool IsTransparentAt(int X, int Y);
	virtual HRESULT DisplayTransZoom(int X, int Y, RECT rect, float ZoomX, float ZoomY, DWORD Alpha=0xFFFFFFFF, TSpriteBlendMode BlendMode=BLEND_NORMAL, bool MirrorX=false, bool MirrorY=false);
	virtual HRESULT DisplayTrans(int X, int Y, RECT rect, DWORD Alpha=0xFFFFFFFF, TSpriteBlendMode BlendMode=BLEND_NORMAL, bool MirrorX=false, bool MirrorY=false);
	virtual HRESULT Display(int X, int Y, RECT rect, TSpriteBlendMode BlendMode=BLEND_NORMAL, bool MirrorX=false, bool MirrorY=false);
	virtual HRESULT DisplayZoom(int X, int Y, RECT rect, float ZoomX, float ZoomY, DWORD Alpha=0xFFFFFFFF, bool Transparent=false, TSpriteBlendMode BlendMode=BLEND_NORMAL, bool MirrorX=false, bool MirrorY=false);
	virtual HRESULT DisplayTransform(int X, int Y, int HotX, int HotY, RECT Rect, float ZoomX, float ZoomY, DWORD Alpha, float Rotate, TSpriteBlendMode BlendMode=BLEND_NORMAL, bool MirrorX=false, bool MirrorY=false);
	virtual HRESULT Create(char* Filename, bool default_ck, BYTE ck_red, BYTE ck_green, BYTE ck_blue, int LifeTime=-1, bool KeepLoaded=false);
	HRESULT CreateEmptyTexture(int Width, int Height);
	LPDIRECT3DTEXTURE m_Texture;
	CBSurfaceD3D(CBGame* inGame);
	virtual ~CBSurfaceD3D();
	D3DFORMAT m_TextureFormat;
	D3DLOCKED_RECT m_LockedRect;
	D3DSURFACE_DESC m_SurfaceDesc;
	virtual bool IsTransparentAtLite(int X, int Y);
};

#endif // !defined(AFX_BSURFACED3D_H__0E7DF98A_8A09_44C5_97FC_B284F25987F4__INCLUDED_)
