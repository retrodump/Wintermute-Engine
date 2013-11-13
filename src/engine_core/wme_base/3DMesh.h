// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_3DMESH_H__FEA7BD26_9610_411D_8B06_F46DF217FADE__INCLUDED_)
#define AFX_3DMESH_H__FEA7BD26_9610_411D_8B06_F46DF217FADE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class C3DMesh : public CBNamedObject
{
public:
	HRESULT Persist(CBPersistMgr* PersistMgr);
	HRESULT CreateVertexBuffer();
	int m_MaterialIndex;
	HRESULT ComputeNormals();
	void Cleanup();
	C3DMesh(CBGame* inGame);
	virtual ~C3DMesh();
	HRESULT FillVertexBuffer();
	HRESULT FillVertexBuffer(DWORD Color);
	C3DFace* m_Faces;
	WORD m_NumFaces;
	WORD m_NumVertices;
	C3DVertex* m_Vertices;
	LPDIRECT3DVERTEXBUFFER m_VB;
	bool m_Visible;
};

#endif // !defined(AFX_3DMESH_H__FEA7BD26_9610_411D_8B06_F46DF217FADE__INCLUDED_)
