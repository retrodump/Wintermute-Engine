// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "AdObject3D.h"


class CAdAttach3DX : public CAdObject3D
{
public:
	DECLARE_PERSISTENT(CAdAttach3DX, CAdObject3D);

	CAdAttach3DX(CBGame* inGame, CBObject* Owner);
	virtual ~CAdAttach3DX(void);

	HRESULT Init(char* ModelFile, char* Name, char* ParentBone);
	HRESULT Update();
	HRESULT Display(D3DXMATRIX* ViewMat, bool RegisterObjects);
	HRESULT DisplayShadowVol(D3DXMATRIX* ModelMat, D3DXVECTOR3* Light, float ExtrusionDepth, bool Update);
	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();

	char* GetParentBone();


	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	virtual char* ScToString();

private:
	CBObject* m_Owner;
	char* m_ParentBone;
};
