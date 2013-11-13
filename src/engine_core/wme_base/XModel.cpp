// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\XModel.h"
#include "XFile.h"

IMPLEMENT_PERSISTENT(CXModel, false);

//////////////////////////////////////////////////////////////////////////
CXModel::CXModel(CBGame* inGame, CBObject* Owner) : CBObject(inGame)
{
	m_Owner = Owner;

	m_RootFrame = NULL;

	memset(&m_DrawingViewport, 0, sizeof(D3DVIEWPORT));
	D3DXMatrixIdentity(&m_LastWorldMat);
	D3DXMatrixIdentity(&m_LastViewMat);
	D3DXMatrixIdentity(&m_LastProjMat);
	m_LastOffsetX = m_LastOffsetY = 0;

	m_BBoxStart = m_BBoxEnd = D3DXVECTOR3(0,0,0);
	SetRectEmpty(&m_BoundingRect);

	for(int i=0; i<X_NUM_ANIMATION_CHANNELS; i++)
	{
		m_Channels[i] = NULL;
	}

	m_TicksPerSecond = 4800;

}

//////////////////////////////////////////////////////////////////////////
CXModel::~CXModel()
{
	Cleanup();
}

//////////////////////////////////////////////////////////////////////////
void CXModel::Cleanup(bool Complete)
{
	// empty animation channels
	for(int i=0; i<X_NUM_ANIMATION_CHANNELS; i++)
	{
		SAFE_DELETE(m_Channels[i]);
	}

	// remove animation sets
	for(int i=0; i<m_AnimationSets.GetSize(); i++)
	{
		delete m_AnimationSets[i];
	}
	m_AnimationSets.RemoveAll();

	// remove merged models filenames
	if(Complete)
	{
		for(int i=0; i<m_MergedModels.GetSize(); i++)
		{
			SAFE_DELETE_ARRAY(m_MergedModels[i]);
		}
		m_MergedModels.RemoveAll();
	}

	for(int i=0; i<m_MatSprites.GetSize(); i++)
	{
		SAFE_DELETE(m_MatSprites[i]);
	}
	m_MatSprites.RemoveAll();


	// remove root frame
	SAFE_DELETE(m_RootFrame);

	m_ParentModel = NULL;

	m_TicksPerSecond = 4800; // default ticks
}

#ifdef WME_D3D9
//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::LoadFromFile(char* Filename, CXModel* ParentModel)
{
	Cleanup(false);

	CXFile* XFile = new CXFile(Game);
	if(!XFile) return E_FAIL;


	LPDIRECTXFILEDATA XObj = NULL;
	HRESULT ResLoop=E_FAIL;

	m_ParentModel = ParentModel;

	HRESULT res = XFile->OpenFile(Filename);
	if(FAILED(res))
	{
		goto cleanup;
	}

	// get top level objects
	m_RootFrame = new CXFrameNode(Game);

	SIZE_T NumChildren;
	XFile->GetEnum()->GetChildren(&NumChildren);
	for(SIZE_T i=0; i<NumChildren; i++)
	{
		ResLoop = XFile->GetEnum()->GetChild(i, &XObj);
		if(FAILED(ResLoop)) break;

		res = m_RootFrame->LoadFromXData(Filename, this, XObj);
		if(FAILED(res))
		{
			RELEASE(XObj);
			Game->LOG(res, "Error loading top level object from '%s'", Filename);
			break;
		}
		RELEASE(XObj);
	}

	if(!m_RootFrame->HasChildren())
	{
		Game->LOG(ResLoop, "Error getting any top level objects in '%s'", Filename);
		res = E_FAIL;
	}

	if(SUCCEEDED(res)) res = FindBones(false, ParentModel);

	// setup animation channels
	for(int i=0; i<X_NUM_ANIMATION_CHANNELS; i++)
	{
		m_Channels[i] = new CXAnimationChannel(Game, this);
	}

	if(m_Filename != Filename) CBUtils::SetString(&m_Filename, Filename);

cleanup:
	RELEASE(XObj);
	SAFE_DELETE(XFile);

	return res;

}

//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::MergeFromFile(char* Filename)
{
	if(!m_RootFrame)
	{
		Game->LOG(0, "Error: CXModel::MergeFromFile called on an empty model");
		return E_FAIL;
	}

	CXFile* XFile = new CXFile(Game);
	if(!XFile) return E_FAIL;

	HRESULT res = XFile->OpenFile(Filename);
	if(FAILED(res))
	{
		goto cleanup;
	}


	LPDIRECTXFILEDATA XObj = NULL;
	HRESULT ResLoop;

	SIZE_T NumChildren;
	XFile->GetEnum()->GetChildren(&NumChildren);
	for(SIZE_T i=0; i<NumChildren; i++)
	{
		ResLoop = XFile->GetEnum()->GetChild(i, &XObj);
		if(FAILED(ResLoop)) break;

		res = m_RootFrame->MergeFromXData(Filename, this, XObj);
		if(FAILED(res))
		{
			Game->LOG(res, "Error loading top level object from '%s'", Filename);
			break;
		}
		RELEASE(XObj);
	}

	if(SUCCEEDED(res)) res = FindBones(true);

	// remember path for save/load purposes
	bool Found = false;
	for(int i=0; i<m_MergedModels.GetSize(); i++)
	{
		if(stricmp(m_MergedModels[i], Filename)==0)
		{
			Found = true;
			break;
		}
	}
	if(!Found)
	{
		char* Path = new char[strlen(Filename)+1];
		strcpy(Path, Filename);
		m_MergedModels.Add(Path);
	}

cleanup:
	RELEASE(XObj);
	SAFE_DELETE(XFile);

	return res;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::LoadAnimationSet(char* Filename, LPDIRECTXFILEDATA XObj)
{
	HRESULT res = S_OK;

	// create the animation set object
	CXAnimationSet* AnimSet = new CXAnimationSet(Game, this);
	res = LoadName(AnimSet, XObj);
	if(FAILED(res))
	{
		SAFE_DELETE(AnimSet);
		return res;
	}

	// use the filename for unnamed animation sets
	if(AnimSet->m_Name[0]=='\0')
	{
		char fname[_MAX_FNAME];
		_splitpath(Filename, NULL, NULL, fname, NULL);
		AnimSet->SetName(fname);
	}

	// query through the child objects to load the animations
	LPDIRECTXFILEDATA XChildData;
	GUID ObjectType;

	SIZE_T NumChildren;
	XObj->GetChildren(&NumChildren);

	for(SIZE_T i=0; i<NumChildren; i++)
	{
		Game->MiniUpdate();

		res = XObj->GetChild(i, &XChildData);
		if(SUCCEEDED(res))
		{
			res = XChildData->GetType(&ObjectType);
			if(FAILED(res))
			{
				RELEASE(XChildData);
				SAFE_DELETE(AnimSet);
				Game->LOG(res, "Error getting object type while loading animation set");
				return res;
			}

			if(ObjectType == TID_D3DRMAnimation)
			{
				res = LoadAnimation(Filename, XChildData, AnimSet);
				if(FAILED(res))
				{
					RELEASE(XChildData);
					SAFE_DELETE(AnimSet);
					return res;
				}
			}			
		}
		RELEASE(XChildData);
	}

	m_AnimationSets.Add(AnimSet);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::LoadAnimation(char* Filename, LPDIRECTXFILEDATA XObj, CXAnimationSet* ParentAnimSet)
{
	// if no parent anim set is specified, create one
	bool NewAnimSet = false;
	if(ParentAnimSet == NULL)
	{
		ParentAnimSet = new CXAnimationSet(Game, this);

		char fname[_MAX_FNAME];
		_splitpath(Filename, NULL, NULL, fname, NULL);
		ParentAnimSet->SetName(fname);

		NewAnimSet = true;
	}

	// create the new object
	CXAnimation* Anim = new CXAnimation(Game);

	// load the animation
	SIZE_T NumChildren;
	XObj->GetChildren(&NumChildren);

	for(SIZE_T i=0; i<NumChildren; i++)
	{
		LPDIRECTXFILEDATA XChildData = NULL;
		HRESULT res = XObj->GetChild(i, &XChildData);
		if(SUCCEEDED(res))
		{
			res = Anim->Load(XChildData, ParentAnimSet);
			if(FAILED(res))
			{
				RELEASE(XChildData);
				SAFE_DELETE(Anim);
				if(NewAnimSet) SAFE_DELETE(ParentAnimSet);
				return res;
			}
			RELEASE(XChildData);
		}
	}
	ParentAnimSet->AddAnimation(Anim);

	if(NewAnimSet) m_AnimationSets.Add(ParentAnimSet);

	return S_OK;
}
#else
//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::LoadFromFile(char* Filename, CXModel* ParentModel)
{	
	Cleanup(false);
	CXFile* XFile = new CXFile(Game);
	if(!XFile) return E_FAIL;


	LPDIRECTXFILEDATA XObj = NULL;
	HRESULT ResLoop=E_FAIL;

	m_ParentModel = ParentModel;

	HRESULT res = XFile->OpenFile(Filename);
	if(FAILED(res))
	{
		goto cleanup;
	}

	// get top level objects
	m_RootFrame = new CXFrameNode(Game);
	

	while(SUCCEEDED(ResLoop = XFile->GetEnum()->GetNextDataObject(&XObj)))
	{
		res = m_RootFrame->LoadFromXData(Filename, this, XObj);
		if(FAILED(res))
		{
			Game->LOG(res, "Error loading top level object from '%s'", Filename);
			break;
		}
		RELEASE(XObj);
	}


	if(!m_RootFrame->HasChildren())
	{
		Game->LOG(ResLoop, "Error getting any top level objects in '%s'", Filename);
		res = E_FAIL;
	}

	if(SUCCEEDED(res)) res = FindBones(false, ParentModel);

	// setup animation channels
	for(int i=0; i<X_NUM_ANIMATION_CHANNELS; i++)
	{
		m_Channels[i] = new CXAnimationChannel(Game, this);
	}

	if(m_Filename != Filename) CBUtils::SetString(&m_Filename, Filename);

cleanup:
	RELEASE(XObj);
	SAFE_DELETE(XFile);
	
	return res;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::MergeFromFile(char* Filename)
{
	if(!m_RootFrame)
	{
		Game->LOG(0, "Error: CXModel::MergeFromFile called on an empty model");
		return E_FAIL;
	}

	LPDIRECTXFILEDATA XObj = NULL;

	CXFile* XFile = new CXFile(Game);
	if(!XFile) return E_FAIL;

	HRESULT res = XFile->OpenFile(Filename);
	if(FAILED(res))
	{
		goto cleanup;
	}
	
	HRESULT ResLoop;
	while(SUCCEEDED(ResLoop = XFile->GetEnum()->GetNextDataObject(&XObj)))
	{
		res = m_RootFrame->MergeFromXData(Filename, this, XObj);
		if(FAILED(res))
		{
			Game->LOG(res, "Error loading top level object from '%s'", Filename);
			break;
		}
		RELEASE(XObj);
	}

	if(SUCCEEDED(res)) res = FindBones(true);

	// remember path for save/load purposes
	bool Found = false;
	for(int i=0; i<m_MergedModels.GetSize(); i++)
	{
		if(stricmp(m_MergedModels[i], Filename)==0)
		{
			Found = true;
			break;
		}
	}
	if(!Found)
	{
		char* Path = new char[strlen(Filename)+1];
		strcpy(Path, Filename);
		m_MergedModels.Add(Path);
	}

cleanup:
	RELEASE(XObj);
	SAFE_DELETE(XFile);

	return res;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::LoadAnimationSet(char* Filename, LPDIRECTXFILEDATA XObj)
{
	HRESULT res = S_OK;

	// create the animation set object
	CXAnimationSet* AnimSet = new CXAnimationSet(Game, this);
	res = LoadName(AnimSet, XObj);
	if(FAILED(res))
	{
		SAFE_DELETE(AnimSet);
		return res;
	}

	// use the filename for unnamed animation sets
	if(AnimSet->m_Name[0]=='\0')
	{
		char fname[_MAX_FNAME];
		_splitpath(Filename, NULL, NULL, fname, NULL);
		AnimSet->SetName(fname);
	}

	// query through the child objects to load the animations
	LPDIRECTXFILEDATA XChildData;
	LPDIRECTXFILEOBJECT XChildObj;	
	const GUID* ObjectType;

	while(SUCCEEDED(XObj->GetNextObject(&XChildObj)))
	{
		Game->MiniUpdate();

		// Query the child for it's FileData
		res = XChildObj->QueryInterface(IID_IDirectXFileData, (void**)&XChildData);
		if(SUCCEEDED(res))
		{
			res = XChildData->GetType(&ObjectType);
			if(FAILED(res))
			{
				RELEASE(XChildObj);
				RELEASE(XChildData);
				SAFE_DELETE(AnimSet);
				Game->LOG(res, "Error getting object type while loading animation set");
				return res;
			}

			if(*ObjectType == TID_D3DRMAnimation)
			{
				res = LoadAnimation(Filename, XChildData, AnimSet);
				if(FAILED(res))
				{
					RELEASE(XChildObj);
					RELEASE(XChildData);
					SAFE_DELETE(AnimSet);
					return res;
				}
			}

			RELEASE(XChildData);
		}
		RELEASE(XChildObj);	
	}

	m_AnimationSets.Add(AnimSet);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::LoadAnimation(char* Filename, LPDIRECTXFILEDATA XObj, CXAnimationSet* ParentAnimSet)
{
	// if no parent anim set is specified, create one
	bool NewAnimSet = false;
	if(ParentAnimSet == NULL)
	{
		ParentAnimSet = new CXAnimationSet(Game, this);

		char fname[_MAX_FNAME];
		_splitpath(Filename, NULL, NULL, fname, NULL);
		ParentAnimSet->SetName(fname);

		NewAnimSet = true;
	}

	// create the new object
	CXAnimation* Anim = new CXAnimation(Game);

	// load the animation
	LPDIRECTXFILEOBJECT XChildObj;
	while(SUCCEEDED(XObj->GetNextObject(&XChildObj)))
	{
		HRESULT res = Anim->Load(XChildObj, ParentAnimSet);
		if(FAILED(res))
		{
			RELEASE(XChildObj);
			SAFE_DELETE(Anim);
			if(NewAnimSet) SAFE_DELETE(ParentAnimSet);
			return res;
		}		
		RELEASE(XChildObj);
	}
	ParentAnimSet->AddAnimation(Anim);

	if(NewAnimSet) m_AnimationSets.Add(ParentAnimSet);

	return S_OK;
}
#endif


//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::FindBones(bool AnimOnly, CXModel* ParentModel)
{
	CXFrameNode* RootFrame;
	if(ParentModel==NULL) RootFrame = m_RootFrame;
	else RootFrame = ParentModel->GetRootFrame();

	if(RootFrame && !AnimOnly) m_RootFrame->FindBones(RootFrame);
	for(int i=0; i<m_AnimationSets.GetSize(); i++)
	{
		m_AnimationSets[i]->FindBones(RootFrame);
	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::LoadName(CBNamedObject* Obj, LPDIRECTXFILEDATA Data)
{
	return LoadName(&Obj->m_Name, Data);
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::LoadName(char** TargetStr, LPDIRECTXFILEDATA Data)
{
	DWORD NameLen;
	HRESULT res = Data->GetName(NULL, &NameLen);
	if(FAILED(res)) return res;

	if(NameLen > 0)
	{
		char* Name = new char[NameLen];

		res = Data->GetName(Name, &NameLen);
		if(FAILED(res))
		{
			delete[] Name;
			return res;
		}

		CBUtils::SetString(TargetStr, Name);
		delete[] Name;
	}
	else CBUtils::SetString(TargetStr, "");

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::Update()
{
	// reset all bones to default position
	Reset();
	
	// update all animation channels
	for(int i=0; i<X_NUM_ANIMATION_CHANNELS; i++)
	{
		m_Channels[i]->Update(i==1);
	}

	// update matrices
	if(m_RootFrame)
	{
		D3DXMATRIX TempMat;
		D3DXMatrixIdentity(&TempMat);
		m_RootFrame->UpdateMatrices(&TempMat);

		return m_RootFrame->UpdateMeshes();
	}
	else return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::PlayAnim(int Channel, char* Name, DWORD TransitionTime, bool ForceReset, DWORD StopTransitionTime)
{
	if(Channel < 0 || Channel >= X_NUM_ANIMATION_CHANNELS) return E_FAIL;

	// are we already playing this animation?
	if(!ForceReset)
	{
		if(m_Channels[Channel]->GetName() && stricmp(m_Channels[Channel]->GetName(), Name)==0)
			return S_OK;
	}

	//Game->LOG(0, "------ %d %s %d", Channel, Name, TransitionTime);

	// find animation set by name
	CXAnimationSet* Anim = GetAnimationSetByName(Name);
	if(Anim)
	{
		char* CurrentAnim = m_Channels[Channel]->GetName();
		if(m_Owner && CurrentAnim && Name) TransitionTime = m_Owner->GetAnimTransitionTime(CurrentAnim, Name);
		
		return m_Channels[Channel]->PlayAnim(Anim, TransitionTime, StopTransitionTime);
	}
	else
	{
		return E_FAIL;
	}

}


//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::StopAnim(int Channel, DWORD TransitionTime)
{
	if(Channel < 0 || Channel >= X_NUM_ANIMATION_CHANNELS) return E_FAIL;

	return m_Channels[Channel]->StopAnim(TransitionTime);
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::StopAnim(DWORD TransitionTime)
{
	for(int Channel=0; Channel<NUM_SKEL_ANIM_CHANNELS; Channel++)
		StopAnim(Channel, TransitionTime);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::Reset()
{
	if(m_RootFrame)
	{
		m_RootFrame->ResetMatrices();
	}
/*
	// !!! tiny.x hack
	{
		D3DXMATRIX RootMat;
		D3DXMatrixRotationX( &RootMat, -1.4f );
		D3DXMATRIX mat;
		D3DXMatrixRotationY( &mat, -D3DX_PI );
		D3DXMatrixMultiply( &RootMat, &RootMat, &mat );
		D3DXMatrixScaling( &mat, 0.2f, 0.2f, 0.2f );
		D3DXMatrixMultiply( &RootMat, &RootMat, &mat );
		D3DXMatrixTranslation(&mat, 0.0f, -5.0f, 0.0f);
		D3DXMatrixMultiply( &RootMat, &RootMat, &mat );
		m_RootFrame->SetTransformationMatrix(&RootMat);
	}
*/

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
bool CXModel::IsAnimPending(int Channel, char* AnimName)
{
	if(!AnimName)
	{
		if(m_Channels[Channel]->IsPlaying()) return true;
	}
	else
	{
		if(m_Channels[Channel]->IsPlaying() && m_Channels[Channel]->GetName() && stricmp(AnimName, m_Channels[Channel]->GetName())==0) return true;
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////
bool CXModel::IsAnimPending(char* AnimName)
{
	for(int Channel=0; Channel<X_NUM_ANIMATION_CHANNELS; Channel++)
	{
		if(IsAnimPending(Channel, AnimName)) return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::UpdateShadowVol(C3DShadowVolume* Shadow, D3DXMATRIX* ModelMat, D3DXVECTOR3* Light, float ExtrusionDepth)
{

	if(m_RootFrame)
	{
		return m_RootFrame->UpdateShadowVol(Shadow, ModelMat, Light, ExtrusionDepth);
	}
	else return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::Render()
{
	CBRenderD3D* Rend = (CBRenderD3D*)Game->m_Renderer;

	if(m_RootFrame)
	{
		// set culling
		if(m_Owner && !m_Owner->m_DrawBackfaces)
			Rend->m_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		else
			Rend->m_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		
		// gameSpace stores colors in vertices, disable for now
		Rend->m_Device->SetRenderState(D3DRS_COLORVERTEX, FALSE);

		// render everything
		HRESULT res = m_RootFrame->Render(this);

		// remember matrices for object picking purposes
		Rend->m_Device->GetTransform(D3DTS_WORLD,      &m_LastWorldMat);
		Rend->m_Device->GetTransform(D3DTS_VIEW,       &m_LastViewMat);
		Rend->m_Device->GetTransform(D3DTS_PROJECTION, &m_LastProjMat);

		// remember scene offset
		RECT rc;
		Game->GetCurrentViewportRect(&rc);
		float Width = (float)rc.right - (float)rc.left;
		float Height = (float)rc.bottom - (float)rc.top;

		// margins
		int mleft = rc.left;
		int mright = Rend->m_Width - Width - rc.left;
		int mtop = rc.top;
		int mbottom = Rend->m_Height - Height - rc.top;

		m_LastOffsetX = Game->m_OffsetX + (mleft - mright)/2;
		m_LastOffsetY = Game->m_OffsetY + (mtop - mbottom)/2;

		// update bounding box and 2D bounding rectangle
		UpdateBoundingRect();

		return res;
	}
	else return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
D3DXMATRIX* CXModel::GetBoneMatrix(char* BoneName)
{
	CXFrameNode* Bone = m_RootFrame->FindFrame(BoneName);
	if(Bone)
	{
		return Bone->GetCombinedMatrix();
	}
	else return NULL;
}

//////////////////////////////////////////////////////////////////////////
CXFrameNode* CXModel::GetRootFrame()
{
	return m_RootFrame;
}

//////////////////////////////////////////////////////////////////////////
bool CXModel::IsTransparentAt(int X, int Y)
{
	if(!m_RootFrame) return false;

	X += m_LastOffsetX;
	Y += m_LastOffsetY;

	CBRenderD3D* Rend = (CBRenderD3D*)Game->m_Renderer;
	if(!Rend->m_Camera) return true;


	float ResWidth, ResHeight;
	float LayerWidth, LayerHeight;
	float ModWidth, ModHeight;
	bool CustomViewport;
	Rend->GetProjectionParams(&ResWidth, &ResHeight, &LayerWidth, &LayerHeight, &ModWidth, &ModHeight, &CustomViewport);


	X-=m_DrawingViewport.X + ModWidth;
	Y-=m_DrawingViewport.Y + ModHeight;

	if(CustomViewport)
	{
		X += Rend->m_DrawOffsetX;
		Y += Rend->m_DrawOffsetY;
	}

	D3DXVECTOR3 PickRayDir;
	D3DXVECTOR3 PickRayOrig;

	// Compute the vector of the pick ray in screen space
	D3DXVECTOR3 vec;
	vec.x =  ((( 2.0f * X) / m_DrawingViewport.Width ) - 1) / m_LastProjMat._11;
	vec.y = -((( 2.0f * Y) / m_DrawingViewport.Height) - 1) / m_LastProjMat._22;
	vec.z =  1.0f;

	// Get the inverse view matrix
	D3DXMATRIX m;
	D3DXMatrixInverse(&m, NULL, &m_LastViewMat);

	// Transform the screen space pick ray into 3D space
	PickRayDir.x  = vec.x*m._11 + vec.y*m._21 + vec.z*m._31;
	PickRayDir.y  = vec.x*m._12 + vec.y*m._22 + vec.z*m._32;
	PickRayDir.z  = vec.x*m._13 + vec.y*m._23 + vec.z*m._33;
	PickRayOrig.x = m._41;
	PickRayOrig.y = m._42;
	PickRayOrig.z = m._43;

	// transform to model space
	D3DXVECTOR3 end = PickRayOrig + PickRayDir;
	D3DXMatrixInverse(&m, NULL, &m_LastWorldMat);
	D3DXVec3TransformCoord(&PickRayOrig, &PickRayOrig, &m);
	D3DXVec3TransformCoord(&end, &end, &m);
	PickRayDir = end - PickRayOrig;

	return !m_RootFrame->PickPoly(&PickRayOrig, &PickRayDir);
}


//////////////////////////////////////////////////////////////////////////
void CXModel::UpdateBoundingRect()
{
	m_BBoxStart = m_BBoxEnd = D3DXVECTOR3(0,0,0);
	if(m_RootFrame) m_RootFrame->GetBoundingBox(&m_BBoxStart, &m_BBoxEnd);


	m_BoundingRect.left = m_BoundingRect.top = INT_MAX;
	m_BoundingRect.right = m_BoundingRect.bottom = INT_MIN;

	CBRenderD3D* Rend = (CBRenderD3D*)Game->m_Renderer;
	LPDIRECT3DDEVICE dev = Rend->m_Device;

	D3DXMATRIX viewMat, projMat, worldMat;
	D3DXVECTOR3 vec2d(0,0,0);
	dev->GetTransform(D3DTS_VIEW, &viewMat);
	dev->GetTransform(D3DTS_PROJECTION, &projMat);
	dev->GetTransform(D3DTS_WORLD, &worldMat);

	dev->GetViewport(&m_DrawingViewport);

	float x1 = m_BBoxStart.x;
	float x2 = m_BBoxEnd.x;
	float y1 = m_BBoxStart.y;
	float y2 = m_BBoxEnd.y;
	float z1 = m_BBoxStart.z;
	float z2 = m_BBoxEnd.z;

	D3DXVec3Project(&vec2d, &D3DXVECTOR3(x1,y1,z1), &m_DrawingViewport, &projMat, &viewMat, &worldMat);
	UpdateRect(&m_BoundingRect, &vec2d);
	D3DXVec3Project(&vec2d, &D3DXVECTOR3(x2,y1,z1), &m_DrawingViewport, &projMat, &viewMat, &worldMat);
	UpdateRect(&m_BoundingRect, &vec2d);
	D3DXVec3Project(&vec2d, &D3DXVECTOR3(x1,y1,z2), &m_DrawingViewport, &projMat, &viewMat, &worldMat);
	UpdateRect(&m_BoundingRect, &vec2d);
	D3DXVec3Project(&vec2d, &D3DXVECTOR3(x2,y1,z2), &m_DrawingViewport, &projMat, &viewMat, &worldMat);
	UpdateRect(&m_BoundingRect, &vec2d);

	D3DXVec3Project(&vec2d, &D3DXVECTOR3(x1,y2,z1), &m_DrawingViewport, &projMat, &viewMat, &worldMat);
	UpdateRect(&m_BoundingRect, &vec2d);
	D3DXVec3Project(&vec2d, &D3DXVECTOR3(x2,y2,z1), &m_DrawingViewport, &projMat, &viewMat, &worldMat);
	UpdateRect(&m_BoundingRect, &vec2d);
	D3DXVec3Project(&vec2d, &D3DXVECTOR3(x1,y2,z2), &m_DrawingViewport, &projMat, &viewMat, &worldMat);
	UpdateRect(&m_BoundingRect, &vec2d);
	D3DXVec3Project(&vec2d, &D3DXVECTOR3(x2,y2,z2), &m_DrawingViewport, &projMat, &viewMat, &worldMat);
	UpdateRect(&m_BoundingRect, &vec2d);

	m_BoundingRect.left -= Rend->m_DrawOffsetX;
	m_BoundingRect.right -= Rend->m_DrawOffsetX;
	m_BoundingRect.bottom -= Rend->m_DrawOffsetY;
	m_BoundingRect.top -= Rend->m_DrawOffsetY;
}

//////////////////////////////////////////////////////////////////////////
void CXModel::UpdateRect(RECT* rc, D3DXVECTOR3* vec)
{
	rc->left   = min(rc->left,   vec->x);
	rc->right  = max(rc->right,  vec->x);
	rc->top    = min(rc->top,    vec->y);
	rc->bottom = max(rc->bottom, vec->y);
}

//////////////////////////////////////////////////////////////////////////
CXAnimationSet* CXModel::GetAnimationSetByName(char* Name)
{
	for(int i=0; i<m_AnimationSets.GetSize(); i++)
	{
		if(stricmp(Name, m_AnimationSets[i]->m_Name)==0)
		{
			return m_AnimationSets[i];
		}
	}
	return NULL;
}

TOKEN_DEF_START
	TOKEN_DEF (NAME)
	TOKEN_DEF (LOOPING)
	TOKEN_DEF (EVENT)
	TOKEN_DEF (FRAME)
TOKEN_DEF_END
//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::ParseAnim(BYTE* Buffer)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (NAME)
		TOKEN_TABLE (LOOPING)
		TOKEN_TABLE (EVENT)
	TOKEN_TABLE_END

	BYTE* params;
	int cmd;
	CBParser parser(Game);

	char* Name = NULL;
	bool Looping = false;
	bool LoopingSet = false;

	while ((cmd = parser.GetCommand ((char**)&Buffer, commands, (char**)&params)) > 0)
	{
		switch (cmd)
		{
		case TOKEN_NAME:
		{
			CBUtils::SetString(&Name, (char*)params);

			CXAnimationSet* Anim = GetAnimationSetByName(Name);
			if(!Anim) Game->LOG(0, "Error: Animation '%s' cannot be found in the model.", Name);

			break;
		}

		case TOKEN_LOOPING:
			parser.ScanStr((char*)params, "%b", &Looping);
			LoopingSet = true;
			break;

		case TOKEN_EVENT:
			if(!Name) Game->LOG(0, "Error: NAME filed must precede any EVENT fields in actor definition files.");
			else
			{
				CXAnimationSet* Anim = GetAnimationSetByName(Name);
				if(Anim) ParseEvent(Anim, params);
			}
			break;
		}
	}
	if (cmd != PARSERR_EOF) return E_FAIL;

	HRESULT Ret = S_OK;

	if(Name)
	{
		CXAnimationSet* Anim = GetAnimationSetByName(Name);
		if(Anim)
		{
			if(LoopingSet) Anim->m_Looping = Looping;
		}
	}
	SAFE_DELETE_ARRAY(Name);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::ParseEvent(CXAnimationSet* Anim, BYTE* Buffer)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (NAME)
		TOKEN_TABLE (FRAME)
	TOKEN_TABLE_END

	BYTE* params;
	int cmd;
	CBParser parser(Game);

	CXAnimationSet::CXAnimationEvent* Event = new CXAnimationSet::CXAnimationEvent();
	if(!Event) return E_FAIL;

	while ((cmd = parser.GetCommand ((char**)&Buffer, commands, (char**)&params)) > 0)
	{
		switch (cmd)
		{
		case TOKEN_NAME:
			CBUtils::SetString(&Event->m_EventName, (char*)params);
			break;

		case TOKEN_FRAME:
			parser.ScanStr((char*)params, "%d", &Event->m_Frame);
			break;
		}
	}
	if (cmd != PARSERR_EOF)
	{
		SAFE_DELETE(Event);
		return E_FAIL;
	}

	if(Event->m_EventName) Anim->AddEvent(Event);
	else
	{
		SAFE_DELETE(Event);
	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::SetMaterialSprite(char* MaterialName, char* SpriteFilename)
{
	if(!MaterialName || !SpriteFilename) return E_FAIL;
	if(!m_RootFrame) return E_FAIL;

	CBSprite* Sprite = new CBSprite(Game);
	if(!Sprite || FAILED(Sprite->LoadFile(SpriteFilename)))
	{
		SAFE_DELETE(Sprite);
		return E_FAIL;
	}

	CXModelMatSprite* MatSprite = NULL;
	for(int i=0; i<m_MatSprites.GetSize(); i++)
	{
		if(stricmp(m_MatSprites[i]->m_MatName, MaterialName)==0)
		{
			MatSprite = m_MatSprites[i];
			break;
		}
	}
	if(MatSprite)
	{
		MatSprite->SetSprite(Sprite);
	}
	else
	{
		MatSprite = new CXModelMatSprite(MaterialName, Sprite);
		m_MatSprites.Add(MatSprite);
	}
	m_RootFrame->SetMaterialSprite(MatSprite->m_MatName, MatSprite->m_Sprite);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::SetMaterialTheora(char* MaterialName, char* TheoraFilename)
{
	if(!MaterialName || !TheoraFilename) return E_FAIL;
	if(!m_RootFrame) return E_FAIL;

	CVidTheoraPlayer* Theora = new CVidTheoraPlayer(Game);
	if(!Theora || FAILED(Theora->Initialize(TheoraFilename)))
	{
		SAFE_DELETE(Theora);
		return E_FAIL;		
	}
	Theora->Play(VID_PLAY_POS, 0, 0, false, false, true);

	CXModelMatSprite* MatSprite = NULL;
	for(int i=0; i<m_MatSprites.GetSize(); i++)
	{
		if(stricmp(m_MatSprites[i]->m_MatName, MaterialName)==0)
		{
			MatSprite = m_MatSprites[i];
			break;
		}
	}
	if(MatSprite)
	{
		MatSprite->SetTheora(Theora);
	}
	else
	{
		MatSprite = new CXModelMatSprite(MaterialName, Theora);
		m_MatSprites.Add(MatSprite);
	}
	m_RootFrame->SetMaterialTheora(MatSprite->m_MatName, MatSprite->m_Theora);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::SetMaterialEffect(char* MaterialName, char* EffectFilename)
{
	if(!MaterialName || !EffectFilename) return E_FAIL;
	if(!m_RootFrame) return E_FAIL;


	C3DEffect* effect = new C3DEffect(Game);
	if(FAILED(effect->CreateFromFile(EffectFilename)))
	{
		SAFE_DELETE(effect);
		return E_FAIL;
	}

	CXModelMatSprite* MatSprite = NULL;
	for(int i=0; i<m_MatSprites.GetSize(); i++)
	{
		if(stricmp(m_MatSprites[i]->m_MatName, MaterialName)==0)
		{
			MatSprite = m_MatSprites[i];
			break;
		}
	}
	if(MatSprite)
	{
		MatSprite->SetEffect(effect);
	}
	else
	{
		MatSprite = new CXModelMatSprite(MaterialName, effect);
		m_MatSprites.Add(MatSprite);
	}
	m_RootFrame->SetMaterialEffect(MatSprite->m_MatName, MatSprite->m_Effect, MatSprite->m_EffectParams);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::RemoveMaterialEffect(char* MaterialName)
{
	if(!MaterialName) return E_FAIL;
	if(!m_RootFrame) return E_FAIL;


	CXModelMatSprite* MatSprite = NULL;
	for(int i=0; i<m_MatSprites.GetSize(); i++)
	{
		if(stricmp(m_MatSprites[i]->m_MatName, MaterialName)==0)
		{
			SAFE_DELETE(m_MatSprites[i]);
			m_MatSprites.RemoveAt(i);
			m_RootFrame->RemoveMaterialEffect(MaterialName);
			return S_OK;
		}
	}
	return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::SetMaterialEffectParam(char* MaterialName, char* ParamName, CScValue* Val)
{
	if(!MaterialName) return E_FAIL;
	if(!m_RootFrame) return E_FAIL;


	CXModelMatSprite* MatSprite = NULL;
	for(int i=0; i<m_MatSprites.GetSize(); i++)
	{
		if(stricmp(m_MatSprites[i]->m_MatName, MaterialName)==0)
		{
			if(m_MatSprites[i]->m_EffectParams)
			{
				m_MatSprites[i]->m_EffectParams->SetParam(ParamName, Val);
				return S_OK;
			}
			else return E_FAIL;
		}
	}
	return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::SetMaterialEffectParam(char* MaterialName, char* ParamName, D3DXVECTOR4 Val)
{
	if(!MaterialName) return E_FAIL;
	if(!m_RootFrame) return E_FAIL;


	CXModelMatSprite* MatSprite = NULL;
	for(int i=0; i<m_MatSprites.GetSize(); i++)
	{
		if(stricmp(m_MatSprites[i]->m_MatName, MaterialName)==0)
		{
			if(m_MatSprites[i]->m_EffectParams)
			{
				m_MatSprites[i]->m_EffectParams->SetParam(ParamName, Val);
				return S_OK;
			}
			else return E_FAIL;
		}
	}
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::InitializeSimple()
{
	if(!m_RootFrame) return E_FAIL;

	// init after load
	for(int i=0; i<m_MatSprites.GetSize(); i++)
	{
		if(m_MatSprites[i]->m_Theora)
			m_RootFrame->SetMaterialTheora(m_MatSprites[i]->m_MatName, m_MatSprites[i]->m_Theora);
		else if(m_MatSprites[i]->m_Sprite)
			m_RootFrame->SetMaterialSprite(m_MatSprites[i]->m_MatName, m_MatSprites[i]->m_Sprite);

		if(m_MatSprites[i]->m_EffectFile)
		{
			C3DEffect* effect = new C3DEffect(Game);
			if (SUCCEEDED(effect->CreateFromFile(m_MatSprites[i]->m_EffectFile)))
			{
				m_MatSprites[i]->m_Effect = effect;
				m_RootFrame->SetMaterialEffect(m_MatSprites[i]->m_MatName, m_MatSprites[i]->m_Effect, m_MatSprites[i]->m_EffectParams);
			}
			else SAFE_DELETE(effect);
		}
	}

	if(m_ParentModel) FindBones(false, m_ParentModel);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::Persist(CBPersistMgr* PersistMgr)
{
	CBObject::Persist(PersistMgr);

	PersistMgr->Transfer(TMEMBER(m_BBoxStart));
	PersistMgr->Transfer(TMEMBER(m_BBoxEnd));
	PersistMgr->Transfer(TMEMBER(m_BoundingRect));

	if(!PersistMgr->m_Saving)
	{
		memset(&m_DrawingViewport, 0, sizeof(D3DVIEWPORT));
	}

	PersistMgr->Transfer(TMEMBER(m_LastOffsetX));
	PersistMgr->Transfer(TMEMBER(m_LastOffsetY));

	PersistMgr->Transfer(TMEMBER(m_LastProjMat));
	PersistMgr->Transfer(TMEMBER(m_LastViewMat));
	PersistMgr->Transfer(TMEMBER(m_LastWorldMat));

	PersistMgr->Transfer(TMEMBER(m_Owner));	
	m_MergedModels.Persist(PersistMgr);

	// load model
	if(!PersistMgr->m_Saving)
	{
		for(int i=0; i<X_NUM_ANIMATION_CHANNELS; i++)
		{
			m_Channels[i] = NULL;
		}
		m_RootFrame = NULL;

		if(m_Filename) LoadFromFile(m_Filename);

		for(int i=0; i<m_MergedModels.GetSize(); i++)
		{
			MergeFromFile(m_MergedModels[i]);
		}
	}
	PersistMgr->Transfer(TMEMBER(m_ParentModel));

	// animation properties
	int NumAnims;
	if(PersistMgr->m_Saving) NumAnims = m_AnimationSets.GetSize();
	PersistMgr->Transfer(TMEMBER(NumAnims));

	if(PersistMgr->m_Saving)
	{
		for(int i=0; i<m_AnimationSets.GetSize(); i++)
		{
			PersistMgr->Transfer(TMEMBER(m_AnimationSets[i]->m_Name));
			m_AnimationSets[i]->Persist(PersistMgr);
		}
	}
	else
	{
		for(int i=0; i<NumAnims; i++)
		{
			bool NeedsDelete = false;
			char* AnimName;
			PersistMgr->Transfer(TMEMBER(AnimName));
			CXAnimationSet* AnimSet = GetAnimationSetByName(AnimName);
			if(!AnimSet)
			{
				AnimSet = new CXAnimationSet(Game, this);
				NeedsDelete = true;
			}
			AnimSet->Persist(PersistMgr);
			if(NeedsDelete) delete AnimSet;
			delete [] AnimName;
		}		
	}

	// persist channels
	for(int i=0; i<X_NUM_ANIMATION_CHANNELS; i++)
	{
		m_Channels[i]->Persist(PersistMgr);
	}

	// persist material sprites
	int NumMatSprites;
	if(PersistMgr->m_Saving) NumMatSprites = m_MatSprites.GetSize();
	PersistMgr->Transfer(TMEMBER(NumMatSprites));

	for(int i=0; i<NumMatSprites; i++)
	{
		if(PersistMgr->m_Saving) m_MatSprites[i]->Persist(PersistMgr);
		else
		{
			CXModelMatSprite* MatSprite = new CXModelMatSprite();
			MatSprite->Persist(PersistMgr);
			m_MatSprites.Add(MatSprite);
		}
	}


	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::InvalidateDeviceObjects()
{
	if(m_RootFrame) return m_RootFrame->InvalidateDeviceObjects();
	else return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::RestoreDeviceObjects()
{
	if(m_RootFrame) return m_RootFrame->RestoreDeviceObjects();
	else return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXModel::UnloadAnimation(char* AnimName)
{
	bool Found = false;
	for(int i=0; i<m_AnimationSets.GetSize(); i++)
	{
		if(stricmp(AnimName, m_AnimationSets[i]->m_Name)==0)
		{
			for(int j=0; j<X_NUM_ANIMATION_CHANNELS; j++)
			{
				if(m_Channels[j]) m_Channels[j]->UnloadAnim(m_AnimationSets[i]);
			}

			Found = true;
			SAFE_DELETE(m_AnimationSets[i]);
			m_AnimationSets.RemoveAt(i);
			i++;
		}
	}
	return Found?S_OK:E_FAIL;
}