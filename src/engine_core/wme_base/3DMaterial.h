// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_3DMATERIAL_H__633126D3_1220_46A0_A1B3_34B531E858D6__INCLUDED_)
#define AFX_3DMATERIAL_H__633126D3_1220_46A0_A1B3_34B531E858D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class C3DEffect;
class C3DEffectParams;

class C3DMaterial : public CBNamedObject
{	
public:
	C3DMaterial(CBGame* inGame);
	virtual ~C3DMaterial();
	D3DMATERIAL m_Material;

	HRESULT SetTexture(char* Filename, bool AdoptName=false);
	HRESULT SetSprite(CBSprite* Sprite, bool AdoptName=false);
	HRESULT SetTheora(CVidTheoraPlayer* Theora, bool AdoptName=false);
	CBSurface* GetSurface();
	HRESULT SetEffect(C3DEffect* Effect, C3DEffectParams* Params, bool AdoptName=false);

	C3DEffect* GetEffect() { return m_Effect; }
	C3DEffectParams* GetEffectParams() { return m_Params; }

	HRESULT InvalidateDeviceObjects();
	HRESULT RestoreDeviceObjects();

private:
	char* m_TextureFilename;	
	CBSurface* m_Surface;
	bool m_OwnedSurface;
	CBSprite* m_Sprite;
	CVidTheoraPlayer* m_Theora;
	C3DEffect* m_Effect;
	C3DEffectParams* m_Params;
};

#endif // !defined(AFX_3DMATERIAL_H__633126D3_1220_46A0_A1B3_34B531E858D6__INCLUDED_)
