// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\XFrameNode.h"
#include "XModel.h"


//////////////////////////////////////////////////////////////////////////
CXFrameNode::CXFrameNode(CBGame* inGame) : CBNamedObject(inGame)
{
	D3DXMatrixIdentity(&m_TransformationMatrix);
	D3DXMatrixIdentity(&m_OriginalMatrix);
	D3DXMatrixIdentity(&m_CombinedMatrix);

	for(int i=0; i<2; i++)
	{
		m_TransPos[i]   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_TransScale[i] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		m_TransRot[i]   = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
		m_LerpValue[i]  = 0.0f;

		m_TransUsed[i]  = false;
	}
}


//////////////////////////////////////////////////////////////////////////
CXFrameNode::~CXFrameNode(void)
{
	// remove child frames
	for(int i=0; i<m_Frames.GetSize(); i++)
	{
		delete m_Frames[i];
	}
	m_Frames.RemoveAll();


	// remove meshes
	for(int i=0; i<m_Meshes.GetSize(); i++)
	{
		delete m_Meshes[i];
	}
	m_Meshes.RemoveAll();
}


//////////////////////////////////////////////////////////////////////////
D3DXMATRIX* CXFrameNode::GetCombinedMatrix()
{
	return &m_CombinedMatrix;
}


//////////////////////////////////////////////////////////////////////////
D3DXMATRIX* CXFrameNode::GetOriginalMatrix()
{
	return &m_OriginalMatrix;
}


//////////////////////////////////////////////////////////////////////////
void CXFrameNode::SetTransformationMatrix(D3DXMATRIX* Mat)
{
	m_TransformationMatrix = *Mat;
}


//////////////////////////////////////////////////////////////////////////
void CXFrameNode::SetTransformation(int Slot, D3DXVECTOR3 Pos, D3DXVECTOR3 Scale, D3DXQUATERNION Rot, float LerpValue)
{
	if(Slot < 0 || Slot > 1) return;

	m_TransPos[Slot]   = Pos;
	m_TransScale[Slot] = Scale;
	m_TransRot[Slot]   = Rot;
	m_LerpValue[Slot]  = LerpValue;

	m_TransUsed[Slot]  = true;
}

#ifdef WME_D3D9
//////////////////////////////////////////////////////////////////////////
HRESULT CXFrameNode::LoadFromXData(char* Filename, CXModel* Model, LPDIRECTXFILEDATA XObj)
{
	Game->MiniUpdate();

	HRESULT res = S_OK;

	// {9E415A43-7BA6-4a73-8743-B73D47E88476}
	GUID DXFILEOBJ_AnimTicksPerSecond = { 0x9e415a43, 0x7ba6, 0x4a73, { 0x87, 0x43, 0xb7, 0x3d, 0x47, 0xe8, 0x84, 0x76 } };

	// get the type of the object
	GUID ObjectType;
	res = XObj->GetType(&ObjectType);
	if(FAILED(res))
	{
		Game->LOG(res, "Error getting object type");
		return res;
	}

	// load a child mesh
	if(ObjectType == TID_D3DRMMesh)
	{
		CXMesh* Mesh = new CXMesh(Game);
		res = Mesh->LoadFromXData(Filename, XObj);
		if(SUCCEEDED(res))
		{
			m_Meshes.Add(Mesh);
			return S_OK;
		}
		else
		{
			SAFE_DELETE(Mesh);
			return res;
		}		
	}


	// load the transformation matrix
	else if(ObjectType == TID_D3DRMFrameTransformMatrix)
	{		
		D3DXMATRIX* mat;
		SIZE_T Size;

		res = XObj->Lock(&Size, (LPCVOID*)&mat);
		if(FAILED(res))
		{
			Game->LOG(res, "Error loading transformation matrix");
			return res;
		}
		else
		{
			m_TransformationMatrix = *mat;
			m_OriginalMatrix = *mat;
			XObj->Unlock();
			return S_OK;
		}
	}

	// load animation set
	else if(ObjectType == TID_D3DRMAnimationSet )
	{
		return Model->LoadAnimationSet(Filename, XObj);
	}

	// load a single animation (shouldn't happen here)
	else if(ObjectType == TID_D3DRMAnimation)
	{
		return Model->LoadAnimation(Filename, XObj);
	}

	// create a new child frame
	else if(ObjectType == TID_D3DRMFrame)
	{
		CXFrameNode* ChildFrame = new CXFrameNode(Game);

		// get the name of the child frame
		res = CXModel::LoadName(ChildFrame, XObj);
		if(FAILED(res))
		{
			Game->LOG(res, "Error loading frame name");
			SAFE_DELETE(ChildFrame);
			return res;
		}

		// Enumerate child objects.
		// Child object can be data, data reference or binary.
		// Use QueryInterface() to find what objecttype of object a child is.
		res = E_FAIL;

		SIZE_T NumChildren;
		XObj->GetChildren(&NumChildren);
		for(SIZE_T i=0; i<NumChildren; i++)
		{
			LPDIRECTXFILEDATA XChildData;
			res = XObj->GetChild(i, &XChildData);
			if(SUCCEEDED(res))
			{
				res = ChildFrame->LoadFromXData(Filename, Model, XChildData);
				RELEASE(XChildData);
			}
		}
		if(SUCCEEDED(res)) m_Frames.Add(ChildFrame);
		else
		{
			SAFE_DELETE(ChildFrame);
		}
		return res;
	}

	else if(ObjectType == DXFILEOBJ_AnimTicksPerSecond)
	{
		DWORD* TicksPerSecond;
		DWORD Size;

		res = XObj->Lock(&Size, (LPCVOID*)&TicksPerSecond);
		if(FAILED(res))
		{
			Game->LOG(res, "Error loading ticks per seconds info");
			return res;
		}
		else
		{
			Model->m_TicksPerSecond = *TicksPerSecond;
			XObj->Unlock();
			return S_OK;
		}
	}

	return S_OK; // unknown type, ignore
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXFrameNode::MergeFromXData(char* Filename, CXModel* Model, LPDIRECTXFILEDATA XObj)
{
	HRESULT res = S_OK;

	// get the type of the object
	GUID ObjectType;
	res = XObj->GetType(&ObjectType);
	if(FAILED(res))
	{
		Game->LOG(res, "Error getting object type");
		return res;
	}
	// load animation set
	else if(ObjectType == TID_D3DRMAnimationSet )
	{
		return Model->LoadAnimationSet(Filename, XObj);
	}

	// load a single animation (shouldn't happen here)
	else if(ObjectType == TID_D3DRMAnimation)
	{
		return Model->LoadAnimation(Filename, XObj);
	}

	// scan child frames
	else if(ObjectType == TID_D3DRMFrame)
	{
		// Enumerate child objects.
		// Child object can be data, data reference or binary.
		// Use QueryInterface() to find what objecttype of object a child is.
		res = E_FAIL;

		SIZE_T NumChildren;
		XObj->GetChildren(&NumChildren);
		for(SIZE_T i=0; i<NumChildren; i++)
		{
			LPDIRECTXFILEDATA XChildData;
			res = XObj->GetChild(i, &XChildData);
			if(SUCCEEDED(res))
			{
				// load the child's object
				res = MergeFromXData(Filename, Model, XChildData);
				RELEASE(XChildData);
			}
		}
		return res;
	}
	return S_OK; // unknown type, ignore
}

#else
//////////////////////////////////////////////////////////////////////////
HRESULT CXFrameNode::LoadFromXData(char* Filename, CXModel* Model, LPDIRECTXFILEDATA XObj)
{
	Game->MiniUpdate();

	HRESULT res = S_OK;

	// {9E415A43-7BA6-4a73-8743-B73D47E88476}
	GUID DXFILEOBJ_AnimTicksPerSecond = { 0x9e415a43, 0x7ba6, 0x4a73, { 0x87, 0x43, 0xb7, 0x3d, 0x47, 0xe8, 0x84, 0x76 } };

	// get the type of the object
	const GUID* ObjectType;
	res = XObj->GetType(&ObjectType);
	if(FAILED(res))
	{
		Game->LOG(res, "Error getting object type");
		return res;
	}

	// load a child mesh
	if(*ObjectType == TID_D3DRMMesh)
	{
		CXMesh* Mesh = new CXMesh(Game);
		res = Mesh->LoadFromXData(Filename, XObj);
		if(SUCCEEDED(res))
		{
			m_Meshes.Add(Mesh);
			return S_OK;
		}
		else
		{
			SAFE_DELETE(Mesh);
			return res;
		}		
	}

	
	// load the transformation matrix
	else if(*ObjectType == TID_D3DRMFrameTransformMatrix)
	{		
		D3DXMATRIX* mat;
		DWORD Size;

		res = XObj->GetData(NULL, &Size, (void**)&mat);
		if(FAILED(res))
		{
			Game->LOG(res, "Error loading transformation matrix");
			return res;
		}
		else
		{
			m_TransformationMatrix = *mat;
			m_OriginalMatrix = *mat;
			return S_OK;
		}
	}
	
	// load animation set
	else if(*ObjectType == TID_D3DRMAnimationSet )
	{
		return Model->LoadAnimationSet(Filename, XObj);
	}

	// load a single animation (shouldn't happen here)
	else if(*ObjectType == TID_D3DRMAnimation)
	{
		return Model->LoadAnimation(Filename, XObj);
	}

	// create a new child frame
	else if(*ObjectType == TID_D3DRMFrame)
	{
		CXFrameNode* ChildFrame = new CXFrameNode(Game);

		// get the name of the child frame
		res = CXModel::LoadName(ChildFrame, XObj);
		if(FAILED(res))
		{
			Game->LOG(res, "Error loading frame name");
			SAFE_DELETE(ChildFrame);
			return res;
		}

		// Enumerate child objects.
		// Child object can be data, data reference or binary.
		// Use QueryInterface() to find what objecttype of object a child is.
		LPDIRECTXFILEDATA XChildData;
		LPDIRECTXFILEOBJECT XChildObj;
		res = E_FAIL;
		while(SUCCEEDED(XObj->GetNextObject(&XChildObj)))
		{
			// Query the child for it's FileData
			res = XChildObj->QueryInterface(IID_IDirectXFileData, (void**)&XChildData);
			if(SUCCEEDED(res))
			{
				// load the child's object
				res = ChildFrame->LoadFromXData(Filename, Model, XChildData);
				RELEASE(XChildData);
			}
			RELEASE(XChildObj);
		}
		if(SUCCEEDED(res)) m_Frames.Add(ChildFrame);
		else
		{
			SAFE_DELETE(ChildFrame);
		}
		return res;
	}

	else if(*ObjectType == DXFILEOBJ_AnimTicksPerSecond)
	{
		DWORD* TicksPerSecond;
		DWORD Size;

		res = XObj->GetData(NULL, &Size, (void**)&TicksPerSecond);
		if(FAILED(res))
		{
			Game->LOG(res, "Error loading ticks per seconds info");
			return res;
		}
		else
		{
			Model->m_TicksPerSecond = *TicksPerSecond;
			return S_OK;
		}
	}

	return S_OK; // unknown type, ignore
}


//////////////////////////////////////////////////////////////////////////
HRESULT CXFrameNode::MergeFromXData(char* Filename, CXModel* Model, LPDIRECTXFILEDATA XObj)
{
	HRESULT res = S_OK;

	// get the type of the object
	const GUID* ObjectType;
	res = XObj->GetType(&ObjectType);
	if(FAILED(res))
	{
		Game->LOG(res, "Error getting object type");
		return res;
	}
	// load animation set
	else if(*ObjectType == TID_D3DRMAnimationSet )
	{
		return Model->LoadAnimationSet(Filename, XObj);
	}

	// load a single animation (shouldn't happen here)
	else if(*ObjectType == TID_D3DRMAnimation)
	{
		return Model->LoadAnimation(Filename, XObj);
	}

	// scan child frames
	else if(*ObjectType == TID_D3DRMFrame)
	{
		// Enumerate child objects.
		// Child object can be data, data reference or binary.
		// Use QueryInterface() to find what objecttype of object a child is.
		LPDIRECTXFILEDATA XChildData;
		LPDIRECTXFILEOBJECT XChildObj;
		res = E_FAIL;
		while(SUCCEEDED(XObj->GetNextObject(&XChildObj)))
		{
			// Query the child for it's FileData
			res = XChildObj->QueryInterface(IID_IDirectXFileData, (void**)&XChildData);
			if(SUCCEEDED(res))
			{
				// load the child's object
				res = MergeFromXData(Filename, Model, XChildData);
				RELEASE(XChildData);
			}
			RELEASE(XChildObj);
		}
		return res;
	}
	return S_OK; // unknown type, ignore
}
#endif

//////////////////////////////////////////////////////////////////////////
HRESULT CXFrameNode::FindBones(CXFrameNode* RootFrame)
{
	// find the bones of the meshes
	for(int i=0; i<m_Meshes.GetSize(); i++)
	{
		m_Meshes[i]->FindBones(RootFrame);
	}

	// find the bones for the child frames
	for(int i=0; i<m_Frames.GetSize(); i++)
	{
		m_Frames[i]->FindBones(RootFrame);
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
CXFrameNode* CXFrameNode::FindFrame(const char* FrameName)
{
	if(m_Name && strcmp(m_Name, FrameName)==0) return this;
	else
	{
		for(int i=0; i<m_Frames.GetSize(); i++)
		{
			CXFrameNode* FoundFrame = m_Frames[i]->FindFrame(FrameName);
			if(FoundFrame) return FoundFrame;
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXFrameNode::UpdateMatrices(D3DXMATRIX* ParentMat)
{
	if(m_TransUsed[0])
	{
		D3DXVECTOR3 TransPos = m_TransPos[0];
		D3DXVECTOR3 TransScale = m_TransScale[0];
		D3DXQUATERNION TransRot = m_TransRot[0];
		float LerpValue = m_LerpValue[0];

		if(m_TransUsed[1])
		{
			D3DXVec3Lerp(&TransScale, &TransScale, &m_TransScale[1], LerpValue);
			D3DXQuaternionSlerp(&TransRot, &TransRot, &m_TransRot[1], LerpValue);
			D3DXVec3Lerp(&TransPos, &TransPos, &m_TransPos[1], LerpValue);
		}


		// prepare local transformation matrix
		D3DXMatrixIdentity(&m_TransformationMatrix);
	
		D3DXMATRIX ScaleMat;
		D3DXMatrixScaling(&ScaleMat, TransScale.x, TransScale.y, TransScale.z);
		D3DXMatrixMultiply(&m_TransformationMatrix, &m_TransformationMatrix, &ScaleMat);

		D3DXMATRIX RotMat;
		D3DXMatrixRotationQuaternion(&RotMat, &TransRot);
		D3DXMatrixMultiply(&m_TransformationMatrix, &m_TransformationMatrix, &RotMat);

		D3DXMATRIX PosMat;
		D3DXMatrixTranslation(&PosMat, TransPos.x, TransPos.y, TransPos.z);
		D3DXMatrixMultiply(&m_TransformationMatrix, &m_TransformationMatrix, &PosMat);
	}
	m_TransUsed[0] = m_TransUsed[1] = false;


	// multiply by parent transformation
	D3DXMatrixMultiply(&m_CombinedMatrix, &m_TransformationMatrix, ParentMat);


	// update child frames
	for(int i=0; i<m_Frames.GetSize(); i++)
	{
		m_Frames[i]->UpdateMatrices(&m_CombinedMatrix);
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXFrameNode::UpdateMeshes()
{
	HRESULT res = S_OK;

	// update meshes
	for(int i=0; i<m_Meshes.GetSize(); i++)
	{
		res = m_Meshes[i]->Update(this);
		if(FAILED(res)) return res;
	}

	// render child frames
	for(int i=0; i<m_Frames.GetSize(); i++)
	{
		res = m_Frames[i]->UpdateMeshes();
		if(FAILED(res)) return res;
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXFrameNode::ResetMatrices()
{
	m_TransformationMatrix = m_OriginalMatrix;

	// update child frames
	for(int i=0; i<m_Frames.GetSize(); i++)
	{
		m_Frames[i]->ResetMatrices();
	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CXFrameNode::UpdateShadowVol(C3DShadowVolume* Shadow, D3DXMATRIX* ModelMat, D3DXVECTOR3* Light, float ExtrusionDepth)
{
	HRESULT res = S_OK;

	// meshes
	for(int i=0; i<m_Meshes.GetSize(); i++)
	{
		res = m_Meshes[i]->UpdateShadowVol(Shadow, ModelMat, Light, ExtrusionDepth);
		if(FAILED(res)) return res;
	}

	// child frames
	for(int i=0; i<m_Frames.GetSize(); i++)
	{
		res = m_Frames[i]->UpdateShadowVol(Shadow, ModelMat, Light, ExtrusionDepth);
		if(FAILED(res)) return res;
	}
	return S_OK;

}

//////////////////////////////////////////////////////////////////////////
HRESULT CXFrameNode::Render(CXModel* model)
{
	HRESULT res = S_OK;

	// render meshes
	for(int i=0; i<m_Meshes.GetSize(); i++)
	{
		res = m_Meshes[i]->Render(model);
		if(FAILED(res)) return res;
	}

	// render child frames
	for(int i=0; i<m_Frames.GetSize(); i++)
	{
		res = m_Frames[i]->Render(model);
		if(FAILED(res)) return res;
	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
bool CXFrameNode::PickPoly(D3DXVECTOR3* PickRayOrig, D3DXVECTOR3* PickRayDir)
{
	bool Found = false;
	for(int i=0; i<m_Meshes.GetSize(); i++)
	{
		Found = m_Meshes[i]->PickPoly(PickRayOrig, PickRayDir);
		if(Found) return true;
	}

	for(int i=0; i<m_Frames.GetSize(); i++)
	{
		Found = m_Frames[i]->PickPoly(PickRayOrig, PickRayDir);
		if(Found) return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXFrameNode::GetBoundingBox(D3DXVECTOR3* BoxStart, D3DXVECTOR3* BoxEnd)
{
	for(int i=0; i<m_Meshes.GetSize(); i++)
	{
		BoxStart->x = min(BoxStart->x, m_Meshes[i]->m_BBoxStart.x);
		BoxStart->y = min(BoxStart->y, m_Meshes[i]->m_BBoxStart.y);
		BoxStart->z = min(BoxStart->z, m_Meshes[i]->m_BBoxStart.z);

		BoxEnd->x = max(BoxEnd->x, m_Meshes[i]->m_BBoxEnd.x);
		BoxEnd->y = max(BoxEnd->y, m_Meshes[i]->m_BBoxEnd.y);
		BoxEnd->z = max(BoxEnd->z, m_Meshes[i]->m_BBoxEnd.z);
	}
	for(int i=0; i<m_Frames.GetSize(); i++)
	{
		m_Frames[i]->GetBoundingBox(BoxStart, BoxEnd);
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
bool CXFrameNode::HasChildren()
{
	return m_Frames.GetSize() > 0;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXFrameNode::SetMaterialSprite(char* MatName, CBSprite* Sprite)
{
	for(int i=0; i<m_Meshes.GetSize(); i++)
	{
		m_Meshes[i]->SetMaterialSprite(MatName, Sprite);
	}

	for(int i=0; i<m_Frames.GetSize(); i++)
	{
		m_Frames[i]->SetMaterialSprite(MatName, Sprite);
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXFrameNode::SetMaterialTheora(char* MatName, CVidTheoraPlayer* Theora)
{
	for(int i=0; i<m_Meshes.GetSize(); i++)
	{
		m_Meshes[i]->SetMaterialTheora(MatName, Theora);
	}

	for(int i=0; i<m_Frames.GetSize(); i++)
	{
		m_Frames[i]->SetMaterialTheora(MatName, Theora);
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXFrameNode::SetMaterialEffect(char* MatName, C3DEffect* Effect, C3DEffectParams* Params)
{
	for(int i=0; i<m_Meshes.GetSize(); i++)
	{
		m_Meshes[i]->SetMaterialEffect(MatName, Effect, Params);
	}

	for(int i=0; i<m_Frames.GetSize(); i++)
	{
		m_Frames[i]->SetMaterialEffect(MatName, Effect, Params);
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXFrameNode::RemoveMaterialEffect(char* MatName)
{
	for(int i=0; i<m_Meshes.GetSize(); i++)
	{
		m_Meshes[i]->RemoveMaterialEffect(MatName);
	}

	for(int i=0; i<m_Frames.GetSize(); i++)
	{
		m_Frames[i]->RemoveMaterialEffect(MatName);
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CXFrameNode::InvalidateDeviceObjects()
{
	for(int i=0; i<m_Meshes.GetSize(); i++)
	{
		m_Meshes[i]->InvalidateDeviceObjects();
	}

	for(int i=0; i<m_Frames.GetSize(); i++)
	{
		m_Frames[i]->InvalidateDeviceObjects();
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXFrameNode::RestoreDeviceObjects()
{
	for(int i=0; i<m_Meshes.GetSize(); i++)
	{
		m_Meshes[i]->RestoreDeviceObjects();
	}

	for(int i=0; i<m_Frames.GetSize(); i++)
	{
		m_Frames[i]->RestoreDeviceObjects();
	}
	return S_OK;
}
