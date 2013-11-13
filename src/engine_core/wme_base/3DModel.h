// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_3DMODEL_H__2832CC5E_4547_451F_9AF4_DAF16985BEC1__INCLUDED_)
#define AFX_3DMODEL_H__2832CC5E_4547_451F_9AF4_DAF16985BEC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BObject.h"

class C3DModel : public CBObject  
{
private:
	//////////////////////////////////////////////////////////////////////////
	class C3DCachedLoader
	{
	public:
		char* m_Filename;
		C3DLoaderMS3D* m_Loader;

		C3DCachedLoader()
		{
			m_Filename = NULL;
			m_Loader = NULL;
		}

		virtual ~C3DCachedLoader()
		{
			if(m_Filename) delete [] m_Filename;
			if(m_Loader) delete m_Loader;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	class C3DMeshInfo
	{
	public:
		char* m_Name;
		char* m_ParentBone;
		char* m_Filename;

		C3DMeshInfo(char* Name, char* Filename, char* ParentBone)
		{
			m_Name = NULL;
			if(Name) CBUtils::SetString(&m_Name, Name);

			m_Filename = NULL;
			if(Filename) CBUtils::SetString(&m_Filename, Filename);

			m_ParentBone = NULL;			
			if(ParentBone) CBUtils::SetString(&m_ParentBone, ParentBone);
		}

		virtual ~C3DMeshInfo()
		{
			if(m_Name) delete [] m_Name;
			if(m_Filename) delete [] m_Filename;
			if(m_ParentBone) delete [] m_ParentBone;
		}
	};

public:
	DECLARE_PERSISTENT(C3DModel, CBObject);

	CBObject* m_Owner;
	bool IsAnimPending(char* AnimName=NULL);
	bool IsAnimPending(int Channel, char* AnimName=NULL);
	HRESULT AnimStop();
	HRESULT AnimStop(int Channel);
	HRESULT AddMesh(char* Filename, char* MeshName, char* JointName);
	HRESULT ShowMesh(char* MeshName, bool Show=true);
	bool IsMeshVisible(char* MeshName);
	HRESULT DeleteMesh(char* MeshName);
	D3DVIEWPORT m_DrawingViewport;
	D3DXMATRIX m_LastWorldMat;
	D3DXMATRIX m_LastViewMat;
	D3DXMATRIX m_LastProjMat;
	int m_LastOffsetX;
	int m_LastOffsetY;
	bool IsTransparentAt(int X, int Y);
	RECT m_BoundingRect;
	void UpdateBoundingRect();
	D3DXVECTOR3 m_BBoxStart;
	D3DXVECTOR3 m_BBoxEnd;
	HRESULT SetAnimation(int Channel, char* Name, bool ForceReset=false);
	void SetDefaults();
	void Cleanup();
	void AnimRestart();
	void AnimRestart(int Channel);
	void AnimSetupJoints();
	void AnimAdvance();
	HRESULT PrepareMeshes();
	HRESULT Render();
	HRESULT Update();
	HRESULT LoadFile(char* Filename);
	HRESULT LoadFinalize();
	int m_NumMaterials;
	int m_NumJoints;
	int m_NumMeshes;
	CBArray<C3DMesh*, C3DMesh*> m_Meshes;
	C3DJoint* m_Joints;
	CBArray<C3DMaterial*, C3DMaterial*> m_Materials;
	C3DModel(CBGame* inGame, CBObject* Owner);
	virtual ~C3DModel();
	DWORD m_StartTime[NUM_SKEL_ANIM_CHANNELS];
	CBArray<C3DSkelAnim*, C3DSkelAnim*> m_Animations;
	
	CBArray<C3DCachedLoader*, C3DCachedLoader*> m_LoaderCache;
	C3DLoaderMS3D* ParseFileMS3D(char* Filename);

	CBArray<C3DMeshInfo*, C3DMeshInfo*> m_AttachedMeshes;
	CBArray<C3DMeshInfo*, C3DMeshInfo*> m_DeletedMeshes;

	HRESULT ClearLoaderCache();
	HRESULT ParseAnim(BYTE* Buffer);
	HRESULT ParseEvent(C3DSkelAnim* Anim, BYTE* Buffer);
private:
	C3DSkelAnim* m_CurrAnims[NUM_SKEL_ANIM_CHANNELS];
	void UpdateRect(RECT* rc, D3DXVECTOR3* vec);
};

#endif // !defined(AFX_3DMODEL_H__2832CC5E_4547_451F_9AF4_DAF16985BEC1__INCLUDED_)
