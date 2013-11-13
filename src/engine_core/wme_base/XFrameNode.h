// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BBase.h"
#include "XMesh.h"

class CXModel;


class CXFrameNode :	public CBNamedObject
{
public:
	CXFrameNode(CBGame* inGame);
	virtual ~CXFrameNode();

	HRESULT UpdateMatrices(D3DXMATRIX* ParentMat);
	HRESULT UpdateMeshes();
	HRESULT ResetMatrices();
	HRESULT Render(CXModel* model);
	HRESULT UpdateShadowVol(C3DShadowVolume* Shadow, D3DXMATRIX* ModelMat, D3DXVECTOR3* Light, float ExtrusionDepth);

	HRESULT LoadFromXData(char* Filename, CXModel* Model, LPDIRECTXFILEDATA XObj);
	HRESULT MergeFromXData(char* Filename, CXModel* Model, LPDIRECTXFILEDATA XObj);
	HRESULT FindBones(CXFrameNode* RootFrame);
	CXFrameNode* FindFrame(const char* FrameName);
	D3DXMATRIX* GetCombinedMatrix();
	D3DXMATRIX* GetOriginalMatrix();
	void SetTransformationMatrix(D3DXMATRIX* Mat);
	bool PickPoly(D3DXVECTOR3* PickRayOrig, D3DXVECTOR3* PickRayDir);
	HRESULT GetBoundingBox(D3DXVECTOR3* BoxStart, D3DXVECTOR3* BoxEnd);

	void SetTransformation(int Slot, D3DXVECTOR3 Pos, D3DXVECTOR3 Scale, D3DXQUATERNION Rot, float LerpValue);

	bool HasChildren();

	HRESULT SetMaterialSprite(char* MatName, CBSprite* Sprite);
	HRESULT SetMaterialTheora(char* MatName, CVidTheoraPlayer* Theora);
	HRESULT SetMaterialEffect(char* MatName, C3DEffect* Effect, C3DEffectParams* Params);
	HRESULT RemoveMaterialEffect(char* MatName);

	HRESULT InvalidateDeviceObjects();
	HRESULT RestoreDeviceObjects();

protected:
	CBArray<CXFrameNode*, CXFrameNode*> m_Frames;
	CBArray<CXMesh*, CXMesh*> m_Meshes;

	D3DXMATRIX m_TransformationMatrix;
	D3DXMATRIX m_OriginalMatrix;
	D3DXMATRIX m_CombinedMatrix;

	D3DXVECTOR3 m_TransPos[2];
	D3DXVECTOR3 m_TransScale[2];
	D3DXQUATERNION m_TransRot[2];
	bool m_TransUsed[2];
	float m_LerpValue[2];
};
