// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once

#include "dcgf.h"


class C3DShadowVolume : public CBBase
{
public:
	C3DShadowVolume(CBGame* inGame);
	virtual ~C3DShadowVolume(void);
	
	HRESULT AddMesh(LPD3DXMESH Mesh, DWORD* Adjacency, D3DXMATRIX* ModelMat, D3DXVECTOR3* Light, float ExtrusionDepth);
	HRESULT Reset();

	HRESULT RenderToStencilBuffer();
	HRESULT RenderToScene();

	HRESULT SetColor(DWORD Color);

private:
	HRESULT Render();
	inline void AddVertex(D3DXVECTOR3* Vertex);
	DWORD m_Color;
	HRESULT InitMask();
	D3DXVECTOR3* m_Vertices; // Vertex data for rendering shadow volume
	int m_VertBufferSize;
	DWORD       m_NumVertices;
	LPDIRECT3DVERTEXBUFFER  m_StencilMaskVB;

};
