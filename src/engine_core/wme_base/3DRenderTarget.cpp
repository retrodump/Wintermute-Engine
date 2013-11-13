// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "3DRenderTarget.h"


#ifdef WME_D3D9

//////////////////////////////////////////////////////////////////////////
C3DRenderTarget::C3DRenderTarget(CBGame* inGame) : CBBase(inGame)
{
	m_RenderTargetTexture = NULL;
	m_RenderTargetSurface = NULL;

	m_IsDepthTarget = false;

	m_ShouldClearColor = m_ShouldClearDepth = false;
	m_ClearColor = 0;
	m_ClearDepth = 0;

	m_Format = D3DFMT_UNKNOWN;
	m_Width = m_Height = m_MipLevels = 0;

}

//////////////////////////////////////////////////////////////////////////
C3DRenderTarget::~C3DRenderTarget()
{
	InvalidateDeviceObjects();
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DRenderTarget::InvalidateDeviceObjects()
{
	RELEASE(m_RenderTargetTexture);
	RELEASE(m_RenderTargetSurface);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DRenderTarget::RestoreDeviceObjects()
{
	if (m_IsDepthTarget) return CreateRenderDepthTarget(m_Format, m_Width, m_Height);
	else return CreateRenderColorTarget(m_Format, m_Width, m_Height, m_MipLevels);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DRenderTarget::CreateRenderColorTarget(D3DFORMAT format, int width, int height, int mipLevels)
{
	HRESULT res;

	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;

	m_IsDepthTarget = false;

	res = D3DXCreateTexture(m_Renderer->m_Device, width, height, mipLevels, D3DUSAGE_RENDERTARGET, format, D3DPOOL_DEFAULT, &m_RenderTargetTexture);
	if (FAILED(res))
	{
		Game->LOG(res, "Error creating render target texture %dx%d", width, height);
		return res;
	}

	m_Format = format;
	m_Width = width;
	m_Height = height;
	m_MipLevels = mipLevels;
	
	return m_RenderTargetTexture->GetSurfaceLevel(0, &m_RenderTargetSurface);
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DRenderTarget::CreateRenderDepthTarget(D3DFORMAT format, int width, int height)
{
	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;

	m_IsDepthTarget = true;

	m_Format = format;
	m_Width = width;
	m_Height = height;

	return m_Renderer->m_Device->CreateDepthStencilSurface(width, height, format, D3DMULTISAMPLE_NONE, 0, FALSE, &m_RenderTargetSurface, NULL);
}


//////////////////////////////////////////////////////////////////////////
void C3DRenderTarget::SetClearColor(D3DCOLOR val)
{
	m_ShouldClearColor = true;
	m_ClearColor = val;
}


//////////////////////////////////////////////////////////////////////////
void C3DRenderTarget::SetClearDepth(float val)
{
	m_ShouldClearDepth = true;
	m_ClearDepth = val;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DRenderTarget::Activate()
{
	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;
	HRESULT res;

	if (m_IsDepthTarget)
	{
		res = m_Renderer->m_Device->SetDepthStencilSurface(m_RenderTargetSurface);
	}
	else
	{
		res = m_Renderer->m_Device->SetRenderTarget(0, m_RenderTargetSurface);
	}

	if (SUCCEEDED(res))
	{
		DWORD flags = 0;
		if (m_ShouldClearColor) flags |= D3DCLEAR_TARGET;
		if (m_ShouldClearDepth) flags |= D3DCLEAR_ZBUFFER;

		if (flags != 0)
			m_Renderer->m_Device->Clear(0, NULL, flags, m_ClearColor, m_ClearDepth, 0L);
	}
	
	return res;
}

#endif
