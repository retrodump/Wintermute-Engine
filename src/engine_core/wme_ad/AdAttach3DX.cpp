// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf_ad.h"
#include "AdAttach3DX.h"

IMPLEMENT_PERSISTENT(CAdAttach3DX, false);

//////////////////////////////////////////////////////////////////////////
CAdAttach3DX::CAdAttach3DX(CBGame* inGame, CBObject* Owner):CAdObject3D(inGame)
{
	m_Owner = Owner;
	m_ParentBone = NULL;
	m_DropToFloor = false;
}


//////////////////////////////////////////////////////////////////////////
CAdAttach3DX::~CAdAttach3DX(void)
{
	SAFE_DELETE_ARRAY(m_ParentBone);
	m_Owner = NULL; // ref only
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdAttach3DX::Init(char* ModelFile, char* Name, char* ParentBone)
{
	SAFE_DELETE(m_ModelX);

	CBUtils::SetString(&m_ParentBone, ParentBone);
	SetName(Name);

	m_ModelX = new CXModel(Game, m_Owner);
	if(!m_ModelX) return E_FAIL;

	HRESULT res = m_ModelX->LoadFromFile(ModelFile);
	if(FAILED(res))
	{
		SAFE_DELETE(m_ModelX);
	}
	return res;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdAttach3DX::Update()
{
	CAdObject3D::Update();
	if(m_ModelX) return m_ModelX->Update();
	else return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdAttach3DX::Display(D3DXMATRIX* ViewMat, bool RegisterObjects)
{
	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;

	D3DXMATRIX FinalMat;
	D3DXMatrixMultiply(&FinalMat, &m_WorldMatrix, ViewMat);
	m_Renderer->m_Device->SetTransform(D3DTS_WORLD, &FinalMat);

	if(m_ModelX) m_ModelX->Render();

	if(RegisterObjects && m_Owner && m_Owner->m_Registrable) Game->m_Renderer->m_RectList.Add(new CBActiveRect(Game, m_Owner, m_ModelX, m_ModelX->m_BoundingRect.left, m_ModelX->m_BoundingRect.top, m_ModelX->m_BoundingRect.right - m_ModelX->m_BoundingRect.left, m_ModelX->m_BoundingRect.bottom-m_ModelX->m_BoundingRect.top, true));

	m_Renderer->m_LastTexture = NULL;
		
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdAttach3DX::DisplayShadowVol(D3DXMATRIX* ModelMat, D3DXVECTOR3* Light, float ExtrusionDepth, bool Update)
{
	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;

	D3DXMATRIX FinalMat;
	D3DXMatrixMultiply(&FinalMat, &m_WorldMatrix, ModelMat);
	if(m_ModelX)
	{
		if(Update)
		{
			GetShadowVolume()->Reset();
			m_ModelX->UpdateShadowVol(GetShadowVolume(), &FinalMat, Light, ExtrusionDepth);
		}

		m_Renderer->m_Device->SetTransform(D3DTS_WORLD, &FinalMat);
		GetShadowVolume()->RenderToStencilBuffer();
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
char* CAdAttach3DX::GetParentBone()
{
	return m_ParentBone;
}

//////////////////////////////////////////////////////////////////////////
// high level scripting interface
//////////////////////////////////////////////////////////////////////////
HRESULT CAdAttach3DX::ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name)
{
	//////////////////////////////////////////////////////////////////////////
	// PlayAnim / PlayAnimAsync
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "PlayAnim")==0 || strcmp(Name, "PlayAnimAsync")==0)
	{
		Stack->CorrectParams(1);
		char* AnimName = Stack->Pop()->GetString();
		if(!m_ModelX || FAILED(m_ModelX->PlayAnim(0, AnimName, 0, true)))
			Stack->PushBool(false);
		else{
			if(strcmp(Name, "PlayAnimAsync")!=0) Script->WaitFor(this);
			Stack->PushBool(true);
		}		
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// StopAnim
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "StopAnim")==0)
	{
		Stack->CorrectParams(0);
		HRESULT Ret = E_FAIL;
		if(m_ModelX) Ret = m_ModelX->StopAnim(0);
		Stack->PushBool(SUCCEEDED(Ret));
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// StopAnimChannel
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "StopAnimChannel")==0){
		Stack->CorrectParams(1);
		int Channel = Stack->Pop()->GetInt();
		HRESULT Ret = E_FAIL;
		if(m_ModelX) Ret = m_ModelX->StopAnim(Channel, 0);
		Stack->PushBool(SUCCEEDED(Ret));
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// PlayAnimChannel / PlayAnimChannelAsync
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "PlayAnimChannel")==0 || strcmp(Name, "PlayAnimChannelAsync")==0)
	{
		Stack->CorrectParams(2);
		int Channel = Stack->Pop()->GetInt();
		char* Name = Stack->Pop()->GetString();
		if(!m_ModelX || FAILED(m_ModelX->PlayAnim(Channel, Name, 0, true))) Stack->PushBool(false);
		else{
			if(strcmp(Name, "PlayAnimChannelAsync")!=0) Script->WaitFor(this);
			Stack->PushBool(true);
		}		
		return S_OK;
	}

	else return CAdObject3D::ScCallMethod(Script, Stack, ThisStack, Name);
}

//////////////////////////////////////////////////////////////////////////
CScValue* CAdAttach3DX::ScGetProperty(char* Name)
{
	m_ScValue->SetNULL();

	//////////////////////////////////////////////////////////////////////////
	// Type
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Type")==0){
		m_ScValue->SetString("attachment");
		return m_ScValue;
	}
	else return CAdObject3D::ScGetProperty(Name);
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdAttach3DX::ScSetProperty(char *Name, CScValue *Value)
{
	/*
	//////////////////////////////////////////////////////////////////////////
	// TalkAnimName
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "TalkAnimName")==0){
		if(Value->IsNULL()) SetString(&m_TalkAnimName, "talk");
		else SetString(&m_TalkAnimName, Value->GetString());
		return S_OK;
	}
	else*/ return CAdObject3D::ScSetProperty(Name, Value);
}

//////////////////////////////////////////////////////////////////////////
char* CAdAttach3DX::ScToString()
{
	return "[attachment object]";
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdAttach3DX::Persist(CBPersistMgr* PersistMgr)
{
	CAdObject3D::Persist(PersistMgr);

	PersistMgr->Transfer(TMEMBER(m_Owner));
	PersistMgr->Transfer(TMEMBER(m_ParentBone));

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdAttach3DX::InvalidateDeviceObjects()
{
	if(m_ModelX) m_ModelX->InvalidateDeviceObjects();
	if(m_ShadowModel) m_ShadowModel->InvalidateDeviceObjects();

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdAttach3DX::RestoreDeviceObjects()
{
	if(m_ModelX) m_ModelX->RestoreDeviceObjects();
	if(m_ShadowModel) m_ShadowModel->RestoreDeviceObjects();
	return S_OK;
}
