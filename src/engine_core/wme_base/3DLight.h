// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_3DLIGHT_H__896A01D9_84A0_47E2_AD30_88B05F3FFD9C__INCLUDED_)
#define AFX_3DLIGHT_H__896A01D9_84A0_47E2_AD30_88B05F3FFD9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class C3DLight : public CBScriptable
{
public:
	HRESULT Persist(CBPersistMgr* PersistMgr);
	HRESULT GetViewMatrix(D3DXMATRIX *ViewMatrix);
	C3DLight(CBGame* inGame);
	virtual ~C3DLight();
	D3DCOLOR m_DiffuseColor;
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Target;
	bool m_IsSpotlight;
	bool m_Active;
	float m_Falloff;
	
	float m_Distance;
	bool m_IsAvailable;

	HRESULT SetLight(int Index=0);
};

#endif // !defined(AFX_3DLIGHT_H__896A01D9_84A0_47E2_AD30_88B05F3FFD9C__INCLUDED_)
