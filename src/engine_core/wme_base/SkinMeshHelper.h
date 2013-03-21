// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once

class CSkinMeshHelper
{
public:
#ifdef WME_D3D9
	CSkinMeshHelper(LPD3DXMESH Mesh, LPD3DXSKININFO SkinInfo);
#else
	CSkinMeshHelper(LPD3DXSKINMESH SkinMesh, LPD3DXBUFFER BufBoneOffset, LPD3DXBUFFER BufBoneNames);
#endif
	virtual ~CSkinMeshHelper(void);

	DWORD GetNumFaces();
	DWORD GetNumBones();
	HRESULT GetOriginalMesh(LPD3DXMESH* ppMesh);
	HRESULT GenerateSkinnedMesh(DWORD Options, FLOAT minWeight, CONST LPDWORD pAdjacencyIn, LPDWORD pAdjacencyOut, LPD3DXMESH* ppMesh);
	HRESULT UpdateSkinnedMesh(CONST D3DXMATRIX* pBoneTransforms, LPD3DXMESH pMesh);
	const char* GetBoneName(DWORD BoneIndex);
	LPD3DXMATRIX GetBoneOffsetMatrix(DWORD BoneIndex);

private:
#ifdef WME_D3D9
	LPD3DXMESH m_Mesh;
	LPD3DXSKININFO m_SkinInfo;
#else
	LPD3DXSKINMESH m_SkinMesh;
	D3DXMATRIX* m_BoneOffsetMatrices;
	LPD3DXBUFFER m_BufBoneNames;
#endif
};
