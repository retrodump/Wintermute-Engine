// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "3DLight.h"


//////////////////////////////////////////////////////////////////////////
C3DLight::C3DLight(CBGame* inGame):CBScriptable(inGame, false, false)
{
	m_DiffuseColor = DRGBA(255, 255, 255, 255);
	m_Pos = D3DXVECTOR3(0, 0, 0);
	m_Target = D3DXVECTOR3(0, 0, 0);
	m_IsSpotlight = false;
	m_Falloff = 0;
	m_Active = true;

	m_Distance = 0.0f;
	m_IsAvailable = false;
}


//////////////////////////////////////////////////////////////////////////
C3DLight::~C3DLight()
{

}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DLight::SetLight(int Index)
{
	LPDIRECT3DDEVICE Device = ((CBRenderD3D*)Game->m_Renderer)->m_Device;

	D3DLIGHT d3dLight;
	ZeroMemory(&d3dLight, sizeof(D3DLIGHT));

	d3dLight.Type = m_IsSpotlight ? D3DLIGHT_SPOT : D3DLIGHT_POINT;
	d3dLight.Diffuse.r = (float)D3DCOLGetR(m_DiffuseColor) / 256.0f;
	d3dLight.Diffuse.g = (float)D3DCOLGetG(m_DiffuseColor) / 256.0f;
	d3dLight.Diffuse.b = (float)D3DCOLGetB(m_DiffuseColor) / 256.0f;

	d3dLight.Position.x = m_Pos.x;
	d3dLight.Position.y = m_Pos.y;
	d3dLight.Position.z = m_Pos.z;
	
	if(m_IsSpotlight)
	{
		D3DXVECTOR3 Dir = m_Target - m_Pos;
		d3dLight.Direction = Dir;
		d3dLight.Range = D3DXVec3Length(&Dir);

		d3dLight.Theta        = 0.5f;
		d3dLight.Phi          = 1.0f;
		d3dLight.Falloff      = 1.0f;
		d3dLight.Attenuation0 = 1.0f;
	}
	else
	{
		d3dLight.Range = 100000.0f;
		d3dLight.Attenuation0 = 1.0f;
	}

	//d3dLight.Falloff = m_Falloff;

	//d3dLight.Attenuation0 = 100.0f; 
	
	
	HRESULT ret = Device->SetLight(Index, &d3dLight);

	if(m_Active)
		Device->LightEnable(Index, TRUE);

	return ret;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DLight::GetViewMatrix(D3DXMATRIX *ViewMatrix)
{
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(ViewMatrix, &m_Pos, &m_Target, &up);
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DLight::Persist(CBPersistMgr* PersistMgr)
{
	PersistMgr->Transfer(TMEMBER(m_Active));
	PersistMgr->Transfer(TMEMBER(m_DiffuseColor));
	return S_OK;
}
