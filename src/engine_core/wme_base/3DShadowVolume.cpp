// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\3DShadowVolume.h"
#include "3DUtils.h"

//////////////////////////////////////////////////////////////////////////
C3DShadowVolume::C3DShadowVolume(CBGame* inGame) : CBBase(inGame)
{
	m_NumVertices = 0;
	m_StencilMaskVB = NULL;
	m_Color = 0x7f000000;

	m_Vertices = NULL;
	m_VertBufferSize = 0;
}


//////////////////////////////////////////////////////////////////////////
C3DShadowVolume::~C3DShadowVolume(void)
{
	RELEASE(m_StencilMaskVB);
	free(m_Vertices);
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DShadowVolume::Reset()
{
	m_NumVertices = 0;
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DShadowVolume::AddMesh(LPD3DXMESH Mesh, DWORD* Adjacency, D3DXMATRIX* ModelMat, D3DXVECTOR3* Light, float ExtrusionDepth)
{
	if(!Mesh || !Adjacency) return E_FAIL;

	CBRenderD3D* Rend = (CBRenderD3D*)Game->m_Renderer;

	D3DXVECTOR3 InvLight;
	D3DXMATRIX MatInverseModel;
	D3DXMatrixInverse(&MatInverseModel, NULL, ModelMat);
	D3DXVec3TransformNormal(&InvLight, Light, &MatInverseModel);


	HRESULT res;

	// lock vertex buffer
	BYTE* Points = NULL;
	res = Mesh->LockVertexBufferUni(D3DLOCK_READONLY, &Points);
	if (FAILED(res)) return E_FAIL;

	// lock index buffer
	WORD* Indices;
	res = Mesh->LockIndexBufferUni(D3DLOCK_READONLY, &Indices);
	if (FAILED(res))
	{
		Mesh->UnlockVertexBuffer();
		return E_FAIL;
	}

	DWORD NumFaces = Mesh->GetNumFaces();
	DWORD NumVerts = Mesh->GetNumVertices();

	// Allocate a temporary edge list
	WORD* Edges = new WORD[NumFaces * 6];

	if(!Edges)
	{
		Mesh->UnlockVertexBuffer();
		Mesh->UnlockIndexBuffer();
		return E_OUTOFMEMORY;
	}
	DWORD NumEdges = 0;
	DWORD FvfSize = D3DXGetFVFVertexSize(Mesh->GetFVF());


	bool* IsFront = new bool[NumFaces];

	// First pass : for each face, record if it is front or back facing the light
	for(DWORD i=0; i<NumFaces; i++)
	{
		WORD Face0 = Indices[3*i+0];
		WORD Face1 = Indices[3*i+1];
		WORD Face2 = Indices[3*i+2];

		D3DXVECTOR3 v0 = *(D3DXVECTOR3*)(Points + Face0 * FvfSize);
		D3DXVECTOR3 v1 = *(D3DXVECTOR3*)(Points + Face1 * FvfSize);
		D3DXVECTOR3 v2 = *(D3DXVECTOR3*)(Points + Face2 * FvfSize);

		// Transform vertices or transform light?
		D3DXVECTOR3 vNormal;
		D3DXVec3Cross( &vNormal, &(v2-v1), &(v1-v0) );

		if( D3DXVec3Dot( &vNormal, &InvLight ) >= 0.0f )
		{
			IsFront[i] = false;	//	back face
		}
		else
		{
			IsFront[i] = true;	//	front face
		}
	}	

	// First pass : for each face, record if it is front or back facing the light
	for(DWORD i=0; i<NumFaces; i++)
	{
		if (IsFront[i])
		{
			WORD wFace0 = Indices[3*i+0];
			WORD wFace1 = Indices[3*i+1];
			WORD wFace2 = Indices[3*i+2];

			DWORD Adjacent0 = Adjacency[3 * i];
			DWORD Adjacent1 = Adjacency[3 * i + 1];
			DWORD Adjacent2 = Adjacency[3 * i + 2];

			if ((int)Adjacent0 < 0 || IsFront[Adjacent0] == false)
			{
				//	add edge v0-v1
				Edges[2*NumEdges+0] = wFace0;
				Edges[2*NumEdges+1] = wFace1;
				NumEdges++;
			}
			if ((int)Adjacent1 < 0 || IsFront[Adjacent1] == false)
			{
				//	add edge v1-v2
				Edges[2*NumEdges+0] = wFace1;
				Edges[2*NumEdges+1] = wFace2;
				NumEdges++;
			}
			if ((int)Adjacent2 < 0 || IsFront[Adjacent2] == false)
			{
				//	add edge v2-v0
				Edges[2*NumEdges+0] = wFace2;
				Edges[2*NumEdges+1] = wFace0;
				NumEdges++;
			}
		}
	}
	
	for(DWORD i = 0; i < NumEdges; i++)
	{
		D3DXVECTOR3 v1 = *(D3DXVECTOR3*)(Points + Edges[2*i+0] * FvfSize);
		D3DXVECTOR3 v2 = *(D3DXVECTOR3*)(Points + Edges[2*i+1] * FvfSize);
		D3DXVECTOR3 v3 = v1 - InvLight * ExtrusionDepth;
		D3DXVECTOR3 v4 = v2 - InvLight * ExtrusionDepth;

		// Add a quad (two triangles) to the vertex list
		AddVertex(&v1);
		AddVertex(&v2);
		AddVertex(&v3);

		AddVertex(&v2);
		AddVertex(&v4);
		AddVertex(&v3);
	}

	// Delete the temporary edge list
	delete [] Edges;
	delete [] IsFront;

	// Unlock the geometry buffers
	Mesh->UnlockVertexBuffer();
	Mesh->UnlockIndexBuffer();

	return S_OK;
}

#define VERT_BUF_GROW_BY 16000
//////////////////////////////////////////////////////////////////////////
void C3DShadowVolume::AddVertex(D3DXVECTOR3* Vertex)
{
	if((m_NumVertices + 1) * sizeof(D3DXVECTOR3) > m_VertBufferSize)
	{
		if(!m_Vertices) m_Vertices = (D3DXVECTOR3*)malloc(VERT_BUF_GROW_BY * sizeof(D3DXVECTOR3));
		else m_Vertices = (D3DXVECTOR3*)realloc(m_Vertices, m_VertBufferSize + VERT_BUF_GROW_BY * sizeof(D3DXVECTOR3));

		m_VertBufferSize += VERT_BUF_GROW_BY * sizeof(D3DXVECTOR3);
	}
	m_Vertices[m_NumVertices++] = *Vertex;
}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DShadowVolume::Render()
{
	CBRenderD3D* Rend = (CBRenderD3D*)Game->m_Renderer;

	Rend->m_LastTexture = NULL;
	Rend->m_Device->SetTexture(0, NULL);

	C3DUtils::SetFixedVertexShader(Rend->m_Device, D3DFVF_XYZ);
	Rend->m_Device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_NumVertices / 3, m_Vertices, sizeof(D3DXVECTOR3));
		
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DShadowVolume::RenderToStencilBuffer() 
{
	CBRenderD3D* Rend = (CBRenderD3D*)Game->m_Renderer;

	// Disable z-buffer writes (note: z-testing still occurs), and enable the
	// stencil-buffer
	Rend->m_Device->SetRenderState(D3DRS_ZWRITEENABLE,  FALSE);
	Rend->m_Device->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// Set up stencil compare fuction, reference value, and masks.
	// Stencil test passes if ((ref & mask) cmpfn (stencil & mask)) is true.
	// Note: since we set up the stencil-test to always pass, the STENCILFAIL
	// renderstate is really not needed.
	Rend->m_Device->SetRenderState(D3DRS_STENCILFUNC,  D3DCMP_ALWAYS);
	Rend->m_Device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	Rend->m_Device->SetRenderState(D3DRS_STENCILFAIL,  D3DSTENCILOP_KEEP);

	Rend->m_Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
	Rend->m_Device->SetRenderState(D3DRS_LIGHTING, FALSE);	


	// If z-test passes, inc/decrement stencil buffer value	
	Rend->m_Device->SetRenderState(D3DRS_STENCILREF,       0x1);
	Rend->m_Device->SetRenderState(D3DRS_STENCILMASK,      0xffffffff);
	Rend->m_Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);    
	

	// Make sure that no pixels get drawn to the frame buffer
	Rend->m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	Rend->m_Device->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_ZERO);
	Rend->m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	Rend->m_Device->SetRenderState(D3DRS_STENCILPASS,      D3DSTENCILOP_INCR);

	// Draw back-side of shadow volume in stencil/z only
	Rend->m_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	Render();



	// Decrement stencil buffer value
	Rend->m_Device->SetRenderState(D3DRS_STENCILPASS,      D3DSTENCILOP_DECR);

	// Draw front-side of shadow volume in stencil/z only
	Rend->m_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	Render();


	// Restore render states	
	Rend->m_Device->SetRenderState(D3DRS_LIGHTING, TRUE);
	Rend->m_Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	Rend->m_Device->SetRenderState(D3DRS_CULLMODE,  D3DCULL_CCW);
	Rend->m_Device->SetRenderState(D3DRS_ZWRITEENABLE,     TRUE) ;
	Rend->m_Device->SetRenderState(D3DRS_STENCILENABLE,    FALSE);
	Rend->m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DShadowVolume::RenderToScene() 
{
	if(!m_StencilMaskVB) InitMask();


	CBRenderD3D* Rend = (CBRenderD3D*)Game->m_Renderer;

	Rend->m_Device->SetRenderState(D3DRS_ZENABLE,          FALSE);
	Rend->m_Device->SetRenderState(D3DRS_STENCILENABLE,    TRUE);
	Rend->m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	Rend->m_Device->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
	Rend->m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	Rend->m_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	Rend->m_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	Rend->m_Device->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	Rend->m_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	Rend->m_Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	Rend->m_Device->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);

	// Only write where stencil val >= 1 (count indicates # of shadows that
	// overlap that pixel)
	Rend->m_Device->SetRenderState(D3DRS_STENCILREF,  0x1 );
	Rend->m_Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);
	Rend->m_Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

	Rend->m_Device->SetRenderState(D3DRS_FOGENABLE,          FALSE);

	// Draw a big, gray square
	Rend->m_Device->SetRenderState(D3DRS_COLORVERTEX, TRUE);


	C3DUtils::SetFixedVertexShader(Rend->m_Device, D3DFVF_SHADOWVOLVERTEX);
	C3DUtils::SetStreamSource(Rend->m_Device, 0, m_StencilMaskVB, sizeof(SHADOWVOLVERTEX));
	Rend->m_Device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);


	// Restore render states
	Rend->m_Device->SetRenderState(D3DRS_ZENABLE,          TRUE);
	Rend->m_Device->SetRenderState(D3DRS_STENCILENABLE,    FALSE);

	Rend->Setup3D(NULL, true);

	// clear stencil buffer
	Rend->m_Device->Clear(0, NULL, D3DCLEAR_STENCIL, 0x00, 1.0f, 0);

	/*
	Rend->m_Device->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	Render();
	Rend->m_Device->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	*/

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DShadowVolume::InitMask() 
{
	CBRenderD3D* Rend = (CBRenderD3D*)Game->m_Renderer;

	RELEASE(m_StencilMaskVB);

	Rend->m_Device->CreateVertexBufferUni(4 * sizeof(SHADOWVOLVERTEX),	D3DUSAGE_WRITEONLY, D3DFVF_SHADOWVOLVERTEX,	D3DPOOL_MANAGED, &m_StencilMaskVB);
	SHADOWVOLVERTEX* v;
	m_StencilMaskVB->LockVB(0, 0, &v, 0);
	{
		v[0].p = D3DXVECTOR4(0.0f, Rend->m_RealHeight, 0.0f, 1.0f);
		v[1].p = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
		v[2].p = D3DXVECTOR4(Rend->m_RealWidth, Rend->m_RealHeight, 0.0f, 1.0f);
		v[3].p = D3DXVECTOR4(Rend->m_RealWidth, 0.0f, 0.0f, 1.0f);
		v[0].color = m_Color;
		v[1].color = m_Color;
		v[2].color = m_Color;
		v[3].color = m_Color;
	}
	m_StencilMaskVB->Unlock();

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DShadowVolume::SetColor(DWORD Color) 
{
	if(Color != m_Color)
	{
		m_Color = Color;
		return InitMask();
	}
	else return S_OK;
}
