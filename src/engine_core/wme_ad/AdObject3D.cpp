// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf_ad.h"
#include "AdObject3D.h"


IMPLEMENT_PERSISTENT(CAdObject3D, false);

//////////////////////////////////////////////////////////////////////////
CAdObject3D::CAdObject3D(CBGame* inGame):CAdObject(inGame)
{
	m_Is3D = true;
	
	m_Velocity = 1.0f;
	m_AngVelocity = 1.0f;
	m_LastPosVector = D3DXVECTOR3(0, 0, 0);	
	
	m_State = m_NextState = STATE_READY;
	m_DropToFloor = true;
	m_ShadowType = SHADOW_STENCIL;

	m_TempSkelAnim = NULL;

	m_ShadowVolume = NULL;

	m_AmbientLightColor = 0x00000000;
	m_HasAmbientLightColor = false;
}


//////////////////////////////////////////////////////////////////////////
CAdObject3D::~CAdObject3D()
{
	m_TempSkelAnim = NULL; // ref only
	SAFE_DELETE(m_ShadowVolume);

	ClearIgnoredLights();
}

//////////////////////////////////////////////////////////////////////////
void CAdObject3D::ClearIgnoredLights()
{
	for(int i=0; i<m_IgnoredLights.GetSize(); i++)
	{
		delete [] m_IgnoredLights[i];
	}
	m_IgnoredLights.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdObject3D::AddIgnoredLight(char* LightName)
{
	for(int i=0; i<m_IgnoredLights.GetSize(); i++)
	{
		if(stricmp(m_IgnoredLights[i], LightName) == 0) return S_OK;
	}
	char* Temp = new char[strlen(LightName)+1];
	strcpy(Temp, LightName);
	m_IgnoredLights.Add(Temp);
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdObject3D::RemoveIgnoredLight(char* LightName)
{
	for(int i=0; i<m_IgnoredLights.GetSize(); i++)
	{
		if(stricmp(m_IgnoredLights[i], LightName) == 0)
		{
			delete [] m_IgnoredLights[i];
			m_IgnoredLights.RemoveAt(i);
			return S_OK;
		}
	}
	return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdObject3D::Update()
{
	CAdGame* AdGame = (CAdGame*)Game;


	// drop to floor
	if(m_DropToFloor && AdGame->m_Scene && AdGame->m_Scene->m_Geom)
	{
		m_PosVector.y = AdGame->m_Scene->m_Geom->GetHeightAt(m_PosVector, 5.0f);
	}

	GetMatrix(&m_WorldMatrix);

	// setup 2D position
	if(m_PosVector!=m_LastPosVector)
	{
		m_LastPosVector = m_PosVector;

		Convert3DTo2D(&m_WorldMatrix, &m_PosX, &m_PosY);
	}

	
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdObject3D::Convert3DTo2D(D3DXMATRIX* WorldMat, int* PosX, int* PosY)
{
	LPDIRECT3DDEVICE dev = ((CBRenderD3D*)Game->m_Renderer)->m_Device;
	D3DXMATRIX viewMat, projMat;
	D3DXVECTOR3 vec2d(0,0,0);
	dev->GetTransform(D3DTS_VIEW, &viewMat);
	dev->GetTransform(D3DTS_PROJECTION, &projMat);

	D3DVIEWPORT vport;
	dev->GetViewport(&vport);

	D3DXVec3Project(&vec2d, &D3DXVECTOR3(0,0,0), &vport, &projMat, &viewMat, WorldMat);

	*PosX = vec2d.x + Game->m_OffsetX - Game->m_Renderer->m_DrawOffsetX;
	*PosY = vec2d.y + Game->m_OffsetY - Game->m_Renderer->m_DrawOffsetY;

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdObject3D::Display()
{
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdObject3D::SetupLights()
{
	CAdGame* AdGame = (CAdGame*)Game;

	if(AdGame->m_Scene && AdGame->m_Scene->m_Geom) return AdGame->m_Scene->m_Geom->EnableLights(m_PosVector, m_IgnoredLights);
	else return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
// high level scripting interface
//////////////////////////////////////////////////////////////////////////
HRESULT CAdObject3D::ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name)
{
	//////////////////////////////////////////////////////////////////////////
	// SkipTo
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "SkipTo")==0){
		Stack->CorrectParams(2);
		int x = Stack->Pop()->GetInt();
		int y = Stack->Pop()->GetInt();

		SkipTo(x, y);
		Stack->PushNULL();

		return S_OK;
	}
	//////////////////////////////////////////////////////////////////////////
	// SkipTo3D
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SkipTo3D")==0){
		Stack->CorrectParams(3);
		m_PosVector.x = Stack->Pop()->GetFloat();
		m_PosVector.y = Stack->Pop()->GetFloat();
		m_PosVector.z = Stack->Pop()->GetFloat();

		Stack->PushNULL();

		return S_OK;
	}
	//////////////////////////////////////////////////////////////////////////
	// GetBonePosition2D
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetBonePosition2D")==0)
	{
		Stack->CorrectParams(1);
		char* BoneName = Stack->Pop()->GetString();
		int X = 0, Y = 0;
		GetBonePosition2D(BoneName, &X, &Y);

		CScValue* Val = Stack->GetPushValue();
		if(Val)
		{
			Val->SetProperty("X", X);
			Val->SetProperty("Y", Y);
		}

		return S_OK;
	}
	//////////////////////////////////////////////////////////////////////////
	// GetBonePosition3D
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetBonePosition3D")==0)
	{
		Stack->CorrectParams(1);
		char* BoneName = Stack->Pop()->GetString();
		D3DXVECTOR3 Pos(0, 0, 0);
		GetBonePosition3D(BoneName, &Pos);

		CScValue* Val = Stack->GetPushValue();
		if(Val)
		{
			Val->SetProperty("X", Pos.x);
			Val->SetProperty("Y", Pos.y);
			Val->SetProperty("Z", Pos.z);
		}

		return S_OK;
	}
	//////////////////////////////////////////////////////////////////////////
	// AddIgnoredLight
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "AddIgnoredLight")==0)
	{
		Stack->CorrectParams(1);
		char* LightName = Stack->Pop()->GetString();
		Stack->PushBool(SUCCEEDED(AddIgnoredLight(LightName)));
		return S_OK;
	}
	//////////////////////////////////////////////////////////////////////////
	// RemoveIgnoredLight
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "RemoveIgnoredLight")==0)
	{
		Stack->CorrectParams(1);
		char* LightName = Stack->Pop()->GetString();
		Stack->PushBool(SUCCEEDED(RemoveIgnoredLight(LightName)));
		return S_OK;
	}

	else return CAdObject::ScCallMethod(Script, Stack, ThisStack, Name);
}


//////////////////////////////////////////////////////////////////////////
CScValue* CAdObject3D::ScGetProperty(char *Name)
{
	m_ScValue->SetNULL();

	//////////////////////////////////////////////////////////////////////////
	// Angle
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Angle")==0){
		m_ScValue->SetFloat(m_Angle);
		return m_ScValue;
	}
	//////////////////////////////////////////////////////////////////////////
	// PosX
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "PosX")==0){
		m_ScValue->SetFloat(m_PosVector.x);
		return m_ScValue;
	}
	//////////////////////////////////////////////////////////////////////////
	// PosY
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "PosY")==0){
		m_ScValue->SetFloat(m_PosVector.y);
		return m_ScValue;
	}
	//////////////////////////////////////////////////////////////////////////
	// PosZ
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "PosZ")==0){
		m_ScValue->SetFloat(m_PosVector.z);
		return m_ScValue;
	}
	//////////////////////////////////////////////////////////////////////////
	// Velocity
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Velocity")==0){
		m_ScValue->SetFloat(m_Velocity);
		return m_ScValue;
	}
	//////////////////////////////////////////////////////////////////////////
	// AngularVelocity / AngVelocity
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "AngularVelocity")==0 || strcmp(Name, "AngVelocity")==0){
		m_ScValue->SetFloat(m_AngVelocity);
		return m_ScValue;
	}
	//////////////////////////////////////////////////////////////////////////
	// DropToFloor
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "DropToFloor")==0){
		m_ScValue->SetBool(m_DropToFloor);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// ShadowType
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "ShadowType")==0)
	{
		m_ScValue->SetInt(m_ShadowType);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// Shadow (obsolete)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Shadow")==0)
	{
		m_ScValue->SetBool(m_ShadowType > SHADOW_NONE);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// SimpleShadow (obsolete)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SimpleShadow")==0)
	{
		m_ScValue->SetBool(m_ShadowType == SHADOW_SIMPLE);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// ShadowColor
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "ShadowColor")==0){
		m_ScValue->SetInt(m_ShadowColor);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// Scale
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Scale")==0){
		m_ScValue->SetFloat(m_Scale3D * 100.0f);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// DrawBackfaces
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "DrawBackfaces")==0){
		m_ScValue->SetBool(m_DrawBackfaces);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// AmbientLightColor
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "AmbientLightColor")==0)
	{
		if(m_HasAmbientLightColor) m_ScValue->SetInt(m_AmbientLightColor);
		else m_ScValue->SetNULL();

		return m_ScValue;
	}

	else return CAdObject::ScGetProperty(Name);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdObject3D::ScSetProperty(char *Name, CScValue *Value)
{

	//////////////////////////////////////////////////////////////////////////
	// Angle
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Angle")==0){
		m_Angle = Value->GetFloat();
		return S_OK;
	}
	//////////////////////////////////////////////////////////////////////////
	// PosX
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "PosX")==0){
		m_PosVector.x = Value->GetFloat();
		return S_OK;
	}
	//////////////////////////////////////////////////////////////////////////
	// PosY
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "PosY")==0){
		m_PosVector.y = Value->GetFloat();
		return S_OK;
	}
	//////////////////////////////////////////////////////////////////////////
	// PosZ
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "PosZ")==0){
		m_PosVector.z = Value->GetFloat();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// X
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "X")==0)
	{
		m_PosX = Value->GetInt();
		CAdGame* AdGame = (CAdGame*)Game;
		D3DXVECTOR3 pos;
		if(AdGame->m_Scene->m_Geom && AdGame->m_Scene->m_Geom->Convert2Dto3D(m_PosX, m_PosY, &pos))
		{
			m_PosVector = pos;
		}
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// Y
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Y")==0){
		m_PosY = Value->GetInt();
		CAdGame* AdGame = (CAdGame*)Game;
		D3DXVECTOR3 pos;
		if(AdGame->m_Scene->m_Geom && AdGame->m_Scene->m_Geom->Convert2Dto3D(m_PosX, m_PosY, &pos))
		{
			m_PosVector = pos;
		}
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// Velocity
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Velocity")==0){
		m_Velocity = Value->GetFloat();
		return S_OK;
	}
	//////////////////////////////////////////////////////////////////////////
	// AngularVelocity / AngVelocity
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "AngularVelocity")==0 || strcmp(Name, "AngVelocity")==0){
		m_AngVelocity = Value->GetFloat();
		return S_OK;
	}
	//////////////////////////////////////////////////////////////////////////
	// DropToFloor
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "DropToFloor")==0){
		m_DropToFloor = Value->GetBool();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// Shadow (obsolete)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Shadow")==0)
	{
		if(Value->GetBool()) m_ShadowType = SHADOW_STENCIL;
		else m_ShadowType = SHADOW_NONE;
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// ShadowType
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "ShadowType")==0)
	{
		m_ShadowType = (TShadowType)Value->GetInt();
		if(m_ShadowType < 0) m_ShadowType = SHADOW_NONE;
		if(m_ShadowType > SHADOW_STENCIL) m_ShadowType = SHADOW_STENCIL;

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SimpleShadow (obsolete)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SimpleShadow")==0)
	{
		if(Value->GetBool()) m_ShadowType = SHADOW_SIMPLE;
		else m_ShadowType = SHADOW_STENCIL;

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// ShadowColor
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "ShadowColor")==0){
		m_ShadowColor = Value->GetInt();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// Scale
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Scale")==0){
		m_Scale3D = Value->GetFloat() / 100.0f;
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// DrawBackfaces
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "DrawBackfaces")==0){
		m_DrawBackfaces = Value->GetBool();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// AmbientLightColor
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "AmbientLightColor")==0)
	{
		if(Value->IsNULL())
		{
			m_AmbientLightColor = 0x00000000;
			m_HasAmbientLightColor = false;
		}
		else
		{
			m_AmbientLightColor = Value->GetInt();
			m_HasAmbientLightColor = true;
		}
		return S_OK;
	}

	else return CAdObject::ScSetProperty(Name, Value);
}


//////////////////////////////////////////////////////////////////////////
char* CAdObject3D::ScToString()
{
	return "[ad object3d]";
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdObject3D::Persist(CBPersistMgr* PersistMgr)
{
	CAdObject::Persist(PersistMgr);

	PersistMgr->Transfer(TMEMBER(m_AngVelocity));
	PersistMgr->Transfer(TMEMBER(m_DropToFloor));
	PersistMgr->Transfer(TMEMBER(m_TempSkelAnim));
	PersistMgr->Transfer(TMEMBER(m_LastPosVector));
	PersistMgr->Transfer(TMEMBER(m_Velocity));	

	if(!PersistMgr->m_Saving)
	{
		m_ShadowVolume = NULL;
	}

	if(PersistMgr->CheckVersion(1, 8, 5))
	{
		m_IgnoredLights.Persist(PersistMgr);
	}
	else
	{
		ClearIgnoredLights();
	}

	if(PersistMgr->CheckVersion(1, 8, 10))
	{
		PersistMgr->Transfer(TMEMBER(m_AmbientLightColor));	
		PersistMgr->Transfer(TMEMBER(m_HasAmbientLightColor));	
	}
	else
	{
		m_AmbientLightColor = 0x00000000;
		m_HasAmbientLightColor = false;
	}


	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdObject3D::SkipTo(int X, int Y, bool Tolerant)
{
	CAdGame* AdGame = (CAdGame*)Game;
	D3DXVECTOR3 pos;
	
	bool Success;
	if(Tolerant) Success = AdGame->m_Scene->m_Geom && AdGame->m_Scene->m_Geom->Convert2Dto3DTolerant(X, Y, &pos);
	else Success = AdGame->m_Scene->m_Geom && AdGame->m_Scene->m_Geom->Convert2Dto3D(X, Y, &pos);

	if(Success)
	{
		m_PosVector = pos;
		m_PosX = X;
		m_PosY = Y;
		return S_OK;
	}
	else return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
C3DShadowVolume* CAdObject3D::GetShadowVolume() 
{
	if(m_ShadowVolume==NULL) m_ShadowVolume = new C3DShadowVolume(Game);
	return m_ShadowVolume;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdObject3D::GetBonePosition2D(char* BoneName, int* X, int* Y)
{
	if(!m_ModelX) return E_FAIL;

	CAdGame* AdGame = (CAdGame*)Game;
	if(!AdGame->m_Scene || !AdGame->m_Scene->m_Geom) return E_FAIL;

	D3DXMATRIX* BoneMat = m_ModelX->GetBoneMatrix(BoneName);
	if(!BoneMat) return E_FAIL;

	D3DXMATRIX BonePosMat;
	D3DXMatrixMultiply(&BonePosMat, BoneMat, &m_WorldMatrix);

	D3DXVECTOR4 VectBone4;
	D3DXVec3Transform(&VectBone4, &D3DXVECTOR3(0, 0, 0), &BonePosMat);
	D3DXVECTOR3 VectBone(VectBone4.x, VectBone4.y, VectBone4.z);

	AdGame->m_Scene->m_Geom->Convert3Dto2D(&VectBone, X, Y);
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdObject3D::GetBonePosition3D(char* BoneName, D3DXVECTOR3* Pos, D3DXVECTOR3* Offset)
{
	if(!m_ModelX) return E_FAIL;

	D3DXMATRIX* BoneMat = m_ModelX->GetBoneMatrix(BoneName);
	if(!BoneMat) return E_FAIL;

	if(!Offset) Offset = &D3DXVECTOR3(0, 0, 0);

	D3DXMATRIX BonePosMat;
	D3DXMatrixMultiply(&BonePosMat, BoneMat, &m_WorldMatrix);

	D3DXVECTOR4 VectBone4;
	D3DXVec3Transform(&VectBone4, Offset, &BonePosMat);

	Pos->x = VectBone4.x;
	Pos->y = VectBone4.y;
	Pos->z = VectBone4.z;

	return S_OK;
}