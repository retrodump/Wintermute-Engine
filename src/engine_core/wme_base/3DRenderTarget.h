// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BBase.h"

#ifdef WME_D3D9


class C3DRenderTarget : public CBBase
{
public:
	C3DRenderTarget(CBGame* inGame);
	~C3DRenderTarget();


	HRESULT CreateRenderColorTarget(D3DFORMAT format, int width, int height, int mipLevels);
	HRESULT CreateRenderDepthTarget(D3DFORMAT format, int width, int height);

	void SetClearColor(D3DCOLOR val);
	void SetClearDepth(float val);

	HRESULT Activate();
	LPDIRECT3DTEXTURE GetTexture() { return m_RenderTargetTexture; }

	bool IsDepthTarget() { return m_IsDepthTarget; }

	HRESULT InvalidateDeviceObjects();
	HRESULT RestoreDeviceObjects();


private:
	bool m_IsDepthTarget;

	bool m_ShouldClearColor;
	bool m_ShouldClearDepth;

	float m_ClearDepth;
	D3DCOLOR m_ClearColor;

	D3DFORMAT m_Format;
	int m_Width;
	int m_Height;
	int m_MipLevels;

	LPDIRECT3DTEXTURE m_RenderTargetTexture;
	LPDIRECT3DSURFACE m_RenderTargetSurface;
};

#endif