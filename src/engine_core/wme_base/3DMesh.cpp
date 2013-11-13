// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "3DMesh.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
C3DMesh::C3DMesh(CBGame* inGame):CBNamedObject(inGame)
{
	m_Vertices = NULL;
	m_Faces = NULL;
	m_VB = NULL;
	m_NumFaces = m_NumVertices = 0;
	m_MaterialIndex = -1;
	m_Visible = true;
}


//////////////////////////////////////////////////////////////////////////
C3DMesh::~C3DMesh()
{
	Cleanup();
}


//////////////////////////////////////////////////////////////////////////
void C3DMesh::Cleanup()
{
	SAFE_DELETE_ARRAY(m_Vertices);
	m_NumVertices = 0;

	SAFE_DELETE_ARRAY(m_Faces);
	m_NumFaces = 0;

	RELEASE(m_VB);
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DMesh::FillVertexBuffer()
{
	HRESULT ret;
	ret = CreateVertexBuffer();
	if(FAILED(ret)) return ret;


	MODELVERTEX* Verts;
	ret = m_VB->LockVB(0, 0, &Verts, 0);
	if(FAILED(ret)){
		Game->LOG(ret, "Error locking vertex buffer.");
		return ret;
	}
	
	for(int i=0; i<m_NumFaces; i++){
		for(int j=0; j<3; j++){
			int iOutVert = i*3+j;
			int iVertex = m_Faces[i].m_Vertices[j];

			Verts[iOutVert].x  = m_Vertices[iVertex].m_Pos.x;
			Verts[iOutVert].y  = m_Vertices[iVertex].m_Pos.y;
			Verts[iOutVert].z  = m_Vertices[iVertex].m_Pos.z;

			Verts[iOutVert].nx = m_Faces[i].m_Normals[j].x;
			Verts[iOutVert].ny = m_Faces[i].m_Normals[j].y;
			Verts[iOutVert].nz = m_Faces[i].m_Normals[j].z;

			Verts[iOutVert].u  = m_Faces[i].m_u[j];
			Verts[iOutVert].v  = m_Faces[i].m_v[j];
		}
	}

	ret = m_VB->Unlock();
	if(FAILED(ret)){
		Game->LOG(ret, "Error unlocking vertex buffer.");
		return ret;
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DMesh::CreateVertexBuffer()
{
	RELEASE(m_VB);

	if(m_NumFaces==0) return S_OK;

	int VBSize = m_NumFaces*sizeof(MODELVERTEX)*3;
	HRESULT ret = ((CBRenderD3D*)Game->m_Renderer)->m_Device->CreateVertexBufferUni(VBSize, 0, D3DFVF_MODELVERTEX, D3DPOOL_MANAGED, &m_VB);
	if(FAILED(ret)){
		Game->LOG(ret, "Error creating vertex buffer.");
		return ret;
	}
	else return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DMesh::FillVertexBuffer(DWORD Color)
{
	RELEASE(m_VB);

	if(m_NumFaces==0) return S_OK;

	int VBSize = m_NumFaces*sizeof(MODELVERTEXCOLOR)*3;
	HRESULT ret = ((CBRenderD3D*)Game->m_Renderer)->m_Device->CreateVertexBufferUni(VBSize, 0, D3DFVF_MODELVERTEXCOLOR, D3DPOOL_MANAGED, &m_VB);
	if(FAILED(ret)){
		Game->LOG(ret, "Error creating vertex buffer.");
		return ret;
	}


	MODELVERTEXCOLOR* Verts;
	ret = m_VB->LockVB(0, 0, &Verts, 0);
	if(FAILED(ret)){
		Game->LOG(ret, "Error locking vertex buffer.");
		return ret;
	}
	
	for(int i=0; i<m_NumFaces; i++){
		for(int j=0; j<3; j++){
			int iOutVert = i*3+j;
			int iVertex = m_Faces[i].m_Vertices[j];

			Verts[iOutVert].x  = m_Vertices[iVertex].m_Pos.x;
			Verts[iOutVert].y  = m_Vertices[iVertex].m_Pos.y;
			Verts[iOutVert].z  = m_Vertices[iVertex].m_Pos.z;

			Verts[iOutVert].nx = m_Faces[i].m_Normals[j].x;
			Verts[iOutVert].ny = m_Faces[i].m_Normals[j].y;
			Verts[iOutVert].nz = m_Faces[i].m_Normals[j].z;

			Verts[iOutVert].u  = m_Faces[i].m_u[j];
			Verts[iOutVert].v  = m_Faces[i].m_v[j];

			Verts[iOutVert].color1 = Color;
			Verts[iOutVert].color2 = Color;
		}
	}

	ret = m_VB->Unlock();
	if(FAILED(ret)){
		Game->LOG(ret, "Error unlocking vertex buffer.");
		return ret;
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DMesh::ComputeNormals()
{
	D3DXVECTOR3* pNormals = new D3DXVECTOR3[m_NumVertices];
	memset(pNormals, 0, sizeof(D3DXVECTOR3)*m_NumVertices);

	int i;

	for(i=0; i<m_NumFaces; i++){
		WORD a = m_Faces[i].m_Vertices[0];
		WORD b = m_Faces[i].m_Vertices[1];
		WORD c = m_Faces[i].m_Vertices[2];

		D3DXVECTOR3* v1 = &m_Vertices[a].m_Pos;
		D3DXVECTOR3* v2 = &m_Vertices[b].m_Pos;
		D3DXVECTOR3* v3 = &m_Vertices[c].m_Pos;

		D3DXVECTOR3 edge1 = *v2 - *v1;
		D3DXVECTOR3 edge2 = *v3 - *v2;
		D3DXVECTOR3 n;

		D3DXVec3Cross(&n, &edge1, &edge2);
		D3DXVec3Normalize(&n, &n);

		pNormals[a] += n;
		pNormals[b] += n;
		pNormals[c] += n;
	}

	// Assign the newly computed normals back to the vertices
	for(i=0; i<m_NumFaces; i++){
		for(int j=0; j<3; j++){
			D3DXVec3Normalize(&m_Faces[i].m_Normals[j], &pNormals[m_Faces[i].m_Vertices[j]]);
			//m_Faces[i].m_Normals[j] = pNormals[m_Faces[i].m_Vertices[j]];
		}
	}

	delete pNormals;

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DMesh::Persist(CBPersistMgr* PersistMgr)
{
	PersistMgr->Transfer(TMEMBER(m_Visible));
	return S_OK;
}
