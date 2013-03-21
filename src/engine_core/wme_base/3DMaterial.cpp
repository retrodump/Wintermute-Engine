// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "3DMaterial.h"
#include "3DEffect.h"
#include "3DEffectParams.h"
#include "3DEffect_D3D8.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
C3DMaterial::C3DMaterial(CBGame* inGame):CBNamedObject(inGame)
{
	memset(&m_Material, 0, sizeof(m_Material));
	m_TextureFilename = NULL;
	m_Surface = NULL;
	m_OwnedSurface = false;
	m_Sprite = NULL;
	m_Theora = NULL;
	m_Effect = NULL;
	m_Params = NULL;
}


//////////////////////////////////////////////////////////////////////////
C3DMaterial::~C3DMaterial()
{
	SAFE_DELETE_ARRAY(m_TextureFilename);

	if(m_Surface && m_OwnedSurface) Game->m_SurfaceStorage->RemoveSurface(m_Surface);
	m_Sprite = NULL; // ref only
	m_Theora = NULL;
	m_Effect = NULL;
	m_Params = NULL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DMaterial::InvalidateDeviceObjects()
{
	if (m_Effect) return m_Effect->InvalidateDeviceObjects();
	else return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DMaterial::RestoreDeviceObjects()
{
	if (m_Effect) return m_Effect->RestoreDeviceObjects();
	else return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DMaterial::SetTexture(char* Filename, bool AdoptName)
{
	if(AdoptName)
	{
		char FName[_MAX_EXT];
		_splitpath(Filename, NULL, NULL, FName, NULL);
		SetName(FName);
	}

	CBUtils::SetString(&m_TextureFilename, Filename);
	if(m_Surface && m_OwnedSurface) Game->m_SurfaceStorage->RemoveSurface(m_Surface);

	m_Surface = Game->m_SurfaceStorage->AddSurface(m_TextureFilename);
	m_OwnedSurface = true;
	m_Sprite = NULL;

	return (m_Surface?S_OK:E_FAIL);
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DMaterial::SetSprite(CBSprite* Sprite, bool AdoptName)
{
	if(AdoptName)
	{
		char FName[_MAX_EXT];
		_splitpath(Sprite->m_Filename, NULL, NULL, FName, NULL);
		SetName(FName);
	}
	CBUtils::SetString(&m_TextureFilename, Sprite->m_Filename);
	if(m_Surface && m_OwnedSurface) Game->m_SurfaceStorage->RemoveSurface(m_Surface);
	m_Surface = NULL;

	m_Sprite = Sprite;
	m_OwnedSurface = false;
	m_Theora = NULL;

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DMaterial::SetTheora(CVidTheoraPlayer* Theora, bool AdoptName)
{
	if(AdoptName)
	{
		char FName[_MAX_EXT];
		_splitpath(Theora->m_Filename, NULL, NULL, FName, NULL);
		SetName(FName);
	}
	CBUtils::SetString(&m_TextureFilename, Theora->m_Filename);
	if(m_Surface && m_OwnedSurface) Game->m_SurfaceStorage->RemoveSurface(m_Surface);
	m_Surface = NULL;

	m_Theora = Theora;
	m_OwnedSurface = false;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DMaterial::SetEffect(C3DEffect* Effect, C3DEffectParams* Params, bool AdoptName)
{
	if (!Effect)
	{
		m_Effect = NULL;
		m_Params = NULL;
		return S_OK;
	}

	if(AdoptName)
	{
		char FName[_MAX_EXT];
		_splitpath(Effect->GetFileName(), NULL, NULL, FName, NULL);
		SetName(FName);
	}
	CBUtils::SetString(&m_TextureFilename, Effect->GetFileName());

	m_Effect = Effect;
	m_Params = Params;

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
CBSurface* C3DMaterial::GetSurface()
{
	if(m_Theora)
	{
		m_Theora->Update();
		return m_Theora->GetTexture();
	}
	else if(m_Sprite)
	{
		m_Sprite->GetCurrentFrame();
		return m_Sprite->GetSurface();
	}
	else return m_Surface;
}
