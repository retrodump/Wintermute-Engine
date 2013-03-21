// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "XMesh.h"
#include "XModel.h"
#include "XFrameNode.h"
#include "3DUtils.h"
#include "3DEffect.h"
#include "3DEffectParams.h"
#include "3DEffectScript.h"
#include <float.h>


//////////////////////////////////////////////////////////////////////////
CXMesh::CXMesh(CBGame* inGame) : CBNamedObject(inGame)
{
	m_NumAttrs = 0;
	m_MaxFaceInfluence = 0;
	m_BufBoneCombinations = NULL;
	m_SkinMesh = NULL; 
	m_BlendedMesh = NULL;
	m_StaticMesh = NULL;

	m_BoneMatrices = NULL;
	m_Adjacency = NULL;
	m_SkinAdjacency = NULL;

	m_BBoxStart = m_BBoxEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}


//////////////////////////////////////////////////////////////////////////
CXMesh::~CXMesh()
{
	RELEASE(m_BufBoneCombinations);
	SAFE_DELETE(m_SkinMesh);
	RELEASE(m_BlendedMesh);
	RELEASE(m_StaticMesh);

	SAFE_DELETE_ARRAY(m_BoneMatrices);
	SAFE_DELETE_ARRAY(m_Adjacency);
	SAFE_DELETE_ARRAY(m_SkinAdjacency);

	for(int i=0; i<m_Materials.GetSize(); i++)
	{
		SAFE_DELETE(m_Materials[i]);
	}
	m_Materials.RemoveAll();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CXMesh::LoadFromXData(char* Filename, LPDIRECTXFILEDATA XObj)
{
	HRESULT res = S_OK;

	CBRenderD3D* Rend = (CBRenderD3D*)Game->m_Renderer;

	// get name
	res = CXModel::LoadName(this, XObj);
	if(FAILED(res))
	{
		Game->LOG(res, "Error loading mesh name");
		return res;
	}

	// load mesh
	LPD3DXBUFFER BufMaterials = NULL;
	LPD3DXBUFFER BufAdjacency = NULL;
	LPD3DXBUFFER BufBoneOffset = NULL;
	UINT NumFaces;

	DWORD NumMaterials;

#ifdef WME_D3D9
	LPD3DXMESH Mesh;
	LPD3DXSKININFO SkinInfo;

	res = D3DXLoadSkinMeshFromXof(XObj, D3DXMESH_MANAGED, Rend->m_Device, &BufAdjacency, 
		&BufMaterials, NULL, &NumMaterials, &SkinInfo, &Mesh);
	
	if(SUCCEEDED(res)) m_SkinMesh = new CSkinMeshHelper(Mesh, SkinInfo);
#else	
	LPD3DXBUFFER BufBoneNames = NULL;
	LPD3DXSKINMESH SkinMesh;
	res = D3DXLoadSkinMeshFromXof(XObj, D3DXMESH_MANAGED, Rend->m_Device, &BufAdjacency, 
		&BufMaterials, &NumMaterials, &BufBoneNames, &BufBoneOffset, &SkinMesh);
	
	if(SUCCEEDED(res)) m_SkinMesh = new CSkinMeshHelper(SkinMesh, BufBoneOffset, BufBoneNames);
#endif


	if(FAILED(res))
	{
		Game->LOG(res, "Error loading skin mesh");
		return res;
	}

	NumFaces = m_SkinMesh->GetNumFaces();


	DWORD NumBones = m_SkinMesh->GetNumBones();

	// Process skinning data
	if(NumBones)
	{
		// bones are available
		m_BoneMatrices = new D3DXMATRIX*[NumBones];

		DWORD* Adjacency = (DWORD*)(BufAdjacency->GetBufferPointer());
		m_SkinAdjacency = new DWORD[NumFaces * 3];
		memcpy(m_SkinAdjacency, Adjacency, NumFaces * 3 * sizeof(DWORD));

		GenerateMesh();
	}
	else
	{
		// no bones are found, blend the mesh and use it as a static mesh
		m_SkinMesh->GetOriginalMesh(&m_StaticMesh);
		m_StaticMesh->CloneMeshFVF(m_StaticMesh->GetOptions(), m_StaticMesh->GetFVF(), Rend->m_Device, &m_BlendedMesh);

		SAFE_DELETE(m_SkinMesh);
		m_NumAttrs = NumMaterials;

		if(m_BlendedMesh)
		{
			NumFaces = m_BlendedMesh->GetNumFaces();
			m_Adjacency = new DWORD[NumFaces * 3];
			m_BlendedMesh->GenerateAdjacency(0, m_Adjacency);
		}
	}


	// check for materials
	if((BufMaterials == NULL) || (NumMaterials == 0))
	{
		// no materials are found, create default material
		C3DMaterial* Mat = new C3DMaterial(Game);
		Mat->m_Material.Diffuse.r = 0.5f;
		Mat->m_Material.Diffuse.g = 0.5f;
		Mat->m_Material.Diffuse.b = 0.5f;
		Mat->m_Material.Specular = Mat->m_Material.Diffuse;
		Mat->m_Material.Ambient = Mat->m_Material.Diffuse;

		m_Materials.Add(Mat);

		m_NumAttrs = 1;
	}
	else
	{
		// texture relative path base
		char PathTemp[MAX_PATH+1];
		int PathLength;
		for (PathLength = strlen(Filename); PathLength--;)
		{
			if(Filename[PathLength] == '/' || Filename[PathLength] == '\\' )
			{
				PathLength++;
				break;
			}
		}
		strncpy(PathTemp, Filename, PathLength);


		// load the materials
		D3DXMATERIAL* FileMats = (D3DXMATERIAL*)BufMaterials->GetBufferPointer();
		for(int i = 0; i < (int)NumMaterials; i++)
		{
			C3DMaterial* Mat = new C3DMaterial(Game);
			Mat->m_Material = FileMats[i].MatD3D;
			Mat->m_Material.Ambient = Mat->m_Material.Diffuse;
						
			if(FileMats[i].pTextureFilename != NULL)
			{
				strcpy(PathTemp + PathLength, FileMats[i].pTextureFilename);
				Mat->SetTexture(PathTemp, true);
			}

			m_Materials.Add(Mat);
		}
		m_NumAttrs = m_Materials.GetSize();
	}

	RELEASE(BufAdjacency);
	RELEASE(BufMaterials);
	RELEASE(BufBoneOffset);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CXMesh::GenerateMesh()
{
	HRESULT res;
	DWORD NumFaces = m_SkinMesh->GetNumFaces();

	RELEASE(m_BlendedMesh);

	SAFE_DELETE_ARRAY(m_Adjacency);
	m_Adjacency = new DWORD[NumFaces * 3];

	// blend the mesh
	res = m_SkinMesh->GenerateSkinnedMesh(0, 0.0f, m_SkinAdjacency, m_Adjacency, &m_BlendedMesh);
	if(FAILED(res))
	{
		Game->LOG(res, "Error converting to blended mesh");
		return res;
	}
		
	// calculate the max face influence count
	if((m_BlendedMesh->GetFVF() & D3DFVF_POSITION_MASK) != D3DFVF_XYZ)
		m_MaxFaceInfluence = 1 + ((m_BlendedMesh->GetFVF() & D3DFVF_POSITION_MASK) - D3DFVF_XYZRHW) / 2;
	else
		m_MaxFaceInfluence = 1;

	//res = D3DXComputeNormals(m_BlendedMesh, NULL);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXMesh::FindBones(CXFrameNode* RootFrame)
{
	// normal meshes don't have bones
	if(!m_SkinMesh ) return S_OK;


	// get the buffer with the names of the bones
	for(DWORD i = 0; i < m_SkinMesh->GetNumBones(); i++)
	{
		// find a frame with the same name
		CXFrameNode* Frame = RootFrame->FindFrame(m_SkinMesh->GetBoneName(i));
		if(Frame)
		{
			// get a *pointer* to its world matrix
			m_BoneMatrices[i] = Frame->GetCombinedMatrix();
		}
		else
		{
			Game->LOG(0, "Warning: Cannot find frame '%s'", m_SkinMesh->GetBoneName(i));
		}
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXMesh::Update(CXFrameNode* ParentFrame)
{
	if(!m_BlendedMesh) return E_FAIL;

	CBRenderD3D* Rend = (CBRenderD3D*)Game->m_Renderer;
	HRESULT res = E_FAIL;

	// update skinned mesh
	if(m_SkinMesh)
	{
		int NumBones = m_SkinMesh->GetNumBones();
		D3DXMATRIX* BoneMatrices = new D3DXMATRIX[NumBones];

		// prepare final matrices
		for(int i=0; i<NumBones; i++)
		{
			D3DXMatrixMultiply(&BoneMatrices[i], m_SkinMesh->GetBoneOffsetMatrix(i), m_BoneMatrices[i]);
		}

		// generate skinned mesh
		res = m_SkinMesh->UpdateSkinnedMesh(BoneMatrices, m_BlendedMesh);
		delete [] BoneMatrices;

		if(FAILED(res))
		{
			Game->LOG(res, "Error updating skinned mesh");
			return res;
		}

		// update mesh bounding box
		BYTE* Points = NULL;
		res = m_BlendedMesh->LockVertexBufferUni(D3DLOCK_READONLY, &Points);
		if (FAILED(res))
		{
			Game->LOG(res, "Error updating bounding box");
			return res;
		}

#ifdef WME_D3D9
		D3DXComputeBoundingBox((LPD3DXVECTOR3)Points, m_BlendedMesh->GetNumVertices(), D3DXGetFVFVertexSize(m_BlendedMesh->GetFVF()), &m_BBoxStart, &m_BBoxEnd);
#else
		D3DXComputeBoundingBox((void*)Points, m_BlendedMesh->GetNumVertices(), m_BlendedMesh->GetFVF(), &m_BBoxStart, &m_BBoxEnd);
#endif
		// if you want something done right...
		if(_isnan(m_BBoxEnd.x))
		{
			float MinX = FLT_MAX;
			float MinY = FLT_MAX;
			float MinZ = FLT_MAX;
			float MaxX = FLT_MIN;
			float MaxY = FLT_MIN;
			float MaxZ = FLT_MIN;

			DWORD FvfSize = m_BlendedMesh->GetFVF();

			BYTE* VectBuf = (BYTE*)Points;			
			for(int i=0; i<m_BlendedMesh->GetNumVertices(); i++)
			{
				D3DXVECTOR3* Vect = (D3DXVECTOR3*)VectBuf;

				MinX = min(MinX, Vect->x);
				MinY = min(MinY, Vect->y);
				MinZ = min(MinZ, Vect->z);

				MaxX = max(MaxX, Vect->x);
				MaxY = max(MaxY, Vect->y);
				MaxZ = max(MaxZ, Vect->z);

				VectBuf += D3DXGetFVFVertexSize(FvfSize);
			}
			m_BBoxStart = D3DXVECTOR3(MinX, MinY, MinZ);
			m_BBoxEnd = D3DXVECTOR3(MaxX, MaxY, MaxZ);
		}

		m_BlendedMesh->UnlockVertexBuffer();
	}
	// update static mesh
	else
	{
		DWORD FvfSize = D3DXGetFVFVertexSize(m_BlendedMesh->GetFVF());
		DWORD NumVertices = m_BlendedMesh->GetNumVertices();

		// lock static vertex buffer
		BYTE* OldPoints = NULL;
		res = m_StaticMesh->LockVertexBufferUni(D3DLOCK_READONLY, &OldPoints);
		if (FAILED(res)) return res;

		// lock blended vertex buffer
		BYTE* NewPoints = NULL;
		res = m_BlendedMesh->LockVertexBufferUni(0, &NewPoints);
		if (FAILED(res))
		{
			m_StaticMesh->UnlockVertexBuffer();
			return res;
		}

		for(DWORD i=0; i<NumVertices; i++)
		{
			D3DXVECTOR3 v = *(D3DXVECTOR3*)(OldPoints + i * FvfSize);
			D3DXVECTOR4 NewVertex;
			D3DXVec3Transform(&NewVertex, &v, ParentFrame->GetCombinedMatrix());

			((D3DXVECTOR3*)(NewPoints + i * FvfSize))->x = NewVertex.x;
			((D3DXVECTOR3*)(NewPoints + i * FvfSize))->y = NewVertex.y;
			((D3DXVECTOR3*)(NewPoints + i * FvfSize))->z = NewVertex.z;
		}

		// update bounding box
#ifdef WME_D3D9
		D3DXComputeBoundingBox((LPD3DXVECTOR3)NewPoints, m_BlendedMesh->GetNumVertices(), D3DXGetFVFVertexSize(m_BlendedMesh->GetFVF()), &m_BBoxStart, &m_BBoxEnd);
#else
		D3DXComputeBoundingBox((void*)NewPoints, m_BlendedMesh->GetNumVertices(), m_BlendedMesh->GetFVF(), &m_BBoxStart, &m_BBoxEnd);
#endif
		m_StaticMesh->UnlockVertexBuffer();
		m_BlendedMesh->UnlockVertexBuffer();
	}
	return res;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXMesh::UpdateShadowVol(C3DShadowVolume* Shadow, D3DXMATRIX* ModelMat, D3DXVECTOR3* Light, float ExtrusionDepth)
{
	if(!m_BlendedMesh) return E_FAIL;
	return Shadow->AddMesh(m_BlendedMesh, m_Adjacency, ModelMat, Light, ExtrusionDepth);
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXMesh::Render(CXModel* model)
{
	if(!m_BlendedMesh) return E_FAIL;
	
	CBRenderD3D* Rend = (CBRenderD3D*)Game->m_Renderer;
	HRESULT res = E_FAIL;

	// render mesh
	LPDIRECT3DVERTEXBUFFER VertexBuffer;
	LPDIRECT3DINDEXBUFFER IndexBuffer;
	m_BlendedMesh->GetVertexBuffer(&VertexBuffer);
	m_BlendedMesh->GetIndexBuffer(&IndexBuffer);


	bool NoAttrs = false;
	DWORD NumAttrs;
	m_BlendedMesh->GetAttributeTable(NULL, &NumAttrs);
	if(NumAttrs==0)
	{
		NoAttrs = true;
		NumAttrs = 1;
	}

	D3DXATTRIBUTERANGE* Attrs = new D3DXATTRIBUTERANGE[NumAttrs];
	m_BlendedMesh->GetAttributeTable(Attrs, NULL);

	if(NoAttrs)
	{
		Attrs[0].AttribId = 0;
		Attrs[0].VertexStart = Attrs[0].FaceStart = 0;
		Attrs[0].VertexCount = m_BlendedMesh->GetNumVertices();
		Attrs[0].FaceCount = m_BlendedMesh->GetNumFaces();
	}

	C3DUtils::SetFixedVertexShader(Rend->m_Device, m_BlendedMesh->GetFVF());
	C3DUtils::SetStreamSource(Rend->m_Device, 0, VertexBuffer, D3DXGetFVFVertexSize(m_BlendedMesh->GetFVF()));
	C3DUtils::SetIndices(Rend->m_Device, IndexBuffer);

	for(int i = 0; i < NumAttrs; i++)
	{	
		C3DMaterial* Mat = m_Materials[Attrs[i].AttribId];

#ifdef WME_D3D9
		if(Mat->GetEffect())
		{
			Rend->m_Device->SetMaterial(&Mat->m_Material);

			// set shader parameters
			Mat->GetEffect()->UpdateGlobals();

			// matrices
			D3DXMATRIX matView, matProj, matWorld;
			Rend->m_Device->GetTransform(D3DTS_VIEW, &matView);
			Rend->m_Device->GetTransform(D3DTS_WORLD, &matWorld);
			Rend->m_Device->GetTransform(D3DTS_PROJECTION, &matProj);
			Mat->GetEffect()->UpdateMatrices(matView, matProj, matWorld);

			// camera
			if (Rend->m_Camera) Mat->GetEffect()->UpdateCamera(Rend->m_Camera->m_Position, Rend->m_Camera->m_Target);

			// material
			Mat->GetEffect()->UpdateMaterial(Mat);

			// lights
			Mat->GetEffect()->UpdateLights(model->m_Owner);

			// custom params
			C3DEffectParams* params = Mat->GetEffectParams();
			if (params) params->SetEffectParams(Mat->GetEffect()->GetEffect());


			ID3DXEffect* effect = Mat->GetEffect()->GetEffect();

			unsigned int numPasses;
			effect->Begin(&numPasses, 0);

			D3DXATTRIBUTERANGE Attr = Attrs[i];

			for (int pass = 0; pass < numPasses; pass++)
			{
				effect->BeginPass(pass);
				
				C3DEffectScript* script = Mat->GetEffect()->GetScriptForPass(pass);
				if (script)
				{
					script->InitFrame();
					while (script->Execute())
					{
						res = C3DUtils::DrawIndexedPrimitive(Rend->m_Device, D3DPT_TRIANGLELIST, Attr.VertexStart, Attr.VertexCount, Attr.FaceStart*3, Attr.FaceCount);

						// maintain polycount
						Rend->m_NumPolygons += Attr.FaceCount;
					}
				}
				effect->EndPass();
			}
			effect->End();
		}
#else
		if(false)
		{
		}
#endif
		else
		{
			// set material
			Rend->m_Device->SetMaterial(&Mat->m_Material);

			// set texture (if any)
			CBSurfaceD3D* Surf = (CBSurfaceD3D*)Mat->GetSurface();
			if(Surf && Surf->m_Texture)
				Rend->m_Device->SetTexture(0, Surf->m_Texture);
			else
				Rend->m_Device->SetTexture(0, NULL);

			// render
			//res = m_BlendedMesh->DrawSubset(i);
			D3DXATTRIBUTERANGE Attr = Attrs[i];		
			res = C3DUtils::DrawIndexedPrimitive(Rend->m_Device, D3DPT_TRIANGLELIST, Attr.VertexStart, Attr.VertexCount, Attr.FaceStart*3, Attr.FaceCount);

			// maintain polycount
			Rend->m_NumPolygons += Attr.FaceCount;
		}
	}

	RELEASE(VertexBuffer);
	RELEASE(IndexBuffer);


	delete [] Attrs;
	
	return res;
}
/*
//////////////////////////////////////////////////////////////////////////
void CXMesh::TempShadowVol()
{
	CBRenderD3D* Rend = (CBRenderD3D*)Game->m_Renderer;
	HRESULT res = E_FAIL;

	static C3DShadowVolume* Vol = NULL;
	if(!Vol)
	{
		Vol = new C3DShadowVolume(Game);
		Vol->InitMesh(m_BlendedMesh);		
	}	

	static int FrameCount = 0;
	FrameCount++;
	if(FrameCount>=3)
	{
		Vol->BuildShadowVolume(m_BlendedMesh, Game->m_TempLight, 300.0f);
		FrameCount = 0;
	}
	Vol->RenderToStencilBuffer();
	Vol->RenderToScene();


	
	//Rend->m_Device->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	//Vol->Render();
	//Rend->m_Device->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );	

	Rend->m_Device->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);


}
*/

//////////////////////////////////////////////////////////////////////////
bool CXMesh::PickPoly(D3DXVECTOR3* PickRayOrig, D3DXVECTOR3* PickRayDir)
{
	if(!m_BlendedMesh) return false;

	HRESULT res;

	DWORD FvfSize = D3DXGetFVFVertexSize(m_BlendedMesh->GetFVF());
	DWORD NumVertices = m_BlendedMesh->GetNumVertices();
	DWORD NumFaces = m_BlendedMesh->GetNumFaces();

	// lock vertex buffer
	BYTE* Points = NULL;
	res = m_BlendedMesh->LockVertexBufferUni(D3DLOCK_READONLY, &Points);
	if (FAILED(res)) return false;

	// lock index buffer
	WORD* Indices;
	res = m_BlendedMesh->LockIndexBufferUni(D3DLOCK_READONLY, &Indices);
	if (FAILED(res))
	{
		m_BlendedMesh->UnlockVertexBuffer();
		return false;
	}


	bool Found = false;
	D3DXVECTOR3 Intersection;

	for(DWORD i=0; i<NumFaces; i++)
	{
		WORD i1, i2, i3;
		i1 = Indices[3*i+0];
		i2 = Indices[3*i+1];
		i3 = Indices[3*i+2];

		D3DXVECTOR3 v0 = *(D3DXVECTOR3*)(Points + Indices[3*i+0] * FvfSize);
		D3DXVECTOR3 v1 = *(D3DXVECTOR3*)(Points + Indices[3*i+1] * FvfSize);
		D3DXVECTOR3 v2 = *(D3DXVECTOR3*)(Points + Indices[3*i+2] * FvfSize);

		if(_isnan(v0.x)) continue;

		Found = C3DUtils::IntersectTriangle(*PickRayOrig, *PickRayDir, v0, v1, v2, &Intersection.x, &Intersection.y, &Intersection.z) != FALSE;
		if(Found) break;
	}

	m_BlendedMesh->UnlockVertexBuffer();
	m_BlendedMesh->UnlockIndexBuffer();

	return Found;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXMesh::SetMaterialSprite(char* MatName, CBSprite* Sprite)
{
	for(int i=0; i<m_Materials.GetSize(); i++)
	{
		if(m_Materials[i]->m_Name && stricmp(m_Materials[i]->m_Name, MatName)==0)
		{
			m_Materials[i]->SetSprite(Sprite);
		}
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXMesh::SetMaterialTheora(char* MatName, CVidTheoraPlayer* Theora)
{
	for(int i=0; i<m_Materials.GetSize(); i++)
	{
		if(m_Materials[i]->m_Name && stricmp(m_Materials[i]->m_Name, MatName)==0)
		{
			m_Materials[i]->SetTheora(Theora);
		}
	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CXMesh::SetMaterialEffect(char* MatName, C3DEffect* Effect, C3DEffectParams* Params)
{
	for(int i=0; i<m_Materials.GetSize(); i++)
	{
		if(m_Materials[i]->m_Name && stricmp(m_Materials[i]->m_Name, MatName)==0)
		{
			m_Materials[i]->SetEffect(Effect, Params);
		}
	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CXMesh::RemoveMaterialEffect(char* MatName)
{
	for(int i=0; i<m_Materials.GetSize(); i++)
	{
		if(m_Materials[i]->m_Name && stricmp(m_Materials[i]->m_Name, MatName)==0)
		{
			m_Materials[i]->SetEffect(NULL, NULL);
		}
	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CXMesh::InvalidateDeviceObjects()
{
	if(m_SkinMesh)
	{
		RELEASE(m_BlendedMesh);
	}

	for(int i=0; i<m_Materials.GetSize(); i++)
		m_Materials[i]->InvalidateDeviceObjects();

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXMesh::RestoreDeviceObjects()
{
	for(int i=0; i<m_Materials.GetSize(); i++)
		m_Materials[i]->RestoreDeviceObjects();

	if(m_SkinMesh) return GenerateMesh();
	else return S_OK;
}
