// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "SkinMeshHelper.h"

#ifdef WME_D3D9

//////////////////////////////////////////////////////////////////////////
CSkinMeshHelper::CSkinMeshHelper(LPD3DXMESH Mesh, LPD3DXSKININFO SkinInfo)
{
	m_Mesh = Mesh;
	m_SkinInfo = SkinInfo;
}

//////////////////////////////////////////////////////////////////////////
CSkinMeshHelper::~CSkinMeshHelper()
{
	RELEASE(m_Mesh);
	RELEASE(m_SkinInfo);
}

//////////////////////////////////////////////////////////////////////////
DWORD CSkinMeshHelper::GetNumFaces()
{
	return m_Mesh->GetNumFaces();
}

//////////////////////////////////////////////////////////////////////////
DWORD CSkinMeshHelper::GetNumBones()
{
	return m_SkinInfo->GetNumBones();
}

//////////////////////////////////////////////////////////////////////////
HRESULT CSkinMeshHelper::GetOriginalMesh(LPD3DXMESH* ppMesh)
{
	LPDIRECT3DDEVICE Device;
	m_Mesh->GetDevice(&Device);
	HRESULT Res = m_Mesh->CloneMeshFVF(m_Mesh->GetOptions(), m_Mesh->GetFVF(), Device, ppMesh);
	RELEASE(Device);

	return Res;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CSkinMeshHelper::GenerateSkinnedMesh(DWORD Options, FLOAT minWeight, CONST LPDWORD pAdjacencyIn, LPDWORD pAdjacencyOut, LPD3DXMESH* ppMesh)
{
	HRESULT Res = GetOriginalMesh(ppMesh);
	if(SUCCEEDED(Res))
	{
		(*ppMesh)->GenerateAdjacency(0, pAdjacencyOut);
	}

	return Res;
	/*
	DWORD MaxFaceInfl;
	DWORD NumBoneComb;
	LPD3DXBUFFER BufBoneComb = NULL;
	HRESULT Ret = m_SkinInfo->ConvertToBlendedMesh(m_Mesh, Options, pAdjacencyIn, pAdjacencyOut, NULL, NULL, &MaxFaceInfl, &NumBoneComb, &BufBoneComb, ppMesh);
	RELEASE(BufBoneComb);

	return Ret;
	*/
}

//////////////////////////////////////////////////////////////////////////
HRESULT CSkinMeshHelper::UpdateSkinnedMesh(CONST D3DXMATRIX* pBoneTransforms, LPD3DXMESH pMesh)
{
	void* SourceVerts;
	void* TargetVerts;

	m_Mesh->LockVertexBuffer(D3DLOCK_READONLY, &SourceVerts);
	pMesh->LockVertexBuffer(0, &TargetVerts);

	HRESULT Ret = m_SkinInfo->UpdateSkinnedMesh(pBoneTransforms, NULL, SourceVerts, TargetVerts);

	m_Mesh->UnlockVertexBuffer();
	pMesh->UnlockVertexBuffer();

	return Ret;
}

//////////////////////////////////////////////////////////////////////////
const char* CSkinMeshHelper::GetBoneName(DWORD BoneIndex)
{
	return m_SkinInfo->GetBoneName(BoneIndex);
}

//////////////////////////////////////////////////////////////////////////
LPD3DXMATRIX CSkinMeshHelper::GetBoneOffsetMatrix(DWORD BoneIndex)
{
	return m_SkinInfo->GetBoneOffsetMatrix(BoneIndex);
}

#else


//////////////////////////////////////////////////////////////////////////
CSkinMeshHelper::CSkinMeshHelper(LPD3DXSKINMESH SkinMesh, LPD3DXBUFFER BufBoneOffset, LPD3DXBUFFER BufBoneNames)
{
	m_SkinMesh = SkinMesh;
	m_BufBoneNames = NULL;
	m_BoneOffsetMatrices = NULL;

	DWORD NumBones = GetNumBones();
	if(NumBones)
	{
		m_BoneOffsetMatrices = new D3DXMATRIX[NumBones];		
		memcpy(m_BoneOffsetMatrices, BufBoneOffset->GetBufferPointer(), NumBones*sizeof(D3DXMATRIX));

		m_BufBoneNames = BufBoneNames;
	}
}

//////////////////////////////////////////////////////////////////////////
CSkinMeshHelper::~CSkinMeshHelper()
{
	RELEASE(m_BufBoneNames);
	RELEASE(m_SkinMesh);
	SAFE_DELETE_ARRAY(m_BoneOffsetMatrices);
}

//////////////////////////////////////////////////////////////////////////
DWORD CSkinMeshHelper::GetNumFaces()
{
	return m_SkinMesh->GetNumFaces();
}

//////////////////////////////////////////////////////////////////////////
DWORD CSkinMeshHelper::GetNumBones()
{
	return m_SkinMesh->GetNumBones();
}

//////////////////////////////////////////////////////////////////////////
HRESULT CSkinMeshHelper::GetOriginalMesh(LPD3DXMESH* ppMesh)
{
	return m_SkinMesh->GetOriginalMesh(ppMesh);
}

//////////////////////////////////////////////////////////////////////////
HRESULT CSkinMeshHelper::GenerateSkinnedMesh(DWORD Options, FLOAT minWeight, CONST LPDWORD pAdjacencyIn, LPDWORD pAdjacencyOut, LPD3DXMESH* ppMesh)
{
	return m_SkinMesh->GenerateSkinnedMesh(Options, minWeight, pAdjacencyIn, pAdjacencyOut, NULL, NULL, ppMesh);
}

//////////////////////////////////////////////////////////////////////////
HRESULT CSkinMeshHelper::UpdateSkinnedMesh(CONST D3DXMATRIX* pBoneTransforms, LPD3DXMESH pMesh)
{
	return m_SkinMesh->UpdateSkinnedMesh(pBoneTransforms, NULL, pMesh);
}

//////////////////////////////////////////////////////////////////////////
const char* CSkinMeshHelper::GetBoneName(DWORD BoneIndex)
{
	char** BoneNames = (char**)(m_BufBoneNames->GetBufferPointer());
	return BoneNames[BoneIndex];
}

//////////////////////////////////////////////////////////////////////////
LPD3DXMATRIX CSkinMeshHelper::GetBoneOffsetMatrix(DWORD BoneIndex)
{
	return &m_BoneOffsetMatrices[BoneIndex];
}

#endif
