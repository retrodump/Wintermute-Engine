// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "3DEffectTexture.h"
#include "3DEffect.h"

#ifdef WME_D3D9

//////////////////////////////////////////////////////////////////////////
C3DEffectTexture::C3DEffectTexture(CBGame* inGame) : CBBase(inGame)
{
	m_Surface = NULL;
	m_Handle = NULL;
}


//////////////////////////////////////////////////////////////////////////
C3DEffectTexture::~C3DEffectTexture()
{
	SAFE_DELETE(m_Surface);
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DEffectTexture::LoadFromFile(const std::string& fileName)
{
	SAFE_DELETE(m_Surface);

	m_Surface = new CBSurfaceD3D(Game);
	HRESULT res = m_Surface->Create(const_cast<char*>(fileName.c_str()), true, 0, 0, 0);
	if (FAILED(res))
	{
		SAFE_DELETE(m_Surface);
	}

	return res;
}


//////////////////////////////////////////////////////////////////////////
LPDIRECT3DTEXTURE C3DEffectTexture::GetTexture()
{
	if (!m_Surface) return NULL;
	else return m_Surface->m_Texture;
}


#endif