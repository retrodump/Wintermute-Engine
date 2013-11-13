// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "VidRenderer.h"


//////////////////////////////////////////////////////////////////////////
CVidRenderer::CVidRenderer(CBGame* inGame):CBBase(inGame)
{
	m_Surface = NULL;
}



//////////////////////////////////////////////////////////////////////////
CVidRenderer::~CVidRenderer()
{
	SAFE_DELETE(m_Surface);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CVidRenderer::Display(int X, int Y, float Zoom)
{
	RECT rc;
	if(m_Surface){
		SetRect(&rc, 0, 0, m_Surface->GetWidth(), m_Surface->GetHeight());
		if(Zoom==100.0f) return m_Surface->Display(X, Y, rc);
		else return m_Surface->DisplayZoom(X, Y, rc, Zoom, Zoom);
	}
	else return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CVidRenderer::Initialize(LPBITMAPINFO Format, LPBITMAPV4HEADER TargetFormat)
{	
	SAFE_DELETE(m_Surface);
	if(Game->m_UseD3D)
		m_Surface = new CBSurfaceD3D(Game);
	else
		m_Surface = new CBSurfaceDD(Game);

	if(!m_Surface) return E_FAIL;

	HRESULT res = m_Surface->Create(Format->bmiHeader.biWidth, Format->bmiHeader.biHeight);
	if(FAILED(res)){
		SAFE_DELETE(m_Surface);
		return res;
	}

	GetTargetFormat(TargetFormat);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CVidRenderer::ProcessFrame(LPBITMAPINFOHEADER)
{
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CVidRenderer::ProcessFrameSlow(LPBITMAPINFOHEADER)
{
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CVidRenderer::GetTargetFormat(LPBITMAPV4HEADER Format)
{
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CVidRenderer::RenderFrame(BYTE *SrcBuf, BYTE *TargetBuf, LONG SrcPitch, LONG TargetPitch, LONG Width, LONG Height)
{
	int xs = SrcPitch>>2;
	int xm = SrcPitch*2;
	int y = Height;
	int z = TargetPitch - SrcPitch;

	if(Width%4 != 0){
		for(int i=0; i<(int)Height; i++){
			memcpy(TargetBuf, SrcBuf, SrcPitch);
			TargetBuf += TargetPitch;
			SrcBuf -= SrcPitch;
		}
	}
	else{
		__asm{
			mov ebx, [y]
			mov esi, SrcBuf
			mov edi, TargetBuf
			cld
		
		lp: mov ecx, [xs]
			rep movsd
				
			sub esi, [xm]
			add edi, [z]
				
			dec ebx
			jnz lp
		}
	}
	
	return S_OK;
}
