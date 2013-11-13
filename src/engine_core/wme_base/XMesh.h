// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "bbase.h"
#include "SkinMeshHelper.h"

class CXFrameNode;

class CXMesh : public CBNamedObject
{
public:
	CXMesh(CBGame* inGame);
	virtual ~CXMesh();

	HRESULT LoadFromXData(char* Filename, LPDIRECTXFILEDATA XObj);
	HRESULT FindBones(CXFrameNode* RootFrame);
	HRESULT Update(CXFrameNode* ParentFrame);
	HRESULT Render(CXModel* model);
	HRESULT UpdateShadowVol(C3DShadowVolume* Shadow, D3DXMATRIX* ModelMat, D3DXVECTOR3* Light, float ExtrusionDepth);

	bool PickPoly(D3DXVECTOR3* PickRayOrig, D3DXVECTOR3* PickRayDir);

	D3DXVECTOR3 m_BBoxStart;
	D3DXVECTOR3 m_BBoxEnd;

	HRESULT SetMaterialSprite(char* MatName, CBSprite* Sprite);
	HRESULT SetMaterialTheora(char* MatName, CVidTheoraPlayer* Theora);
	HRESULT SetMaterialEffect(char* MatName, C3DEffect* Effect, C3DEffectParams* Params);
	HRESULT RemoveMaterialEffect(char* MatName);

	HRESULT InvalidateDeviceObjects();
	HRESULT RestoreDeviceObjects();

protected:
	HRESULT GenerateMesh();
	DWORD m_NumAttrs;
	DWORD m_MaxFaceInfluence;	

	LPD3DXBUFFER m_BufBoneCombinations;

	CSkinMeshHelper* m_SkinMesh; 
	LPD3DXMESH m_BlendedMesh; 
	LPD3DXMESH m_StaticMesh; 

	D3DXMATRIX** m_BoneMatrices;

	DWORD* m_SkinAdjacency;
	DWORD* m_Adjacency;

	CBArray<C3DMaterial*, C3DMaterial*> m_Materials;
};
