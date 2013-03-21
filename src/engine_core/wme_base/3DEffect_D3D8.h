// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BBase.h"


#ifndef WME_D3D9

class C3DEffect : public CBBase
{
public:
	C3DEffect(CBGame* Game);
	~C3DEffect();

	HRESULT CreateFromFile(char* Filename);

	HRESULT InvalidateDeviceObjects() { return E_FAIL; }
	HRESULT RestoreDeviceObjects() { return E_FAIL; }

	char* GetFileName()  { return NULL; }


};


class C3DEffectParams
{
public:
	C3DEffectParams() {};
	~C3DEffectParams() {};

	HRESULT Persist(CBPersistMgr* PersistMgr) { return E_FAIL; }
	void SetEffectParams(ID3DXEffect* effect) {}
	void Clear() {}
	void SetParam(char* ParamName, CScValue* Val) {}
	void SetParam(char* ParamName, D3DXVECTOR4 Val) {}

};

#endif