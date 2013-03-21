// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BBase.h"

#ifdef WME_D3D9

class C3DEffect;

class C3DEffectTexture : public CBBase
{
public:
	C3DEffectTexture(CBGame* inGame);
	~C3DEffectTexture();

	HRESULT LoadFromFile(const std::string& fileName);
	
	void SetHandle(D3DXHANDLE handle) { m_Handle = handle; }
	D3DXHANDLE GetHandle() { return m_Handle; }

	LPDIRECT3DTEXTURE GetTexture();

private:
	D3DXHANDLE m_Handle;
	CBSurfaceD3D* m_Surface;
};

#endif