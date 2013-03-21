// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADOBJECT3D_H__E44ABE3A_864D_40AC_9A60_7F547FCA2B73__INCLUDED_)
#define AFX_ADOBJECT3D_H__E44ABE3A_864D_40AC_9A60_7F547FCA2B73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AdObject.h"

class CAdObject3D : public CAdObject
{
public:
	HRESULT SetupLights();
	HRESULT Convert3DTo2D(D3DXMATRIX* WorldMat, int* PosX, int* PosY);
	HRESULT SkipTo(int X, int Y, bool Tolerant = false);
	char* m_TempSkelAnim;
	D3DXVECTOR3 m_LastPosVector;
	DECLARE_PERSISTENT(CAdObject3D, CAdObject);
	bool m_DropToFloor;
	virtual HRESULT Display();
	virtual HRESULT Update();
	CAdObject3D(CBGame* inGame);
	virtual ~CAdObject3D();
	HRESULT GetBonePosition2D(char* BoneName, int* X, int* Y);
	HRESULT GetBonePosition3D(char* BoneName, D3DXVECTOR3* Pos, D3DXVECTOR3* Offset = NULL);
		
	float m_Velocity;
	float m_AngVelocity;	

	DWORD m_AmbientLightColor;
	bool m_HasAmbientLightColor;
	
	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	virtual char* ScToString();

	C3DShadowVolume* GetShadowVolume();
private:
	C3DShadowVolume* m_ShadowVolume;
	CBArray<char*, char*> m_IgnoredLights;
	void ClearIgnoredLights();
	HRESULT AddIgnoredLight(char* LightName);
	HRESULT RemoveIgnoredLight(char* LightName);
};

#endif // !defined(AFX_ADOBJECT3D_H__E44ABE3A_864D_40AC_9A60_7F547FCA2B73__INCLUDED_)
